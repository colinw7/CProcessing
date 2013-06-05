#ifndef CQImageFilter_H
#define CQImageFilter_H

#include <QImage>
#include <CRGBA.h>

namespace CQImageFilter {
  QImage unsharpMask(const QImage &src, double strength);

  QImage convolve(const QImage &src, const char *kernel, int size, int divisor);

  QImage gaussianBlur(const QImage &src, double bx, double by, int nx, int ny);

  QImage turbulence(const QImage &src, bool fractal, double baseFreq,
                    int numOctaves, int seed);

  QImage erode(const QImage &src);
  QImage dilate(const QImage &src);

  QImage mask(const QImage &src, const QImage &imask);

  QImage tint(const QImage &src, const CRGBA &rgba);

  void getPixel(const QImage &image, int x, int y, CRGBA &rgba);

  void setPixel(QImage &image, int x, int y, const CRGBA &rgba);
};

#endif
