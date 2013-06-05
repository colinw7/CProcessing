#ifndef CProcessing_H
#define CProcessing_H

#include <cfloat>
#include <CKeyType.h>
#include <CCursor.h>

#include <CProcessingCore.h>

class QWidget;
class QPainter;
class QPainterPath;
class QPixmap;

class PFont;
class PShape;
class PGraphics;
class PImage;

typedef IRefPtr<PFont>     PFontP;
typedef IRefPtr<PShape>    PShapeP;
typedef IRefPtr<PGraphics> PGraphicsP;
typedef IRefPtr<PImage>    PImageP;

#ifdef USE_SVG
class CSVGObject;
#endif

#include <string>
#include <QColor>
#include <QImage>
#include <QFont>
#include <CMatrix2D.h>
#include <CMatrix3D.h>
#include <CRGBA.h>

namespace CProcessing {
  extern const int X;  // model coords xyz (formerly MX/MY/MZ)
  extern const int Y;
  extern const int Z;

  extern const int R;  // actual rgb, after lighting
  extern const int G;  // fill stored here, transform in place
  extern const int B;  // TODO don't do that anymore (?)
  extern const int A;

  extern const int U; // texture
  extern const int V;

  extern const int NX; // normal
  extern const int NY;
  extern const int NZ;

  extern const int EDGE;

  // stroke

  /** stroke argb values */
  extern const int SR;
  extern const int SG;
  extern const int SB;
  extern const int SA;

  /** stroke weight */
  extern const int SW;

  // transformations (2D and 3D)

  extern const int TX; // transformed xyzw
  extern const int TY;
  extern const int TZ;

  extern const int VX; // view space coords
  extern const int VY;
  extern const int VZ;
  extern const int VW;

  // material properties

  // Ambient color (usually to be kept the same as diffuse)
  // fill(_) sets both ambient and diffuse.
  extern const int AR;
  extern const int AG;
  extern const int AB;

  // Diffuse is shared with fill.
  extern const int DR;  // TODO needs to not be shared, this is a material property
  extern const int DG;
  extern const int DB;
  extern const int DA;

  // specular (by default kept white)
  extern const int SPR;
  extern const int SPG;
  extern const int SPB;

  extern const int SHINE;

  // emissive (by default kept black)
  extern const int ER;
  extern const int EG;
  extern const int EB;

  // has this vertex been lit yet
  extern const int BEEN_LIT;

  extern const int VERTEX_FIELD_COUNT;

  // renderers known to processing.core

  extern const char *P2D;
  extern const char *P3D;
  extern const char *JAVA2D;
  extern const char *OPENGL;
  extern const char *PDF;
  extern const char *DXF;

  // platform IDs for PApplet.platform

  extern const int OTHER;
  extern const int WINDOWS;
  extern const int MACOSX;
  extern const int LINUX;

  extern const char *platformNames[];

  extern const float EPSILON;

  // max/min values for numbers

  /**
   * Same as Float.MAX_VALUE, but included for parity with MIN_VALUE,
   * and to avoid teaching static methods on the first day.
   */
  extern const float MAX_FLOAT;
  /**
   * Note that Float.MIN_VALUE is the smallest <EM>positive</EM> value
   * for a floating point number, not actually the minimum (negative) value
   * for a float. This constant equals 0xFF7FFFFF, the smallest (farthest
   * negative) value a float can have before it hits NaN.
   */
  extern const float MIN_FLOAT;
  /** Largest possible (positive) integer value */
  extern const int MAX_INT;
  /** Smallest possible (negative) integer value */
  extern const int MIN_INT;

  // useful goodness

  extern const float PI;
  extern const float HALF_PI;
  extern const float THIRD_PI;
  extern const float QUARTER_PI;
  extern const float TWO_PI;

  extern const float DEG_TO_RAD;
  extern const float RAD_TO_DEG;

  // angle modes

  //extern const int RADIANS;
  //extern const int DEGREES;

  // used by split, all the standard whitespace chars
  // (also includes unicode nbsp, that little bostage)

  extern const char *WHITESPACE;

  // for colors and/or images

  extern const int RGB;  // image & color
  extern const int ARGB;  // image
  extern const int HSB;  // color
  extern const int ALPHA;  // image
  extern const int CMYK;  // image & color (someday)

  // image file types

  extern const int TIFF;
  extern const int TARGA;
  extern const int JPEG;
  extern const int GIF;

  // filter/convert types

  extern const int BLUR;
  extern const int GRAY;
  extern const int INVERT;
  extern const int OPAQUE;
  extern const int POSTERIZE;
  extern const int THRESHOLD;
  extern const int ERODE;
  extern const int DILATE;

  // blend mode keyword definitions
  // @see processing.core.PImage#blendColor(int,int,int)

  extern const int REPLACE;
  extern const int BLEND;
  extern const int ADD;
  extern const int SUBTRACT;
  extern const int LIGHTEST;
  extern const int DARKEST;
  extern const int DIFFERENCE;
  extern const int EXCLUSION;
  extern const int MULTIPLY;
  extern const int SCREEN;
  extern const int OVERLAY;
  extern const int HARD_LIGHT;
  extern const int SOFT_LIGHT;
  extern const int DODGE;
  extern const int BURN;

  // colour component bitmasks

  extern const int ALPHA_MASK;
  extern const int RED_MASK;
  extern const int GREEN_MASK;
  extern const int BLUE_MASK;

  // for messages

  extern const int CHATTER;
  extern const int COMPLAINT;
  extern const int PROBLEM;

  // types of projection matrices

  extern const int CUSTOM; // user-specified fanciness
  extern const int ORTHOGRAPHIC; // 2D isometric projection
  extern const int PERSPECTIVE; // perspective matrix

  // shapes

  // the low four bits set the variety,
  // higher bits set the specific shape type

  //extern const int GROUP;

  extern const int POINT;  // shared with light (!)
  extern const int POINTS;

  extern const int LINE;
  extern const int LINES;
  extern const int LINE_STRIP;

  extern const int TRIANGLE;
  extern const int TRIANGLES;
  extern const int TRIANGLE_STRIP;
  extern const int TRIANGLE_FAN;

  extern const int QUAD;
  extern const int QUADS;
  extern const int QUAD_STRIP;

  extern const int POLYGON;
  extern const int PATH;

  extern const int RECT;
  extern const int ELLIPSE;
  extern const int ARC;

  extern const int SPHERE;
  extern const int BOX;

  // shape closing modes

  extern const int OPEN;
  extern const int CLOSE;

  // shape drawing modes

  /** Draw mode convention to use (x, y) to (width, height) */
  extern const int CORNER;
  /** Draw mode convention to use (x1, y1) to (x2, y2) coordinates */
  extern const int CORNERS;
  /** Draw mode from the center, and using the radius */
  extern const int RADIUS;
  /** @deprecated Use RADIUS instead. */
  extern const int CENTER_RADIUS;
  /**
   * Draw from the center, using second pair of values as the diameter.
   * Formerly called CENTER_DIAMETER in alpha releases.
   */
  extern const int CENTER;
  /**
   * Synonym for the CENTER constant. Draw from the center,
   * using second pair of values as the diameter.
   */
  extern const int DIAMETER;
  /** @deprecated Use DIAMETER instead. */
  extern const int CENTER_DIAMETER;

  // vertically alignment modes for text

  /** Default vertical alignment for text placement */
  extern const int BASELINE;
  /** Align text to the top */
  extern const int TOP;
  /** Align text from the bottom, using the baseline. */
  extern const int BOTTOM;

  // uv texture orientation modes

  /** texture coordinates in 0..1 range */
  extern const int NORMAL;
  /** @deprecated use NORMAL instead */
  extern const int NORMALIZED;
  /** texture coordinates based on image width/height */
  extern const int IMAGE;

  // text placement modes

  /**
   * textMode(MODEL) is the default, meaning that characters
   * will be affected by transformations like any other shapes.
   * <p/>
   * Changed value in 0093 to not interfere with LEFT, CENTER, and RIGHT.
   */
  extern const int MODEL;

  /**
   * textMode(SHAPE) draws text using the the glyph outlines of
   * individual characters rather than as textures. If the outlines are
   * not available, then textMode(SHAPE) will be ignored and textMode(MODEL)
   * will be used instead. For this reason, be sure to call textMode()
   * <EM>after</EM> calling textFont().
   * <p/>
   * Currently, textMode(SHAPE) is only supported by OPENGL mode.
   * It also requires Java 1.2 or higher (OPENGL requires 1.4 anyway)
   */
  extern const int SHAPE;

  // text alignment modes
  // are inherited from LEFT, CENTER, RIGHT

  // stroke modes

  extern const int SQUARE;  // called 'butt' in the svg spec
  extern const int ROUND;
  extern const int PROJECT;  // called 'square' in the svg spec
  extern const int MITER;
  extern const int BEVEL;

  // lighting

  extern const int AMBIENT;
  extern const int DIRECTIONAL;
  //extern const int POINT;  // shared with shape feature
  extern const int SPOT;


  // key constants

  // only including the most-used of these guys
  // if people need more esoteric keys, they can learn about
  // the esoteric java KeyEvent api and of virtual keys

  // both key and keyCode will equal these values
  // for 0125, these were changed to 'char' values, because they
  // can be upgraded to ints automatically by Java, but having them
  // as ints prevented split(blah, TAB) from working
  extern const char BACKSPACE;
  extern const char TAB;
  extern const char ENTER;
  extern const char RETURN;
  extern const char ESC;
  extern const char DELETE;

  // i.e. if ((key == CODED) && (keyCode == UP))
  extern const int CODED;

  // key will be CODED and keyCode will be this value
  extern const int UP;
  extern const int DOWN;
  extern const int LEFT;
  extern const int RIGHT;

  // key will be CODED and keyCode will be this value
  extern const int ALT;
  extern const int CONTROL;
  extern const int SHIFT;

  // cursor types

  extern const int ARROW;
  extern const int CROSS;
  extern const int HAND;
  extern const int MOVE;
  extern const int TEXT;
  extern const int WAIT;

  // hints - hint values are positive for the alternate version,
  // negative of the same value returns to the normal/default state

  extern const int DISABLE_OPENGL_2X_SMOOTH;
  extern const int ENABLE_OPENGL_2X_SMOOTH;
  extern const int ENABLE_OPENGL_4X_SMOOTH;

  extern const int ENABLE_NATIVE_FONTS;

  extern const int DISABLE_DEPTH_TEST;
  extern const int ENABLE_DEPTH_TEST;

  extern const int ENABLE_DEPTH_SORT;
  extern const int DISABLE_DEPTH_SORT;

  extern const int DISABLE_OPENGL_ERROR_REPORT;
  extern const int ENABLE_OPENGL_ERROR_REPORT;

  extern const int ENABLE_ACCURATE_TEXTURES;
  extern const int DISABLE_ACCURATE_TEXTURES;

  extern const int HINT_COUNT;

  // error messages

  extern const char *ERROR_BACKGROUND_IMAGE_SIZE;
  extern const char *ERROR_BACKGROUND_IMAGE_FORMAT;

  extern const char *ERROR_TEXTFONT_NULL_PFONT;

  extern const char *ERROR_PUSHMATRIX_OVERFLOW;
  extern const char *ERROR_PUSHMATRIX_UNDERFLOW;
}

//--------

struct color {
  int    mode;
  double c1, c2, c3;
  double a;

  color();
  color(const color &c);

  explicit color(double gray);
  color(double gray, double a);

  color(double v1, double v2, double v3);
  color(double v1, double v2, double v3, double a);

  explicit color(StringP name);

  color(StringP name, double a);

  color(int mode, double c1, double c2, double c3, double a);

  color &operator=(const color &c);

  operator int() const {
    return toInt();
  }

  void setAlpha(double a1) { a = a1; }

  double red  () const;
  double green() const;
  double blue () const;
  double alpha() const;

  double brightness() const;

  QColor qcolor() const;

  uint toInt() const;

  uint toIntNoAlpha() const;

  static color fromInt(uint c);

  void getRGBA(CRGBA &rgba) {
    rgba = CRGBA(red(), green(), blue(), alpha());
  }

  friend bool operator==(const color &lhs, const color &rhs) {
    return lhs.toInt() == rhs.toInt();
  }

  friend bool operator!=(const color &lhs, const color &rhs) {
    return lhs.toInt() != rhs.toInt();
  }
};

class Pixels {
 private:
  QImage   image_;
  int      w_, h_;
  int      format_;
  QPainter *painter_;

 public:
  struct Ref {
    Pixels &pixels;
    int     x, y;

    Ref(Pixels &pixels1, int x1, int y1) :
     pixels(pixels1), x(x1), y(y1) {
    }

    void operator=(uint c);
    void operator=(const color &c);
    void operator=(const Ref &ref);

    operator unsigned int() const;
    operator color() const;

    double red  () const;
    double green() const;
    double blue () const;
    double alpha() const;
  };

  friend struct Ref;

  Pixels();
  Pixels(const Pixels &pixels);
 ~Pixels();

  Pixels &operator=(const Pixels &pixels);

  QImage getImage() const { return image_; }

  int getWidth () const { return w_; }
  int getHeight() const { return h_; }

  int length() const { return w_*h_; }

  void setFormat(int format) { format_ = format; }

  QPainter *getPainter() const;

  bool load(StringP name);

  void resize(int w, int h);

  void setImage(QImage image);

  void draw(QPainter *painter, double x=0, double y=0) const;

  Ref operator[](int i);

  void setPixel(int x, int y, const color &c);

  color getPixel(int x, int y) const;

  void fill(const color &c);

  void save(const char *name);
};

struct Stroke {
  color  c;
  double width;
  int    cap;
  int    join;
  bool   active;

  Stroke() {
    reset();
  }

  void reset() {
    c      = color(CProcessing::RGB, 0, 0, 0, 1);
    width  = 1;
    cap    = CProcessing::ROUND;
    join   = CProcessing::MITER;
    active = true;
  }

  void setPen(QPainter *painter);
};

struct Fill {
  color c;
  bool  active;

  Fill() {
   reset();
  }

  void reset() {
    c      = color(CProcessing::RGB, 1, 1, 1, 1);
    active = true;
  }

  void setBrush(QPainter *painter);
};

class PImage : public PObject {
 public:
  Pixels pixels;
  int    width, height;
  int    format;
  Pixels maskPixels;
  bool   hasMask;

  PImage();
  PImage(int w, int h, int format=CProcessing::ARGB);
  PImage(StringP name);
  PImage(const char *name);

  color get(int x, int y) const;

  void loadPixels();
  void updatePixels();

  void resize(int width1, int height1);

  void mask(PImageP image);

  QImage getImage() const;
};

class PShape : public PObject {
 public:
  PShape();
  PShape(StringP name);

  PShapeP getChild(StringP name);
  PShapeP getChild(const char *name);

  void enableStyle ();
  void disableStyle();

  void draw(PGraphicsP graphics, double x, double y) const;
  void draw(PGraphicsP graphics, double x, double y, double w, double h) const;

 private:
#ifdef USE_SVG
  CSVGObject *obj_;
#else
  void       *obj_;
#endif
  bool        styleEnabled_;
};

class PFont : public PObject {
 private:
  ARRAY1D(char,CharArray1D)

  StringP     name_;
  int         size_;
  bool        smooth_;
  CharArray1D chars_;

 public:
  PFont();

  PFont(StringP name, int size, bool smooth, const CharArray1D &chars);

  PFont(StringP name);

  PFont(const PFont &font);

  PFont &operator=(const PFont &font);

  void setSize(int size);

  QFont getQFont() const;

  bool getSmooth() const;
};

class PGraphics : public PObject {
 public:
  enum BgType {
    COLOR_BG,
    IMAGE_BG
  };

 public:
  int         width;
  int         height;

  const char *mode_;
  BgType      bgType_;
  color       bg_;
  PImageP     bgImage_;
  Stroke      stroke_;
  Fill        fill_;
  PFontP      font_;
  int         colorMode_;
  double      cFactor1_;
  double      cFactor2_;
  double      cFactor3_;
  double      aFactor_;
  int         ellipseMode_;
  int         imageMode_;
  int         rectMode_;
  int         textHAlign_;
  int         textVAlign_;
  color       tintColor_;
  bool        tinted_;
  CMatrix2D   matrix2_;
  CMatrix3D   matrix3_;
  double      projection_;

  std::vector<CMatrix2D> matrix2Stack_;
  std::vector<CMatrix3D> matrix3Stack_;

  Pixels pixels;

 public:
  PGraphics();
  PGraphics(int width, int height, const char *mode);

  PGraphics(const PGraphics &graphics);

  PGraphics &operator=(const PGraphics &graphics);

  void resize(int width, int height);

  void beginDraw();

  void endDraw();

  // transform
  void translate(double dx, double dy);
  void translate(double dx, double dy, double dz);

  void rotate(double angle);
  void rotateX(double angle);
  void rotateY(double angle);
  void rotateZ(double angle);

  void scale(double s);
  void scale(double sx, double sy);

  void resetMatrix();

  void perspective(double fov, double aspect, double zmin, double zmax);

  void pushMatrix();
  void popMatrix();

  void updateTransform();

  // set background
  void background(double gray);
  void background(double gray, double alpha);
  void background(double c1, double c2, double c3);
  void background(double c1, double c2, double c3, double alpha);
  void background(StringP name);
  void background(StringP name, double alpha);
  void background(const color &c);
  void background(const color &c, double alpha);
  void background(PImageP image);

  void fillBackground();

  // set stroke color
  void stroke(double gray);
  void stroke(double gray, double alpha);
  void stroke(double c1, double c2, double c3);
  void stroke(double c1, double c2, double c3, double alpha);
  void stroke(StringP name);
  void stroke(StringP name, double alpha);
  void stroke(const color &c);
  void stroke(const color &c, double alpha);

  void noStroke(); // disable stroke

  // set fill color
  void fill(double gray);
  void fill(double gray, double alpha);
  void fill(double c1, double c2, double c3);
  void fill(double c1, double c2, double c3, double alpha);
  void fill(StringP name);
  void fill(StringP name, double alpha);
  void fill(const color &c);
  void fill(const color &c, double alpha);

  void noFill(); // disable fill

  // set stroke width
  void strokeWeight(double width);

  // set stroke line cap
  void strokeCap(int width);

  // set stroke line join
  void strokeJoin(int width);

  // stroke line
  void line(double x1, double y1, double x2, double y2);
  void line(double x1, double y1, double z1, double x2, double y2, double z2);

  // stroke/fill triangle
  void triangle(double x1, double y1, double x2, double y2, double x3, double y3);

  // stroke/fill quadrilateral
  void quad(double x1, double y1, double x2, double y2,
            double x3, double y3, double x4, double y4);

  // stroke curve
  void curve(double x1, double y1, double x2, double y2,
             double x3, double y3, double x4, double y4);

  // stroke/fill rect
  void rect(double x, double y, double w, double h);

  void rectMode(int mode);

  // stroke/fill ellipse
  void ellipse(double x, double y, double xr, double yr);

  void ellipseMode(int mode);

  // stroke/fill arc
  void arc(double x, double y, double width, double height, double angle1, double angle2);

  // 3D box
  void box(double size);

  // stroke/fill bezier
  void bezier(double x1, double y1, double cx1, double cy1,
              double cx2, double cy2, double x3, double y3);

  void image(PImageP image, double x, double y);
  void image(PImageP image, double x, double y, double w, double h);
  void image(PGraphicsP g, double x, double y);
  void image(PGraphicsP g, double x, double y, double w, double h);

  void copy(PImageP image, int x, int y, int w, int h, int dx, int dy, int dw, int dh);
  void copy(PGraphicsP g, int x, int y, int w, int h, int dx, int dy, int dw, int dh);

  void imageMode(int mode);

  // tint image
  void tint(double gray, double alpha);
  void tint(const color &c);
  void noTint();

  // text
  void textAlign(int align);
  void textAlign(int halign, int valign);

  void textFont(PFontP font);
  void textFont(PFontP font, int size);

  void text(char c, double x, double y);
  void text(const char *text, double x, double y);
  void text(StringP text, double x, double y);
  void text(StringP text, double x, double y, double w, double h);

  bool project(double x, double y, double z, double *px, double *y1);

  // color mode and range
  void colorMode(int mode);
  void colorMode(int mode, double c);
  void colorMode(int mode, double r, double g, double b);
  void colorMode(int mode, double r, double g, double b, double a);

  // filter display
  void filter(int mode);

  // anti-alias
  void smooth();
  void noSmooth();

  // shapes/paths
  void beginShape(int type=CProcessing::POLYGON);

  void vertex      (double x, double y);
  void curveVertex (double x, double y);
  void bezierVertex(double x1, double y1, double x2, double y2, double x3, double y3);

  double bezierPoint(double a, double b, double c, double d, double t);

  void endShape(int closeType=CProcessing::OPEN);

  void shape(PShapeP shape, double x, double y);
  void shape(PShapeP shape, double x, double y, double w, double h);

  void  set(int x, int y, const color &c);
  void  set(int x, int y, uint pixel);
  color get(int x, int y);

  void loadPixels();
  void updatePixels();

  void save(const char *name);
  void save(StringP name);
};

struct ShapePoint {
  enum ShapeType {
    POINT,
    CURVE
  };

  ShapeType type;

  double x1, y1, x2, y2, x3, y3;

  ShapePoint(double x, double y) :
   type(POINT), x1(x), y1(y) {
  }

  ShapePoint(double tx1, double ty1, double tx2, double ty2, double tx3, double ty3) :
   type(CURVE), x1(tx1), y1(ty1), x2(tx2), y2(ty2), x3(tx3), y3(ty3) {
  }
};

namespace CProcessing {
  extern int    &width;
  extern int    &height;
  extern Pixels &pixels;

  extern const char *drawMode;
  extern int         mouseX, mouseY;
  extern int         pmouseX, pmouseY;
  extern bool        mousePressed;
  extern int         mouseButton;
  extern char        key;
  extern int         keyCode;
  extern bool        keyPressed;
  extern int         frameCount;

  extern bool        in3D;
};

namespace CProcessing {
  ARRAY1D(char,CharArray1D)

  void setWindow(QWidget *window);

  void doInit();
  void doSetup();
  void doResize(int w, int h);
  void doDraw3D();
  void doDraw();
  void resetDraw();
  void doLoop();

  void mousePress  (int x, int y);
  void mouseMotion (int x, int y);
  void mouseRelease(int x, int y);

  void mouseUpdatePos(int x, int y);

  void keyPress  (int code, char c);
  void keyRelease(int code, char c);

  void size(int w, int h, const char *mode=JAVA2D); // set size

  void frameRate(int fps); // set frame rate (draw's per sec)
  int  frameRate();        // get frame rate (draw's per sec)

  void loop(); // enable call of draw every frame
  void noLoop(); // disable call of draw every frame

  void redraw();

  // color mode and range
  void colorMode(int mode);
  void colorMode(int mode, double c);
  void colorMode(int mode, double r, double g, double b);
  void colorMode(int mode, double r, double g, double b, double a);

  // filter display
  void filter(int mode);

  // blend colors
  color blendColor(const color &c1, const color &c2, int mode);

  // anti-alias
  void smooth();
  void noSmooth();

  // transform
  void translate(double dx, double dy);
  void translate(double dx, double dy, double dz);

  void rotate(double angle);
  void rotateX(double angle);
  void rotateY(double angle);
  void rotateZ(double angle);

  void scale(double s);
  void scale(double sx, double sy);

  void resetMatrix();

  void perspective(double fov, double aspect, double zmin, double zmax);

  void pushMatrix();
  void popMatrix();

  void cursor(int shape);
  void noCursor();

  // set background
  void background(double gray);
  void background(double gray, double alpha);
  void background(double c1, double c2, double c3);
  void background(double c1, double c2, double c3, double alpha);
  void background(StringP name);
  void background(StringP name, double alpha);
  void background(const color &c);
  void background(const color &c, double alpha);
  void background(PImageP image);

  // set fill color
  void fill(double gray);
  void fill(double gray, double alpha);
  void fill(double c1, double c2, double c3);
  void fill(double c1, double c2, double c3, double alpha);
  void fill(StringP name);
  void fill(StringP name, double alpha);
  void fill(const color &c);
  void fill(const color &c, double alpha);

  void noFill(); // disable fill

  // set stroke color
  void stroke(double gray);
  void stroke(double gray, double alpha);
  void stroke(double c1, double c2, double c3);
  void stroke(double c1, double c2, double c3, double alpha);
  void stroke(StringP name);
  void stroke(StringP name, double alpha);
  void stroke(const color &c);
  void stroke(const color &c, double alpha);

  void noStroke(); // disable stroke

  // set stroke width
  void strokeWeight(double width);

  // set stroke line cap
  void strokeCap(int width);

  // set stroke line join
  void strokeJoin(int width);

  // stroke point
  void point(double x, double y);

  // stroke line
  void line(double x1, double y1, double x2, double y2);

  // stroke/fill triangle
  void triangle(double x1, double y1, double x2, double y2, double x3, double y3);

  // stroke/fill quadrilateral
  void quad(double x1, double y1, double x2, double y2,
            double x3, double y3, double x4, double y4);

  // stroke curve
  void curve(double x1, double y1, double x2, double y2,
             double x3, double y3, double x4, double y4);

  // stroke/fill rect
  void rect(double x, double y, double w, double h);

  void rectMode(int mode);

  // stroke/fill ellipse
  void ellipse(double x, double y, double xr, double yr);

  void ellipseMode(int mode);

  // stroke/fill arc
  void arc(double x, double y, double width, double height, double angle1, double angle2);

  // 3D box
  void box(double size);

  // stroke/fill bezier
  void bezier(double x1, double y1, double cx1, double cy1,
              double cx2, double cy2, double x3, double y3);

  // shapes/paths
  void beginShape(int type=CProcessing::POLYGON);

  void vertex      (double x, double y);
  void curveVertex (double x, double y);
  void bezierVertex(double x1, double y1, double x2, double y2, double x3, double y3);

  double bezierPoint(double a, double b, double c, double d, double t);

  void endShape(int closeType=OPEN);

  PShapeP loadShape(StringP name);
  PShapeP loadShape(const char *name);

  void shape(PShapeP shape, double x, double y);
  void shape(PShapeP shape, double x, double y, double w, double h);

  // image
  PImageP loadImage(const char *name);
  PImageP loadImage(StringP name);

  PImageP requestImage(StringP name);

  PImageP createImage(int width, int height, int format);

  void image(PImageP image, double x, double y);
  void image(PImageP image, double x, double y, double w, double h);
  void image(PGraphicsP g, double x, double y);
  void image(PGraphicsP g, double x, double y, double w, double h);

  void copy(PImageP image, int x, int y, int w, int h, int dx, int dy, int dw, int dh);
  void copy(PGraphicsP g, int x, int y, int w, int h, int dx, int dy, int dw, int dh);

  void imageMode(int mode);

  // tint image
  void tint(double gray, double alpha);
  void tint(const color &c);
  void noTint();

  // text
  void textAlign(int align);
  void textAlign(int halign, int valign);

  void textFont(PFontP font);
  void textFont(PFontP font, int size);

  void text(char c, double x, double y);
  void text(const char *text, double x, double y);
  void text(StringP text, double x, double y);
  void text(StringP text, double x, double y, double w, double h);

  double red  (const Pixels::Ref &ref);
  double green(const Pixels::Ref &ref);
  double blue (const Pixels::Ref &ref);
  double alpha(const Pixels::Ref &ref);

  double red  (const color &c);
  double green(const color &c);
  double blue (const color &c);
  double alpha(const color &c);

  double brightness(const color &c);

  // graphics
  PGraphicsP createGraphics(int width, int height, const char *mode);

  // pixels
  void loadPixels();
  void updatePixels();

  void  set(int x, int y, const color &c);
  void  set(int x, int y, uint pixel);
  color get(int x, int y);

  // angle
  double radians(double a);
  double degrees(double a);

  // font
  PFontP createFont(StringP name, int size, bool smooth, const CharArray1D &chars);

  PFontP loadFont(const char *name);
  PFontP loadFont(StringP name);

  // time

  int second();

  int millis();

  // ---- Math ...

  template<typename T>
  static const T &min(const T &a, const T &b) {
    return (a < b ? a : b);
  }

  template<typename T>
  static const T &max(const T &a, const T &b) {
    return (a < b ? b : a);
  }

  template<typename T>
  static T max(const FArray1D<T> &a) {
    uint len = a.length;

    assert(len > 0);

    T v = a[0];

    for (uint i = 1; i < len; ++i)
      v = max(v, a[i]);

    return v;
  }

  template<typename T>
  static T max(const PArray1D<T> &a) {
    uint len = a.length;

    assert(len > 0);

    T v = a[0];

    for (uint i = 1; i < len; ++i)
      v = max(v, a[i]);

    return v;
  }

  int floor(float f);
  int ceil(float f);

  // square
  double sq(double x);

  // normalize
  double norm(double x, double low, double high);

  // magnitude
  double mag(double x, double y);
  double mag(double x, double y, double z);

  // lerp
  double lerp(double value1, double value2, double amt);

  // map
  double map(double value, double low1, double high1,
             double low2, double high2);

  // random in range
  double random(double high);
  double random(double low, double high);

  void randomSeed(double seed);

  double noise(double x);
  double noise(double x, double y);
  double noise(double x, double y, double z);

  // distance
  double dist(double x1, double y1, double x2, double y2);

  double modulus(double a, double b);

  double constrain(double v, double lower, double upper);

  // lighting

  void lights();

  void directionalLight(double v1, double v2, double v3, double nx, double ny, double nz);

  // ---- Misc

  void unhandled();

  template<typename T>
  void println(const IRefPtr<T> &t) {
    std::cout << *t << std::endl;
  }

  template<typename T>
  void println(const T &t) {
    std::cout << t << std::endl;
  }

  template<typename T>
  void arraycopy(const FArray1D<T> &src, FArray1D<T> &dst) {
    for (int i = 0; i < src.length && i < dst.length; ++i)
      dst[i] = src[i];
  }

  template<typename T>
  void arraycopy(const PArray1D<T> &src, PArray1D<T> &dst) {
    for (int i = 0; i < src.length && i < dst.length; ++i)
      dst[i] = src[i];
  }

  void save(const char *name);
  void save(StringP name);

  StringP nf(int i, int digits);
  StringP nf(float f, int left, int right);
};

namespace System {
  long nanoTime();
};

POBJECT_INTERFACE(Runnable)
  IMETHOD(void, run())
};

#include <CThread.h>

typedef IRefPtr<Runnable> RunnableP;

class Thread : public Runnable {
 public:
  Thread() { data_ = new ThreadData(this); }

  virtual ~Thread() { delete data_; }

  void start() {
    data_->start();
  }

  virtual void run() { }

 private:
  class ThreadData : public CThread {
   public:
    ThreadData(Thread *t) :
     t_(t) {
    }

    void *execute() {
      t_->run();

      return NULL;
    }

   private:
    Thread *t_;
  };

 private:
  ThreadData *data_;
};

typedef IRefPtr<Thread> ThreadP;

using namespace CProcessing;

extern void setup();
extern void draw ();

extern void keyPressed();
extern void keyReleased();
extern void keyTyped();
extern void mousePressed();
extern void mouseReleased();
extern void mouseClicked();
extern void mouseMoved();
extern void mouseDragged();

#define NULL_KEY_TYPED      void keyTyped() { }
#define NULL_KEY_PRESSED    void keyPressed() { }
#define NULL_KEY_RELEASED   void keyReleased() { }
#define NULL_MOUSE_CLICKED  void mouseClicked() { }
#define NULL_MOUSE_PRESSED  void mousePressed() { }
#define NULL_MOUSE_RELEASED void mouseReleased() { }
#define NULL_MOUSE_MOVED    void mouseMoved() { }
#define NULL_MOUSE_DRAGGED  void mouseDragged() { }

/* All (for cut/paste)
NULL_KEY_TYPED
NULL_KEY_PRESSED
NULL_KEY_RELEASED
NULL_MOUSE_CLICKED
NULL_MOUSE_PRESSED
NULL_MOUSE_RELEASED
NULL_MOUSE_MOVED
NULL_MOUSE_DRAGGED
*/

#endif
