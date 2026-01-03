module;
#include <unistd.h>
export module sample;

import std;
import mrender;
import term;

export namespace sample {

void sphere_ppm() {
  mrender::Output output({1000, 1000});

  mrender::scene world;
  auto mat = std::make_shared<mrender::pure>(mrender::vec3{1, 1, 1});
  world.add(
      std::make_unique<mrender::sphere>(mrender::vec3{0, 0, -1}, 0.5, mat));
  mrender::renderScene(output, world);
  output.display();
}

void test_ppm() {
  mrender::Output output({1000, 1000});
  mrender::renderTest(output);
  output.display();
}

void hsv_ppm() {
  mrender::Output output({1000, 1000});
  mrender::renderHSV(output, 1);
  output.display();
}

void hsv_framebuffer() {
  mrender::Output output({1900, 1040}, mrender::OutputType::FrameBuffer);
  unsigned int t = 0;
  while (true) {
    if (++t == 200)
      t = 1;
    double v = t < 100 ? (t + 1) / 100.0 : (200 - t) / 100.0;
    mrender::renderHSV(output, v);
    output.display();
    usleep(16666);
  }
}

void sphere_spin_framebuffer() {
  mrender::Output output({1920, 1080}, mrender::OutputType::FrameBuffer);

  mrender::scene world;
  auto mat = std::make_shared<mrender::pure>(mrender::vec3{1, 1, 1});
  world.add(
      std::make_unique<mrender::sphere>(mrender::vec3{0, 0, 0}, 0.5, mat));
  unsigned int t = 0;
  term::term tm;
  while (!tm.has_key()) {
    if (++t == 90)
      t = 0;
    double theta = std::numbers::pi * t * 4 / 180.0;
    mrender::vec3 r(std::cos(theta), 0, std::sin(theta));
    mrender::renderScene(output, world, 1 * r, -1 * r);
    output.display();
    usleep(16666);
  }
}

void sphere_framebuffer() {
  mrender::Output output({1920, 1080}, mrender::OutputType::FrameBuffer);

  mrender::scene world;
  auto red = std::make_shared<mrender::pure>(mrender::vec3{1, 0, 0});
  auto green = std::make_shared<mrender::pure>(mrender::vec3{0, 1, 0});
  world.add(
      std::make_unique<mrender::sphere>(mrender::vec3{-1, 0, -1.5}, 0.5, red));
  world.add(
      std::make_unique<mrender::sphere>(mrender::vec3{1, 0, -1.5}, 0.5, green));
  mrender::renderScene(output, world, mrender::vec3{0, 0, 0}, mrender::vec3{0, 0, -1}, mrender::vec3{0, 1, 0}, 90);
  output.display();
  term::term tm;
  while (!tm.has_key());
}

} // namespace sample
