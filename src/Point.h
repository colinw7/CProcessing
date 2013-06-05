#ifndef Point_H
#define Point_H

class Point;

typedef IRefPtr<Point> PointP;

class Point : public PObject {
 public:
  Point(int x_, int y_) :
   x(x_), y(y_) {
  }

  StringP toString() {
    return new String("");
  }

 public:
  int x, y;
};

#endif
