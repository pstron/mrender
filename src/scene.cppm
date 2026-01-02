export module scene;

import std;
import ray;
import object;
import material;

export namespace mrender {

class scene {
public:
  void add(std::unique_ptr<object> obj) { objects.push_back(std::move(obj)); }

  bool hit(const ray &ray, double t_min, double t_max, hit_record &rec,
           const object *&out_object) const {
    hit_record temp;
    bool hit_anything = false;
    double closest = t_max;

    for (const auto &obj : objects) {
      if (obj->hit(ray, t_min, closest, temp)) {
        hit_anything = true;
        closest = temp.t;
        rec = temp;
        out_object = obj.get();
      }
    }
    return hit_anything;
  }

private:
  std::vector<std::unique_ptr<object>> objects;
};

} // namespace mrender
