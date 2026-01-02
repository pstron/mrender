module;
#include <cmath>
export module camera;

import std;
import vector;
import ray;

export namespace mrender {

class camera {
public:
  camera(const vec3 &lookfrom, const vec3 &lookat, const vec3 &vup,
         double vfov_deg, double aspect_ratio) noexcept {
    const double theta = vfov_deg * std::numbers::pi / 180.0;
    const double h = std::tan(theta * 0.5);
    const double viewport_height = 2.0 * h;
    const double viewport_width = aspect_ratio * viewport_height;

    w = (lookfrom - lookat).normalized();
    u = cross(vup, w).normalized();
    v = cross(w, u).normalized();

    origin = lookfrom;
    horizontal = viewport_width * u;
    vertical = viewport_height * v;
    lower_left_corner = origin - horizontal * 0.5 - vertical * 0.5 - w;
  }

  [[nodiscard]] ray get_ray(double s, double t) const noexcept {
    return ray{origin,
               (lower_left_corner + s * horizontal + t * vertical - origin)
                   .normalized()};
  }

  camera() = delete;

private:
  vec3 origin;
  vec3 lower_left_corner;
  vec3 horizontal;
  vec3 vertical;
  vec3 u, v, w;
};

} // namespace mrender
