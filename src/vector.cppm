export module vector;

import std;

export namespace mrender {

class vec3 {
public:
  double x, y, z;
  constexpr vec3(double x = 0.0, double y = 0.0, double z = 0.0) noexcept
      : x(x), y(y), z(z) {}
  [[nodiscard]] constexpr vec3 operator+(const vec3 &other) const noexcept {
    return {x + other.x, y + other.y, z + other.z};
  }

  [[nodiscard]] constexpr vec3 operator-(const vec3 &other) const noexcept {
    return {x - other.x, y - other.y, z - other.z};
  }

  [[nodiscard]] constexpr vec3 operator*(double scalar) const noexcept {
    return {x * scalar, y * scalar, z * scalar};
  }

  [[nodiscard]] friend constexpr vec3 operator*(double scalar,
                                                const vec3 &vec) noexcept {
    return vec * scalar;
  }

  [[nodiscard]] constexpr vec3 operator/(double scalar) const {
    return {x / scalar, y / scalar, z / scalar};
  }

  [[nodiscard]] constexpr double dot(const vec3 &other) const noexcept {
    return x * other.x + y * other.y + z * other.z;
  }

  [[nodiscard]] constexpr vec3 cross(const vec3 &other) const noexcept {
    return {y * other.z - z * other.y, z * other.x - x * other.z,
            x * other.y - y * other.x};
  }

  [[nodiscard]] vec3 normalized() const {
    double len = length();
    if (len == 0.0) {
      return *this;
    }
    return *this / len;
  }

  [[nodiscard]] double length() const {
    return std::sqrt(x * x + y * y + z * z);
  }

  [[nodiscard]] constexpr double length_squared() const noexcept {
    return x * x + y * y + z * z;
  }

  [[nodiscard]] auto operator<=>(const vec3 &other) const = default;

  [[nodiscard]] static constexpr vec3 zero() noexcept {
    return {0.0, 0.0, 0.0};
  }
  [[nodiscard]] static constexpr vec3 unit_x() noexcept {
    return {1.0, 0.0, 0.0};
  }
  [[nodiscard]] static constexpr vec3 unit_y() noexcept {
    return {0.0, 1.0, 0.0};
  }
  [[nodiscard]] static constexpr vec3 unit_z() noexcept {
    return {0.0, 0.0, 1.0};
  }

  [[nodiscard]] friend constexpr double dot(const vec3 &a,
                                            const vec3 &b) noexcept {
    return a.x * b.x + a.y * b.y + a.z * b.z;
  }

  [[nodiscard]] friend constexpr vec3 cross(const vec3 &a,
                                            const vec3 &b) noexcept {
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x};
  }

  [[nodiscard]] friend constexpr vec3 normalize(const vec3 &v) noexcept {
    return v.normalized();
  }

  vec3(const vec3 &) = default;
  vec3(vec3 &&) noexcept = default;
  vec3 &operator=(const vec3 &) = default;
  vec3 &operator=(vec3 &&) noexcept = default;
  ~vec3() = default;
};

} // namespace mrender
