export module ray;

import std;
import vector;

export namespace mrender {

class ray {
public:
  vec3 origin;
  vec3 direction;

  constexpr ray(const vec3 &origin = vec3::zero(),
                const vec3 &direction = vec3::unit_z()) noexcept
      : origin(origin), direction(direction) {}

  [[nodiscard]] constexpr vec3 at(double t) const noexcept {
    return origin + direction * t;
  }

  [[nodiscard]] constexpr ray reversed() const noexcept {
    return ray(origin, direction * -1.0);
  }

  [[nodiscard]] ray normalized() const {
    return ray(origin, direction.normalized());
  }

  [[nodiscard]] auto operator<=>(const ray &) const = default;

  [[nodiscard]] static constexpr ray zero() noexcept {
    return ray(vec3::zero(), vec3::unit_z());
  }

  ray() = delete;
  ray(const ray &) = default;
  ray(ray &&) noexcept = default;
  ray &operator=(const ray &) = default;
  ray &operator=(ray &&) noexcept = default;
  ~ray() = default;
};

} // namespace mrender
