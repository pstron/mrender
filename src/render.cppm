export module render;

import std;
import color;
import vector;
import ray;
import camera;
import scene;
import material;
import object;
import output;

export namespace mrender {
namespace vs = std::views;

void renderHSV(Output &out, double v) {
  const Size size = out.get_size();

  const double invW = 1.0 / static_cast<double>(size.x - 1);
  const double invH = 1.0 / static_cast<double>(size.y - 1);

  for (auto y : vs::iota(0ull, size.y)) {
    const double s = 1.0 - static_cast<double>(y) * invH;

    for (auto x : vs::iota(0ull, size.x)) {
      const double h = static_cast<double>(x) * invW * 360.0;
      out.set_pixel(x, y, hsvToRgb(h, s, v));
    }
  }
}

void renderTest(Output &out) {
  const Size size = out.get_size();
  for (auto y : vs::iota(0ull, size.y)) {
    for (auto x : vs::iota(0ull, size.x)) {
      out.set_pixel(x, y,
                    {(colorValue)255, (colorValue)(255 * x / size.x),
                     (colorValue)(255 * y / size.y)});
    }
  }
}

void renderScene(Output &out, scene &scene, vec3 &&lookfrom = vec3{0, 0, 0},
                 vec3 &&lookat = vec3{0, 0, -1}, vec3 &&up = vec3{0, 1, 0},
                 double &&fov = 90.0) {
  const Size size = out.get_size();

  const double inv_w = 1.0 / static_cast<double>(size.x - 1);
  const double inv_h = 1.0 / static_cast<double>(size.y - 1);

  camera cam(lookfrom, lookat, up, fov, static_cast<double>(size.x) / size.y);

  const vec3 light_dir =
      vec3{-1, -1, -1}.normalized(); // Just a simple directional light now

  for (auto y : vs::iota(0ull, size.y)) {
    const double v = 1.0 - static_cast<double>(y) * inv_h;

    for (auto x : vs::iota(0ull, size.x)) {
      const double u = static_cast<double>(x) * inv_w;

      ray ray = cam.get_ray(u, v);

      hit_record rec;
      const object *hit_obj = nullptr;

      rgb pixel;

      if (scene.hit(ray, 0.001, 1e30, rec, hit_obj)) {
        vec3 c = hit_obj->mat().shade(rec, -1 * light_dir);

        c.x = std::clamp(c.x, 0.0, 1.0);
        c.y = std::clamp(c.y, 0.0, 1.0);
        c.z = std::clamp(c.z, 0.0, 1.0);

        pixel = {static_cast<colorValue>(255 * c.x),
                 static_cast<colorValue>(255 * c.y),
                 static_cast<colorValue>(255 * c.z)};
      } else {
        vec3 unit = ray.direction.normalized();
        double t = 0.5 * (unit.y + 1.0);
        vec3 bg = (1.0 - t) * vec3{1, 1, 1} +
                  t * vec3{0.5, 0.7, 1.0}; // A simple sky bg

        pixel = {static_cast<colorValue>(255 * bg.x),
                 static_cast<colorValue>(255 * bg.y),
                 static_cast<colorValue>(255 * bg.z)};
      }

      out.set_pixel(x, y, pixel);
    }
  }
}

} // namespace mrender
