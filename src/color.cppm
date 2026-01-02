module;
#include <cmath>
export module color;

import std;

export namespace mrender {

typedef unsigned char colorValue;

struct rgb {
  colorValue r;
  colorValue g;
  colorValue b;
};

rgb hsvToRgb(double h, double s, double v) {
  h = std::fmod(h, 360.0);
  if (h < 0.0)
    h += 360.0;

  s = std::clamp(s, 0.0, 1.0);
  v = std::clamp(v, 0.0, 1.0);

  const double c = v * s;
  const double h6 = h / 60.0;
  const double x = c * (1.0 - std::abs(std::fmod(h6, 2.0) - 1.0));
  const double m = v - c;

  double r1, g1, b1;

  if (h6 < 1.0) {
    r1 = c;
    g1 = x;
    b1 = 0;
  } else if (h6 < 2.0) {
    r1 = x;
    g1 = c;
    b1 = 0;
  } else if (h6 < 3.0) {
    r1 = 0;
    g1 = c;
    b1 = x;
  } else if (h6 < 4.0) {
    r1 = 0;
    g1 = x;
    b1 = c;
  } else if (h6 < 5.0) {
    r1 = x;
    g1 = 0;
    b1 = c;
  } else {
    r1 = c;
    g1 = 0;
    b1 = x;
  }

  return {static_cast<colorValue>(std::lround((r1 + m) * 255.0)),
          static_cast<colorValue>(std::lround((g1 + m) * 255.0)),
          static_cast<colorValue>(std::lround((b1 + m) * 255.0))};
}

} // namespace mrender
