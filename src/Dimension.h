#ifndef Dimension_H
#define Dimension_H

class Dimension;

typedef IRefPtr<Dimension> DimensionP;

class Dimension : public PObject {
 public:
  Dimension(int w, int h) :
   width(w), height(h) {
  }

  StringP toString() {
    return new String("");
  }

 public:
  int width, height;
};

#endif
