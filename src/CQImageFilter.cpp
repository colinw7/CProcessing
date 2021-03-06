#include <CQImageFilter.h>
#include <CMathGen.h>

QImage
CQImageFilter::
unsharpMask(const QImage &src, double strength)
{
  static char kernel[] = { 0,  0,  1,  0, 0,
                           0,  8, 21,  8, 0,
                           1, 21, 59, 21, 1,
                           0,  8, 21,  8, 0,
                           0,  0,  1,  0, 0};

  QImage dst = convolve(src, kernel, 5, 179);

  double gstrength = 1.0 - strength;

  int   x, y;
  CRGBA rgba, rgba1, rgba2;

  int wx1 = 0;
  int wy1 = 0;
  int wx2 = src.width () - 1;
  int wy2 = src.height() - 1;

  for (y = wy1; y <= wy2; ++y) {
    for (x = wx1; x <= wx2; ++x) {
      getPixel(src, x, y, rgba1);
      getPixel(dst, x, y, rgba2);

      rgba = rgba1*strength + rgba2*gstrength;

      rgba.clamp();

      setPixel(dst, x, y, rgba);
    }
  }

  return dst;
}

QImage
CQImageFilter::
convolve(const QImage &src, const char *kernel, int size, int divisor)
{
  QImage dst(src);

  int border = (size - 1)/2;

  const char *k;
  CRGBA       sum, rgba;
  int         x, y, xk, yk;

  int wx1 = 0;
  int wy1 = 0;
  int wx2 = src.width () - 1;
  int wy2 = src.height() - 1;

  y = wy1;

  for ( ; y < border; ++y) {
    x = wx1;

    for ( ; x <= wx2 - border; ++x) {
      getPixel(src, x, y, rgba);

      setPixel(dst, x, y, rgba);
    }
  }

  for ( ; y <= wy2 - border; ++y) {
    x = wx1;

    for ( ; x < border; ++x) {
      getPixel(src, x, y, rgba);

      setPixel(dst, x, y, rgba);
    }

    for ( ; x <= wx2 - border; ++x) {
      sum.zero();

      k = kernel;

      for (yk = -border; yk <= border; ++yk) {
        for (xk = -border; xk < border; ++xk) {
          getPixel(src, x + xk, y + yk, rgba);

          sum += rgba*(*k++);
        }
      }

      if (divisor != 1)
        sum /= divisor;

      sum.clamp();

      setPixel(dst, x, y, sum);
    }

    for ( ; x <= wx2; ++x) {
      getPixel(src, x, y, rgba);

      setPixel(dst, x, y, rgba);
    }
  }

  for ( ; y <= wy2; ++y) {
    x = wx1;

    for ( ; x <= wx2; ++x) {
      getPixel(src, x, y, rgba);

      setPixel(dst, x, y, rgba);
    }
  }

  return dst;
}

QImage
CQImageFilter::
gaussianBlur(const QImage &src, double bx, double by, int nx, int ny)
{
  // Set Kernel Matrix
  // TODO: cache for same parameters
  double minb = std::min(bx, by);

  if (minb <= 0)
    return src;

  if (nx == 0)
    nx = int(6*bx + 1);

  if (ny == 0)
    ny = int(6*by + 1);

  int nx1 = -nx/2;
  int nx2 =  nx/2;
  int ny1 = -ny/2;
  int ny2 =  ny/2;

  nx = nx2 - nx1 + 1;
  ny = ny2 - ny1 + 1;

  double **m = new double * [nx];

  for (int i = 0; i < nx; ++i)
    m[i] = new double [ny];

  double bxy  = bx*by;
  double bxy1 = 2*bxy;
  double bxy2 = 1.0/(M_PI*bxy1);

  int i2, j2;

  for (int i = 0, i1 = nx1; i < nx; ++i, ++i1) {
    i2 = i1*i1;

    for (int j = 0, j1 = ny1; j < ny; ++j, ++j1) {
      j2 = j1*j1;

      m[i][j] = bxy2*exp(-(i2 + j2)/bxy1);
    }
  }

  //--------

  QImage dst(src);

  CRGBA rgba, rgba1;

  int wx1 = 0;
  int wy1 = 0;
  int wx2 = src.width () - 1;
  int wy2 = src.height() - 1;

  for (int y1 = ny1, y2 = wy1, y3 = ny2; y2 <= wy2; ++y1, ++y2, ++y3) {
    for (int x1 = nx1, x2 = wx1, x3 = nx2; x2 <= wx2; ++x1, ++x2, ++x3) {
      rgba.zero();

      for (int i = 0, x = x1; i < nx; ++i, ++x)
        for (int j = 0, y = y1; j < ny; ++j, ++y) {
          if (x < wx1 || x > wx2 || y < wy1 || y > wy2) continue;

          getPixel(src, x, y, rgba1);

          rgba += rgba1*m[i][j];
        }

      rgba.clamp();

      setPixel(dst, x2, y2, rgba);
    }
  }

  return dst;
}

class CTurbulenceUtil {
 private:
  static const int RAND_m = 2147483647; /* 2**31 - 1 */
  static const int RAND_a = 16807; /* 7**5; primitive root of m */
  static const int RAND_q = 127773; /* m / a */
  static const int RAND_r = 2836; /* m % a */

  static const int BSize   = 0x100;
  static const int BM      = 0xff;
  static const int PerlinN = 0x1000;
  static const int NP      = 12; /* 2^PerlinN */
  static const int NM      = 0xfff;

  //------

  int    latticeSelector[BSize + BSize + 2];
  double gradient[4][BSize + BSize + 2][2];

  //------

 public:
  CTurbulenceUtil(int seed) {
    init(seed);
  }

  /* Produces results in the range [1, 2**31 - 2].

     Algorithm is: r = (a * r) mod m
     where a = 16807 and m = 2**31 - 1 = 2147483647

     See [Park & Miller], CACM vol. 31 no. 10 p. 1195, Oct. 1988

     To test: the algorithm should produce the result 1043618065
     as the 10,000th generated number if the original seed is 1.
  */

 private:
  int initSeed(int lSeed) {
    if (lSeed <= 0)
      lSeed = -(lSeed % (RAND_m - 1)) + 1;

    if (lSeed > RAND_m - 1)
      lSeed = RAND_m - 1;

    return lSeed;
  }

  int random(int lSeed) {
    int result = RAND_a*(lSeed % RAND_q) - RAND_r*(lSeed / RAND_q);

    if (result <= 0)
      result += RAND_m;

    return result;
  }

  void init(int lSeed) {
    double s;

    lSeed = initSeed(lSeed);

    for (int k = 0; k < 4; k++) {
      for (int i = 0; i < BSize; i++) {
        latticeSelector[i] = i;

        for (int j = 0; j < 2; j++) {
          lSeed = random(lSeed);

          gradient[k][i][j] =
            double((lSeed) % (BSize + BSize) - BSize)/BSize;
        }

        s = sqrt(gradient[k][i][0]*gradient[k][i][0] +
                 gradient[k][i][1]*gradient[k][i][1]);

        gradient[k][i][0] /= s;
        gradient[k][i][1] /= s;
      }
    }

    int i = BSize;

    while (--i) {
      int k = latticeSelector[i];

      lSeed = random(lSeed);

      int j = lSeed % BSize;

      latticeSelector[i] = latticeSelector[j];
      latticeSelector[j] = k;
    }

    for (int i = 0; i < BSize + 2; i++) {
      latticeSelector[BSize + i] = latticeSelector[i];

      for (int k = 0; k < 4; k++)
        for (int j = 0; j < 2; j++)
          gradient[k][BSize + i][j] = gradient[k][i][j];
    }
  }

  double s_curve(double t) {
    return (t*t*(3.0 - 2.0*t));
  }

  double lerp(double t, double a, double b) {
    return (a + t*(b - a));
  }

  double noise2(int channelNum, double vec[2]) {
    double t = vec[0] + PerlinN;

    int bx0 = int(t)    & BM;
    int bx1 = (bx0 + 1) & BM;

    double rx0 = t - int(t);
    double rx1 = rx0 - 1.0;

    t = vec[1] + PerlinN;

    int by0 = int(t)    & BM;
    int by1 = (by0 + 1) & BM;

    double ry0 = t - int(t);
    double ry1 = ry0 - 1.0;

    int i = latticeSelector[bx0];
    int j = latticeSelector[bx1];

    int b00 = latticeSelector[i + by0];
    int b10 = latticeSelector[j + by0];
    int b01 = latticeSelector[i + by1];
    int b11 = latticeSelector[j + by1];

    double sx = s_curve(rx0);
    double sy = s_curve(ry0);

    double u, v, *q;

    q = gradient[channelNum][b00]; u = rx0*q[0] + ry0*q[1];
    q = gradient[channelNum][b10]; v = rx1*q[0] + ry0*q[1];

    double a = lerp(sx, u, v);

    q = gradient[channelNum][b01]; u = rx0*q[0] + ry1*q[1];
    q = gradient[channelNum][b11]; v = rx1*q[0] + ry1*q[1];

    double b = lerp(sx, u, v);

    return lerp(sy, a, b);
  }

  // Returns 'turbFunctionResult'

 public:
  double turbulence(int channelNum, double *point, double baseFreq,
                    int numOctaves, bool fractal) {
    double vec[2];

    double sum       = 0;
    double frequency = baseFreq;

    for (int nOctave = 0; nOctave < numOctaves; nOctave++) {
      vec[0] = frequency*point[0];
      vec[1] = frequency*point[1];

      double amplitude = baseFreq/frequency;

      double sum1 = noise2(channelNum, vec)*amplitude;

      if (fractal)
        sum += sum1;
      else
        sum += fabs(sum1);

      frequency *= 2;
    }

    return sum;
  }
};

QImage
CQImageFilter::
turbulence(const QImage &src, bool fractal, double baseFreq,
           int numOctaves, int seed)
{
  QImage dst(src);

  CTurbulenceUtil turbulence(seed);

  double r, g, b, a, point[2];

  int wx1 = 0;
  int wy1 = 0;
  int wx2 = src.width () - 1;
  int wy2 = src.height() - 1;

  for (int y = wy1; y <= wy2; ++y) {
    for (int x = wx1; x <= wx2; ++x) {
      point[0] = x;
      point[1] = y;

      r = turbulence.turbulence(0, point, baseFreq, numOctaves, fractal);
      g = turbulence.turbulence(1, point, baseFreq, numOctaves, fractal);
      b = turbulence.turbulence(2, point, baseFreq, numOctaves, fractal);
      a = turbulence.turbulence(3, point, baseFreq, numOctaves, fractal);

      if (fractal) {
        r = (r + 1.0) / 2.0;
        g = (g + 1.0) / 2.0;
        b = (b + 1.0) / 2.0;
        a = (a + 1.0) / 2.0;
      }

      r = std::min(std::max(r, 0.0), 1.0);
      g = std::min(std::max(g, 0.0), 1.0);
      b = std::min(std::max(b, 0.0), 1.0);
      a = std::min(std::max(a, 0.0), 1.0);

      setPixel(dst, x, y, CRGBA(r, g, b, a));
    }
  }

  return dst;
}

QImage
CQImageFilter::
erode(const QImage &src)
{
  QImage dst(src);

  int wx1 = 0;
  int wy1 = 0;
  int wx2 = src.width () - 1;
  int wy2 = src.height() - 1;

  for (int y1 = wy1, y2 = wy1, y3 = y2 + 1; y2 <= wy2; y1 = y2, y2 = y3++) {
    for (int x1 = wx1, x2 = wx1, x3 = x2 + 1; x2 <= wx2; x1 = x2, x2 = x3++) {
      CRGBA colOrig;

      getPixel(src, x2, y2, colOrig);

      CRGBA colUp, colLeft, colDown, colRight;

      getPixel(src, x1               , y2               , colLeft );
      getPixel(src, std::min(x3, wx2), y2               , colRight);
      getPixel(src, x2               , y1               , colUp   );
      getPixel(src, x2               , std::min(y3, wy2), colDown );

      // compute luminance
      double currLum  = colOrig .getGray();
      double lumLeft  = colLeft .getGray();
      double lumRight = colRight.getGray();
      double lumUp    = colUp   .getGray();
      double lumDown  = colDown .getGray();

      CRGBA colOut = colOrig;

      if (lumLeft < currLum) {
        colOut  = colLeft;
        currLum = lumLeft;
      }
      if (lumRight < currLum) {
        colOut  = colRight;
        currLum = lumRight;
      }
      if (lumUp < currLum) {
        colOut  = colUp;
        currLum = lumUp;
      }
      if (lumDown < currLum) {
        colOut  = colDown;
        currLum = lumDown;
      }

      setPixel(dst, x2, y2, colOut);
    }
  }

  return dst;
}

QImage
CQImageFilter::
dilate(const QImage &src)
{
  QImage dst(src);

  int wx1 = 0;
  int wy1 = 0;
  int wx2 = src.width () - 1;
  int wy2 = src.height() - 1;

  for (int y1 = wy1, y2 = wy1, y3 = y2 + 1; y2 <= wy2; y1 = y2, y2 = y3++) {
    for (int x1 = wx1, x2 = wx1, x3 = x2 + 1; x2 <= wx2; x1 = x2, x2 = x3++) {
      CRGBA colOrig;

      getPixel(src, x2, y2, colOrig);

      CRGBA colUp, colLeft, colDown, colRight;

      getPixel(src, x1               , y2               , colLeft );
      getPixel(src, std::min(x3, wx2), y2               , colRight);
      getPixel(src, x2               , y1               , colUp   );
      getPixel(src, x2               , std::min(y3, wy2), colDown );

      // compute luminance
      double currLum  = colOrig .getGray();
      double lumLeft  = colLeft .getGray();
      double lumRight = colRight.getGray();
      double lumUp    = colUp   .getGray();
      double lumDown  = colDown .getGray();

      CRGBA colOut = colOrig;

      if (lumLeft > currLum) {
        colOut  = colLeft;
        currLum = lumLeft;
      }
      if (lumRight > currLum) {
        colOut  = colRight;
        currLum = lumRight;
      }
      if (lumUp > currLum) {
        colOut  = colUp;
        currLum = lumUp;
      }
      if (lumDown > currLum) {
        colOut  = colDown;
        currLum = lumDown;
      }

      setPixel(dst, x2, y2, colOut);
    }
  }

  return dst;
}

QImage
CQImageFilter::
mask(const QImage &src, const QImage &imask)
{
  QImage dst = QImage(src.width(), src.height(), QImage::Format_ARGB32);

  int wx1 = 0;
  int wy1 = 0;
  int wx2 = src.width () - 1;
  int wy2 = src.height() - 1;

  CRGBA rgba1, rgba2;

  for (int y = wy1; y <= wy2; ++y) {
    for (int x = wx1; x <= wx2; ++x) {
      getPixel(src  , x, y, rgba1);
      getPixel(imask, x, y, rgba2);

      rgba1.setAlpha(rgba2.getBlue());

      setPixel(dst, x, y, rgba1);
    }
  }

  return dst;
}

QImage
CQImageFilter::
tint(const QImage &src, const CRGBA &rgba)
{
  QImage dst = QImage(src.width(), src.height(), QImage::Format_ARGB32);

  int wx1 = 0;
  int wy1 = 0;
  int wx2 = src.width () - 1;
  int wy2 = src.height() - 1;

  CRGBA rgba1, rgba2;

  for (int y = wy1; y <= wy2; ++y) {
    for (int x = wx1; x <= wx2; ++x) {
      getPixel(src, x, y, rgba1);

      rgba2 = rgba1 * rgba;

      rgba2.clamp();

      setPixel(dst, x, y, rgba2);
    }
  }

  return dst;
}

void
CQImageFilter::
getPixel(const QImage &image, int x, int y, CRGBA &rgba)
{
  QRgb rgb = image.pixel(x, y);

  rgba = CRGBA(qRed  (rgb)/255.0,
               qGreen(rgb)/255.0,
               qBlue (rgb)/255.0,
               qAlpha(rgb)/255.0);
}

void
CQImageFilter::
setPixel(QImage &image, int x, int y, const CRGBA &rgba)
{
  uint r, g, b, a;

  rgba.getRGBAI(&r, &g, &b, &a);

  QRgb rgb = qRgba(r, g, b, a);

  image.setPixel(x, y, rgb);
}
