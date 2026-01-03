module;
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
export module framebuffer;

import std;
import color;

export namespace mrender {

class FrameBuffer {
public:
  FrameBuffer() {
    write_sysfs("/sys/class/vtconsole/vtcon1/bind", "0\n");
    fb_fd = ::open("/dev/fb0", O_RDWR);
    if (fb_fd < 0)
      throw std::runtime_error("failed to open /dev/fb0");

    if (::ioctl(fb_fd, FBIOGET_FSCREENINFO, &finfo) < 0)
      throw std::runtime_error("FBIOGET_FSCREENINFO failed");

    if (::ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo) < 0)
      throw std::runtime_error("FBIOGET_VSCREENINFO failed");

    fb_size = finfo.smem_len;
    fb_mem = static_cast<std::uint8_t *>(
        ::mmap(nullptr, fb_size, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0));
    ::memset(fb_mem, 0, fb_size);

    if (fb_mem == MAP_FAILED)
      throw std::runtime_error("mmap framebuffer failed");
  }

  ~FrameBuffer() {
    if (fb_mem && fb_mem != MAP_FAILED) {
      ::memset(fb_mem, 0, fb_size);
      ::munmap(fb_mem, fb_size);
    }
    if (fb_fd >= 0)
      ::close(fb_fd);
    write_sysfs("/sys/class/vtconsole/vtcon1/bind", "1\n");
  }

  FrameBuffer(const FrameBuffer &) = delete;
  FrameBuffer &operator=(const FrameBuffer &) = delete;

  std::size_t width() const noexcept { return vinfo.xres; }
  std::size_t height() const noexcept { return vinfo.yres; }
  std::size_t bpp() const noexcept { return vinfo.bits_per_pixel; }

  void clear() noexcept { std::memset(fb_mem, 0, fb_size); }

  void set_pixel(std::size_t x, std::size_t y, rgb c) noexcept {
    if (x >= vinfo.xres || y >= vinfo.yres)
      return;

    const std::size_t bytes = vinfo.bits_per_pixel / 8;
    const std::size_t offset = y * finfo.line_length + x * bytes;

    fb_mem[offset + 0] = static_cast<std::uint8_t>(c.b);
    fb_mem[offset + 1] = static_cast<std::uint8_t>(c.g);
    fb_mem[offset + 2] = static_cast<std::uint8_t>(c.r);

    if (bytes == 4)
      fb_mem[offset + 3] = 0xFF;
  }

  void blit(const std::vector<rgb> &pixels, std::size_t img_w,
            std::size_t img_h) noexcept {
    if (pixels.empty())
      return;

    const std::size_t fb_w = width();
    const std::size_t fb_h = height();

    const std::size_t x_off = (img_w < fb_w) ? (fb_w - img_w) / 2 : 0;
    const std::size_t y_off = (img_h < fb_h) ? (fb_h - img_h) / 2 : 0;

    const std::size_t draw_w = std::min(img_w, fb_w);
    const std::size_t draw_h = std::min(img_h, fb_h);

    for (std::size_t y = 0; y < draw_h; ++y) {
      for (std::size_t x = 0; x < draw_w; ++x) {
        set_pixel(x + x_off, y + y_off, pixels[y * img_w + x]);
      }
    }
    vinfo.xoffset = 0;
    vinfo.yoffset = 0;
    ioctl(fb_fd, FBIOPAN_DISPLAY, &vinfo);
  }

private:
  int fb_fd{-1};
  fb_fix_screeninfo finfo{};
  fb_var_screeninfo vinfo{};
  std::size_t fb_size{0};
  std::uint8_t *fb_mem{nullptr};
  static void write_sysfs(const char *path, const char *value) noexcept {
    int fd = ::open(path, O_WRONLY);
    if (fd < 0)
      return;
    ::write(fd, value, std::strlen(value));
    ::close(fd);
  }
};

} // namespace mrender
