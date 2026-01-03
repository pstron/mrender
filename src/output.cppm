module;
#include <cstddef>
#include <linux/kd.h>
#include <sys/ioctl.h>
#include <unistd.h>
export module output;

import std;
import color;
import framebuffer;

export namespace mrender {

enum class OutputType { Text, Binary, FrameBuffer };

struct Size {
  std::size_t x;
  std::size_t y;
};

class Output {
public:
  constexpr Output(Size sz, OutputType ot = OutputType::Binary) noexcept
      : size(sz), outputType(ot), pixels(sz.x * sz.y) {}

  void display() const {
    switch (outputType) {
    case OutputType::Text:
    case OutputType::Binary:
      print();
      break;
    case OutputType::FrameBuffer:
      try {
        static FrameBuffer fb;
        fb.blit(pixels, size.x, size.y);
      } catch (const std::exception &e) {
        std::cerr << "[FATAL] " << e.what() << '\n';
      } catch (...) {
        std::cerr << "[FATAL] unknown error\n";
      }
      break;
    }
  }

  void set_pixel(std::size_t x, std::size_t y, rgb color) noexcept {
    pixels[y * size.x + x] = color;
  }

  rgb get_pixel(std::size_t x, std::size_t y) const noexcept {
    return pixels[y * size.x + x];
  }

  Size get_size() const noexcept { return size; }

  Output(Output &&) noexcept = default;
  Output(const Output &) = default;
  Output &operator=(Output &&) noexcept = default;
  Output &operator=(const Output &) = default;
  ~Output() = default;

private:
  OutputType outputType;
  Size size;
  std::vector<rgb> pixels;

  void print() const noexcept {
    print_header();
    print_content();
  }

  void print_header() const noexcept {
    switch (outputType) {
    default:
      std::cerr << "[ERROR] Print header unsupported output type\n";
      break;
    case OutputType::Text:
      std::println("P3");
      break;
    case OutputType::Binary:
      std::println("P6");
      break;
    }
    std::println("{} {}", size.x, size.y);
    std::println("255");
  }

  void print_pixel(rgb color) const noexcept {
    switch (outputType) {
    default:
      std::cerr << "[ERROR] Print content unsupported output type\n";
      break;
    case OutputType::Text:
      std::print("{} {} {} ", color.r, color.g, color.b);
      break;
    case OutputType::Binary:
      const unsigned char rgb_data[3] = {static_cast<unsigned char>(color.r),
                                         static_cast<unsigned char>(color.g),
                                         static_cast<unsigned char>(color.b)};
      std::cout.write(reinterpret_cast<const char *>(rgb_data), 3);
      break;
    }
  }

  void print_content() const noexcept {
    for (auto y : std::views::iota(0ull, size.y)) {
      for (auto x : std::views::iota(0ull, size.x)) {
        print_pixel(get_pixel(x, y));
      }
      if (outputType == OutputType::Text)
        std::print("\n");
    }
    if (outputType == OutputType::Text)
      std::print("\n");
  }
};

} // namespace mrender
