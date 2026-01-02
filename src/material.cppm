export module material;

import std;
import vector;

export namespace mrender {
struct hit_record {
  double t;
  vec3 position;
  vec3 normal;
};

class material {
public:
  virtual ~material() = default;
  virtual vec3 shade(const hit_record &hit, const vec3 &light_dir) const = 0;
};

class pure final : public material {
public:
  explicit pure(vec3 albedo) : albedo(albedo) {}

  vec3 shade(const hit_record &hit, const vec3 &light_dir) const override {
    const double ndotl = std::max(0.0, hit.normal.dot(light_dir));
    return albedo * ndotl;
  }

private:
  vec3 albedo;
};

} // namespace mrender
