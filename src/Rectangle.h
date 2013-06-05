#ifndef Rectangle_H
#define Rectangle_H

class Rectangle;

typedef IRefPtr<Rectangle> RectangleP;

class Rectangle : public PObject {
 public:
  Rectangle(int x_, int y_, int w, int h) :
   x(x_), y(y_), width(w), height(h) {
  }

  void translate(int dx, int dy) {
    x += dx;
    y += dy;
  }

  bool intersects(RectangleP r) {
    if (r->x < x || r->x + r->width  > x + width ||
        r->y < y || r->y + r->height > y + height)
      return false;

    return true;
  }

  StringP toString() {
    return new String("");
  }

 public:
  int x, y, width, height;
};

#endif
