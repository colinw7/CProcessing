#ifndef Polygon_H
#define Polygon_H

#include <Rectangle.h>

class Polygon : public PObject {
 public:
  ARRAY1D(int, IntArray)

  Polygon() {
    npoints = 0;
  }

  Polygon(IntArrayP x, IntArrayP y, int n) {
    npoints = n;

    xpoints.resize(n);
    ypoints.resize(n);

    for (int i = 0; i < n; ++i) {
      xpoints[i] = x[i];
      ypoints[i] = y[i];
    }
  }

  void addPoint(int x, int y) {
    xpoints.push_back(x);
    ypoints.push_back(y);

    ++npoints;
  }

  RectangleP getBounds() {
    assert(npoints);

    int x1 = xpoints[0]; int x2 = x1;
    int y1 = ypoints[0]; int y2 = y1;

    for (int i = 0; i < npoints; ++i) {
      if (xpoints[i] < x1) x1 = xpoints[i];
      if (ypoints[i] < y1) y1 = ypoints[i];
      if (xpoints[i] > x2) x2 = xpoints[i];
      if (ypoints[i] > y2) y2 = ypoints[i];
    }

    return new Rectangle(x1, y1, x2, y2);
  }

 public:
  int              npoints;
  std::vector<int> xpoints;
  std::vector<int> ypoints;
};

typedef IRefPtr<Polygon> PolygonP;

#endif
