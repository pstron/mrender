export module object;

import std;
import vector;
import ray;
import material;

export namespace mrender {

class object {
public:
  object() = default;
  virtual ~object() = default;

  object(const object &) = delete;
  object &operator=(const object &) = delete;
  object(object &&) = default;
  object &operator=(object &&) = default;

  virtual bool hit(const ray &ray, double t_min, double t_max,
                   hit_record &rec) const = 0;

  const material &mat() const noexcept { return *mater; }

protected:
  explicit object(std::shared_ptr<material> mat) : mater(std::move(mat)) {}

private:
  std::shared_ptr<material> mater;
};

class sphere final : public object {
public:
  sphere(vec3 center, double radius, std::shared_ptr<material> mat)
      : object(std::move(mat)), center(center), radius(radius) {}

  bool hit(const ray &ray, double t_min, double t_max,
           hit_record &rec) const override {
    vec3 oc = ray.origin - center;
    double a = ray.direction.dot(ray.direction);
    double b = 2.0 * (oc.dot(ray.direction));
    double c = oc.dot(oc) - radius * radius;
    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
      return false;

    double sqrtd = std::sqrt(discriminant);
    double root = (-b - sqrtd) / (2 * a);

    if (root < t_min || root > t_max) {
      root = (-b + sqrtd) / (2 * a);
      if (root < t_min || root > t_max)
        return false;
    }

    rec.t = root;
    rec.position = ray.origin + ray.direction * root;
    rec.normal = (rec.position - center).normalized();
    return true;
  }

private:
  vec3 center;
  double radius;
};

} // namespace mrender
