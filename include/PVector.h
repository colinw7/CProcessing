#ifndef PVector_H
#define PVector_H

#include <PObject.h>

class PVector;

typedef IRefPtr<PVector> PVectorP;

class PVector : public PObject {
 public:
  double x, y, z;

  PVector() : x(0.0), y(0.0), z(0.0) { }

  PVector(double x1, double y1) : x(x1), y(y1), z(0.0) { }
  PVector(double x1, double y1, double z1) : x(x1), y(y1), z(z1) { }

  PVectorP get() const { return new PVector(x, y, z); }

  void set(PVectorP v) { x = v->x; y = v->y; z = v->z; }

  void set(double x1, double y1, double z1) { x = x1; y = y1; z = z1; }

  PVectorP add(PVectorP v) {
    x += v->x;
    y += v->y;
    z += v->z;

    return this;
  }

  PVectorP add(double x1, double y1, double z1) {
    x += x1;
    y += y1;
    z += z1;

    return this;
  }

  PVectorP sub(PVectorP v) {
    x -= v->x;
    y -= v->y;
    z -= v->z;

    return this;
  }

  static PVectorP sub(PVectorP lhs, PVectorP rhs) {
    PVectorP t = lhs;

    return t->sub(rhs);
  }

  PVectorP mult(double s) {
    x *= s;
    y *= s;
    z *= s;

    return this;
  }

  PVectorP mult(PVectorP v) {
    x *= v->x;
    y *= v->y;
    z *= v->z;

    return this;
  }

  PVectorP div(double s) {
    x /= s;
    y /= s;
    z /= s;

    return this;
  }

  double dot(PVectorP v) const {
    return (x*v->x + y*v->y + z*v->z);
  }

  PVectorP cross(PVectorP v) const {
    return new PVector(y*v->z - z*v->y, z*v->x - x*v->z, x*v->y - y*v->x);
  }

  double mag() const {
    return sqrt(x*x + y*y + z*z);
  }

  PVectorP limit(double mlimit) {
    double m = mag();

    if (m > mlimit)
      return mult(mlimit/m);
    else
      return this;
  }

  PVectorP normalize() {
    double m = mag();

    return div(m);
  }

  double dist(PVectorP rhs) const {
    double dx = fabs(x - rhs->x);
    double dy = fabs(y - rhs->y);
    double dz = fabs(z - rhs->z);

    return sqrt(dx*dx + dy*dy + dz*dz);
  }

  static double dist(PVectorP lhs, PVectorP rhs) {
    return lhs->dist(rhs);
  }

  double heading2D() const {
    double angle = atan2(-y, x);

    return -angle;
  }
};

#endif
