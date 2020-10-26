#include <CProcessing.h>
#include <CProcessingTimer.h>
#include <CProcessingWindow.h>
#include <CProcessingWindow3D.h>

#define USE_SVG 1

#ifdef USE_SVG
#include <CSVG.h>
#include <CSVGImageRenderer.h>
#endif

#include <CQImageFilter.h>

#include <CMathGen.h>
#include <CMathRand.h>
#include <CRGB.h>
#include <CHSB.h>
#include <CRGBName.h>
#include <COSTime.h>
#include <CRGBUtil.h>

#include <QApplication>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QColormap>

typedef std::vector<ShapePoint> ShapePointList;

static bool               setupDone_;
static QWidget           *window_;
static QPainterPath      *path_;
static int                fps_;
static bool               loop_;
static PGraphics          graphics_;
static int                shapeMode_;
static ShapePointList     shapePointList_;
static CHRTime            startTime_;
static CImagePtr          image_;
#ifdef USE_SVG
static CSVG              *svg_;
static CSVGImageRenderer *svgRenderer_;
#endif
static CImageRenderer2D  *irenderer_;
static CProcessingTimer  *timer_;

int    &CProcessing::width  = graphics_.width;
int    &CProcessing::height = graphics_.height;
Pixels &CProcessing::pixels = graphics_.pixels;

const char *CProcessing::drawMode;
int         CProcessing::mouseX;
int         CProcessing::mouseY;
int         CProcessing::pmouseX;
int         CProcessing::pmouseY;
bool        CProcessing::mousePressed;
int         CProcessing::mouseButton;
char        CProcessing::key;
bool        CProcessing::keyPressed;
int         CProcessing::keyCode;

int         CProcessing::frameCount;

bool        CProcessing::in3D;

const int CProcessing::X = 0;
const int CProcessing::Y = 1;
const int CProcessing::Z = 2;

const int CProcessing::R = 3;
const int CProcessing::G = 4;
const int CProcessing::B = 5;
const int CProcessing::A = 6;

const int CProcessing::U = 7;
const int CProcessing::V = 8;

const int CProcessing::NX = 9;
const int CProcessing::NY = 10;
const int CProcessing::NZ = 11;

const int CProcessing::EDGE = 12;

const int CProcessing::SR = 13;
const int CProcessing::SG = 14;
const int CProcessing::SB = 15;
const int CProcessing::SA = 16;

const int CProcessing::SW = 17;

const int CProcessing::TX = 18;
const int CProcessing::TY = 19;
const int CProcessing::TZ = 20;

const int CProcessing::VX = 21;
const int CProcessing::VY = 22;
const int CProcessing::VZ = 23;
const int CProcessing::VW = 24;

const int CProcessing::AR = 25;
const int CProcessing::AG = 26;
const int CProcessing::AB = 27;

const int CProcessing::DR = 3;
const int CProcessing::DG = 4;
const int CProcessing::DB = 5;
const int CProcessing::DA = 6;

const int CProcessing::SPR = 28;
const int CProcessing::SPG = 29;
const int CProcessing::SPB = 30;

const int CProcessing::SHINE = 31;

const int CProcessing::ER = 32;
const int CProcessing::EG = 33;
const int CProcessing::EB = 34;

const int CProcessing::BEEN_LIT = 35;

const int CProcessing::VERTEX_FIELD_COUNT = 36;

const char *CProcessing::P2D    = "processing.core.PGraphics2D";
const char *CProcessing::P3D    = "processing.core.PGraphics3D";
const char *CProcessing::JAVA2D = "processing.core.PGraphicsJava2D";
const char *CProcessing::OPENGL = "processing.opengl.PGraphicsOpenGL";
const char *CProcessing::PDF    = "processing.pdf.PGraphicsPDF";
const char *CProcessing::DXF    = "processing.dxf.RawDXF";

const int CProcessing::OTHER   = 0;
const int CProcessing::WINDOWS = 1;
const int CProcessing::MACOSX  = 2;
const int CProcessing::LINUX   = 3;

const char *CProcessing::platformNames[] = {
  "other", "windows", "macosx", "linux"
};

const float CProcessing::EPSILON = 0.0001f;

const float CProcessing::MAX_FLOAT = FLT_MAX;
const float CProcessing::MIN_FLOAT = -FLT_MAX;

const int CProcessing::MAX_INT = INT_MAX;
const int CProcessing::MIN_INT = INT_MIN;

const float CProcessing::PI = (float) M_PI;
const float CProcessing::HALF_PI    = PI / 2.0f;
const float CProcessing::THIRD_PI   = PI / 3.0f;
const float CProcessing::QUARTER_PI = PI / 4.0f;
const float CProcessing::TWO_PI     = PI * 2.0f;

const float CProcessing::DEG_TO_RAD = PI/180.0f;
const float CProcessing::RAD_TO_DEG = 180.0f/PI;

//const int CProcessing::RADIANS = 0;
//const int CProcessing::DEGREES = 1;

const char *CProcessing::WHITESPACE = " \t\n\r\f\u00A0";

const int CProcessing::RGB   = 1;
const int CProcessing::ARGB  = 2;
const int CProcessing::HSB   = 3;
const int CProcessing::ALPHA = 4;
const int CProcessing::CMYK  = 5;

const int CProcessing::TIFF  = 0;
const int CProcessing::TARGA = 1;
const int CProcessing::JPEG  = 2;
const int CProcessing::GIF   = 3;

const int CProcessing::BLUR      = 11;
const int CProcessing::GRAY      = 12;
const int CProcessing::INVERT    = 13;
const int CProcessing::OPAQUE    = 14;
const int CProcessing::POSTERIZE = 15;
const int CProcessing::THRESHOLD = 16;
const int CProcessing::ERODE     = 17;
const int CProcessing::DILATE    = 18;

const int CProcessing::REPLACE    = 0;
const int CProcessing::BLEND      = 1 << 0;
const int CProcessing::ADD        = 1 << 1;
const int CProcessing::SUBTRACT   = 1 << 2;
const int CProcessing::LIGHTEST   = 1 << 3;
const int CProcessing::DARKEST    = 1 << 4;
const int CProcessing::DIFFERENCE = 1 << 5;
const int CProcessing::EXCLUSION  = 1 << 6;
const int CProcessing::MULTIPLY   = 1 << 7;
const int CProcessing::SCREEN     = 1 << 8;
const int CProcessing::OVERLAY    = 1 << 9;
const int CProcessing::HARD_LIGHT = 1 << 10;
const int CProcessing::SOFT_LIGHT = 1 << 11;
const int CProcessing::DODGE      = 1 << 12;
const int CProcessing::BURN       = 1 << 13;

const int CProcessing::ALPHA_MASK = 0xff000000;
const int CProcessing::RED_MASK   = 0x00ff0000;
const int CProcessing::GREEN_MASK = 0x0000ff00;
const int CProcessing::BLUE_MASK  = 0x000000ff;

const int CProcessing::CHATTER   = 0;
const int CProcessing::COMPLAINT = 1;
const int CProcessing::PROBLEM   = 2;

const int CProcessing::CUSTOM       = 0;
const int CProcessing::ORTHOGRAPHIC = 2;
const int CProcessing::PERSPECTIVE  = 3;

//const int GROUP           = (1 << 2);

const int CProcessing::POINT           = 2;
const int CProcessing::POINTS          = 2;

const int CProcessing::LINE            = 4;
const int CProcessing::LINES           = 4;
const int CProcessing::LINE_STRIP      = 5;

const int CProcessing::TRIANGLE        = 8;
const int CProcessing::TRIANGLES       = 9;
const int CProcessing::TRIANGLE_STRIP  = 10;
const int CProcessing::TRIANGLE_FAN    = 11;

const int CProcessing::QUAD            = 16;
const int CProcessing::QUADS           = 16;
const int CProcessing::QUAD_STRIP      = 17;

const int CProcessing::POLYGON         = 20;
const int CProcessing::PATH            = 21;

const int CProcessing::RECT            = 30;
const int CProcessing::ELLIPSE         = 31;
const int CProcessing::ARC             = 32;

const int CProcessing::SPHERE          = 40;
const int CProcessing::BOX             = 41;

const int CProcessing::OPEN = 1;
const int CProcessing::CLOSE = 2;

const int CProcessing::CORNER   = 0;
const int CProcessing::CORNERS  = 1;
const int CProcessing::RADIUS   = 2;
const int CProcessing::CENTER_RADIUS = 2;
const int CProcessing::CENTER   = 3;
const int CProcessing::DIAMETER = 3;
const int CProcessing::CENTER_DIAMETER = 3;

const int CProcessing::BASELINE = 0;
const int CProcessing::TOP = 101;
const int CProcessing::BOTTOM = 102;

const int CProcessing::NORMAL     = 1;
const int CProcessing::NORMALIZED = 1;
const int CProcessing::IMAGE      = 2;

const int CProcessing::MODEL = 4;
const int CProcessing::SHAPE = 5;

const int CProcessing::SQUARE   = 1 << 0;
const int CProcessing::ROUND    = 1 << 1;
const int CProcessing::PROJECT  = 1 << 2;
const int CProcessing::MITER    = 1 << 3;
const int CProcessing::BEVEL    = 1 << 5;

const int CProcessing::AMBIENT = 0;
const int CProcessing::DIRECTIONAL  = 1;
//const int POINT  = 2;
const int CProcessing::SPOT = 3;

const char CProcessing::BACKSPACE = 8;
const char CProcessing::TAB       = 9;
const char CProcessing::ENTER     = 10;
const char CProcessing::RETURN    = 13;
const char CProcessing::ESC       = 27;
const char CProcessing::DELETE    = 127;

const int CProcessing::CODED     = 0xffff;

const int CProcessing::UP        = CKEY_TYPE_Up;
const int CProcessing::DOWN      = CKEY_TYPE_Down;
const int CProcessing::LEFT      = CKEY_TYPE_Left;
const int CProcessing::RIGHT     = CKEY_TYPE_Right;

const int CProcessing::ALT       = CKEY_TYPE_Alt_L;
const int CProcessing::CONTROL   = CKEY_TYPE_Control_L;
const int CProcessing::SHIFT     = CKEY_TYPE_Shift_L;
const int CProcessing::ARROW = CURSOR_TYPE_ARROW;
const int CProcessing::CROSS = CURSOR_TYPE_CROSS_HAIR;
const int CProcessing::HAND  = CURSOR_TYPE_HAND;
const int CProcessing::MOVE  = CURSOR_TYPE_MOVE;
const int CProcessing::TEXT  = CURSOR_TYPE_TEXT;
const int CProcessing::WAIT  = CURSOR_TYPE_WATCH;

const int CProcessing::DISABLE_OPENGL_2X_SMOOTH    =  1;
const int CProcessing::ENABLE_OPENGL_2X_SMOOTH     = -1;
const int CProcessing::ENABLE_OPENGL_4X_SMOOTH     =  2;

const int CProcessing::ENABLE_NATIVE_FONTS         =  3;

const int CProcessing::DISABLE_DEPTH_TEST          =  4;
const int CProcessing::ENABLE_DEPTH_TEST           = -4;

const int CProcessing::ENABLE_DEPTH_SORT           =  5;
const int CProcessing::DISABLE_DEPTH_SORT          = -5;

const int CProcessing::DISABLE_OPENGL_ERROR_REPORT =  6;
const int CProcessing::ENABLE_OPENGL_ERROR_REPORT  = -6;

const int CProcessing::ENABLE_ACCURATE_TEXTURES    =  7;
const int CProcessing::DISABLE_ACCURATE_TEXTURES   = -7;

const int CProcessing::HINT_COUNT                  = 10;

const char *CProcessing::ERROR_BACKGROUND_IMAGE_SIZE =
  "background image must be the same size as your application";
const char *CProcessing::ERROR_BACKGROUND_IMAGE_FORMAT =
  "background images should be RGB or ARGB";

const char *CProcessing::ERROR_TEXTFONT_NULL_PFONT =
  "A null PFont was passed to textFont()";

const char *CProcessing::ERROR_PUSHMATRIX_OVERFLOW =
  "Too many calls to pushMatrix().";
const char *CProcessing::ERROR_PUSHMATRIX_UNDERFLOW =
  "Too many calls to popMatrix(), and not enough to pushMatrix().";

#ifdef USE_SVG
CSVG *
getSVG()
{
  if (! svg_) {
    CImageNoSrc src;

    image_ = CImageMgrInst->createImage(src);

    svg_         = new CSVG;
    irenderer_   = new CImageRenderer2D(image_);
    svgRenderer_ = new CSVGImageRenderer(irenderer_);

    svg_->setRenderer(svgRenderer_);

    svg_->init();

    //svg_->setSimplifyPath(true);
  }

  return svg_;
}
#endif

void
CProcessing::
doInit()
{
  window_    = NULL;
  path_      = NULL;
  fps_       = 10;
  loop_      = true;
  startTime_ = COSTime::getHRTime();

  double g = 204.0/255.0;

  background(color(RGB, g, g, g, 1.0));

  drawMode = JAVA2D;

  mouseX  = 0;
  mouseY  = 0;
  pmouseX = 0;
  pmouseY = 0;

  mousePressed = false;
  keyPressed   = false;
  mouseButton  = 0;

  key = 0;

  frameCount = 0;
}

void
CProcessing::
setWindow(QWidget *window)
{
  if (window_)
    window_->deleteLater();

  window_ = window;
}

void
CProcessing::
doResize(int w, int h)
{
  graphics_.resize(w, h);

  graphics_.fillBackground();
}

void
CProcessing::
doSetup()
{
  if (! setupDone_) {
    CMathRand::seedRand(time(NULL));

    setup();

    window_->show();

    graphics_.fillBackground();

    setupDone_ = true;
  }
}

void
CProcessing::
doDraw3D()
{
  doSetup();

  resetDraw();

  draw();
}

void
CProcessing::
doDraw()
{
  if (! loop_) {
    doSetup();

    resetDraw();

    draw();
  }

  QPainter painter(window_);

  painter.drawImage(0, 0, graphics_.pixels.getImage());
}

void
CProcessing::
resetDraw()
{
  //graphics_.stroke_.reset();
  //graphics_.fill_  .reset();

  //graphics_.stroke_.setPen  (graphics_.pixels.getPainter());
  //graphics_.fill_  .setBrush(graphics_.pixels.getPainter());

  graphics_.beginDraw();
}

void
CProcessing::
doLoop()
{
  if (drawMode != P3D) {
    if (loop_) {
      ++frameCount;

      doSetup();

      resetDraw();

      draw();
    }

    window_->repaint();
  }
  else {
    ++frameCount;

    window_->repaint();
  }
}

void
CProcessing::
mousePress(int x, int y)
{
  mousePressed = true;

  pmouseX = x;
  pmouseY = y;
  mouseX  = x;
  mouseY  = y;

  ::mousePressed();
}

void
CProcessing::
mouseMotion(int x, int y)
{
  mouseUpdatePos(x, y);

  if (mousePressed)
    ::mouseDragged();
  else
    ::mouseMoved();
}

void
CProcessing::
mouseRelease(int x, int y)
{
  mousePressed = false;

  mouseUpdatePos(x, y);

  ::mouseReleased();

  ::mouseClicked();
}

void
CProcessing::
mouseUpdatePos(int x, int y)
{
  pmouseX = mouseX;
  pmouseY = mouseY;

  mouseX = x;
  mouseY = y;
}

void
CProcessing::
keyPress(int code, char c)
{
  keyPressed = true;

  keyCode = code;
  key     = c;

  ::keyPressed();
}

void
CProcessing::
keyRelease(int code, char c)
{
  keyPressed = false;

  keyCode = code;
  key     = c;

  ::keyReleased();

  ::keyTyped();
}

void
CProcessing::
size(int w, int h, const char *mode)
{
  drawMode = mode;

  if (! window_) {
    if (drawMode == P3D)
      window_ = new CProcessingWindow3D(w, h);
    else
      window_ = new CProcessingWindow(w, h);
  }

  graphics_.resize(w, h);
}

void
CProcessing::
frameRate(int fps)
{
  fps_ = fps;

  if (loop_) {
    timer_->stopTimer();

    timer_->startTimer(fps_);
  }
}

int
CProcessing::
frameRate()
{
  return fps_;
}

void
CProcessing::
loop()
{
  loop_ = true;

  timer_->stopTimer();

  timer_->startTimer(fps_);
}

void
CProcessing::
noLoop()
{
  loop_ = false;

  timer_->stopTimer();
}

void
CProcessing::
redraw()
{
  window_->update();
}

void
CProcessing::
colorMode(int mode)
{
  graphics_.colorMode(mode);
}

void
CProcessing::
colorMode(int mode, double c)
{
  graphics_.colorMode(mode, c);
}

void
CProcessing::
colorMode(int mode, double c1, double c2, double c3)
{
  graphics_.colorMode(mode, c1, c2, c3);
}

void
CProcessing::
colorMode(int mode, double c1, double c2, double c3, double a)
{
  graphics_.colorMode(mode, c1, c2, c3, a);
}

void
CProcessing::
background(double gray)
{
  graphics_.background(gray);
}

void
CProcessing::
background(double gray, double alpha)
{
  graphics_.background(gray, alpha);
}

void
CProcessing::
background(double c1, double c2, double c3)
{
  graphics_.background(c1, c2, c3);
}

void
CProcessing::
background(double c1, double c2, double c3, double alpha)
{
  graphics_.background(c1, c2, c3, alpha);
}

void
CProcessing::
background(StringP name)
{
  graphics_.background(name);
}

void
CProcessing::
background(StringP name, double alpha)
{
  graphics_.background(name, alpha);
}

void
CProcessing::
background(const color &c)
{
  graphics_.background(c);
}

void
CProcessing::
background(const color &c, double alpha)
{
  graphics_.background(c, alpha);
}

void
CProcessing::
background(PImageP image)
{
  graphics_.background(image);
}

void
CProcessing::
translate(double dx, double dy)
{
  graphics_.translate(dx, dy);
}

void
CProcessing::
translate(double dx, double dy, double dz)
{
  graphics_.translate(dx, dy, dz);
}

void
CProcessing::
rotate(double angle)
{
  graphics_.rotate(angle);
}

void
CProcessing::
rotateX(double angle)
{
  graphics_.rotateX(angle);
}

void
CProcessing::
rotateY(double angle)
{
  graphics_.rotateY(angle);
}

void
CProcessing::
rotateZ(double angle)
{
  graphics_.rotateZ(angle);
}

void
CProcessing::
scale(double s)
{
  graphics_.scale(s);
}

void
CProcessing::
scale(double sx, double sy)
{
  graphics_.scale(sx, sy);
}

void
CProcessing::
resetMatrix()
{
  graphics_.resetMatrix();
}

void
CProcessing::
cursor(int)
{
}

void
CProcessing::
noCursor()
{
}

void
CProcessing::
noStroke()
{
  graphics_.noStroke();
}

void
CProcessing::
noFill()
{
  graphics_.noFill();
}

void
CProcessing::
fill(double gray)
{
  graphics_.fill(gray);
}

void
CProcessing::
fill(double gray, double alpha)
{
  graphics_.fill(gray, alpha);
}

void
CProcessing::
fill(double c1, double c2, double c3)
{
  graphics_.fill(c1, c2, c3);
}

void
CProcessing::
fill(double c1, double c2, double c3, double alpha)
{
  graphics_.fill(c1, c2, c3, alpha);
}

void
CProcessing::
fill(StringP name)
{
  graphics_.fill(name);
}

void
CProcessing::
fill(StringP name, double alpha)
{
  graphics_.fill(name, alpha);
}

void
CProcessing::
fill(const color &c)
{
  graphics_.fill(c);
}

void
CProcessing::
fill(const color &c, double alpha)
{
  graphics_.fill(c, alpha);
}

void
CProcessing::
stroke(double gray)
{
  graphics_.stroke(gray);
}

void
CProcessing::
stroke(double gray, double alpha)
{
  graphics_.stroke(gray, alpha);
}

void
CProcessing::
stroke(double c1, double c2, double c3)
{
  graphics_.stroke(c1, c2, c3);
}

void
CProcessing::
stroke(double c1, double c2, double c3, double alpha)
{
  graphics_.stroke(c1, c2, c3, alpha);
}

void
CProcessing::
stroke(StringP name)
{
  graphics_.stroke(name);
}

void
CProcessing::
stroke(StringP name, double alpha)
{
  graphics_.stroke(name, alpha);
}

void
CProcessing::
stroke(const color &c)
{
  graphics_.stroke(c);
}

void
CProcessing::
stroke(const color &c, double alpha)
{
  graphics_.stroke(c, alpha);
}

void
CProcessing::
strokeWeight(double width)
{
  graphics_.strokeWeight(width);
}

void
CProcessing::
strokeCap(int cap)
{
  graphics_.strokeCap(cap);
}

void
CProcessing::
strokeJoin(int join)
{
  graphics_.strokeJoin(join);
}

void
CProcessing::
set(int x, int y, const color &c)
{
  graphics_.set(x, y, c);
}

void
CProcessing::
set(int x, int y, uint pixel)
{
  graphics_.set(x, y, pixel);
}

color
CProcessing::
get(int x, int y)
{
  return graphics_.pixels.getPixel(x, y);
}

void
CProcessing::
point(double x, double y)
{
  if (graphics_.stroke_.active) {
    graphics_.pixels.getPainter()->setPen(graphics_.stroke_.c.qcolor());

    graphics_.pixels.getPainter()->drawPoint(x, y);
  }
}

void
CProcessing::
line(double x1, double y1, double x2, double y2)
{
  graphics_.line(x1, y1, x2, y2);
}

void
CProcessing::
triangle(double x1, double y1, double x2, double y2, double x3, double y3)
{
  graphics_.triangle(x1, y1, x2, y2, x3, y3);
}

void
CProcessing::
quad(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{
  graphics_.quad(x1, y1, x2, y2, x3, y3, x4, y4);
}

void
CProcessing::
curve(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{
  graphics_.curve(x1, y1, x2, y2, x3, y3, x4, y4);
}

void
CProcessing::
rect(double p1, double p2, double p3, double p4)
{
  graphics_.rect(p1, p2, p3, p4);
}

void
CProcessing::
rectMode(int mode)
{
  graphics_.rectMode(mode);
}

void
CProcessing::
box(double size)
{
  graphics_.box(size);
}

void
CProcessing::
ellipse(double p1, double p2, double p3, double p4)
{
  graphics_.ellipse(p1, p2, p3, p4);
}

void
CProcessing::
ellipseMode(int mode)
{
  graphics_.ellipseMode(mode);
}

void
CProcessing::
arc(double x, double y, double width, double height, double angle1, double angle2)
{
  graphics_.arc(x, y, width, height, angle1, angle2);
}

void
CProcessing::
bezier(double x1, double y1, double cx1, double cy1, double cx2, double cy2, double x2, double y2)
{
  graphics_.bezier(x1, y1, cx1, cy1, cx2, cy2, x2, y2);
}

void
CProcessing::
tint(double gray, double alpha)
{
  graphics_.tint(gray, alpha);
}

void
CProcessing::
tint(const color &c)
{
  graphics_.tint(c);
}

void
CProcessing::
noTint()
{
  graphics_.noTint();
}

void
CProcessing::
filter(int mode)
{
  graphics_.filter(mode);
}

namespace BlendAdd {
  double calc(double c1, double c2) {
    return std::min(std::max(c1 + c2, 0.0), 1.0);
  }
};

namespace BlendSoftLight {
  double F(double x) {
    if (x <= 0.25)
      return ((16*x - 12)*x + 4)*x;
    else
      return x;
  }

  double calc(double c1, double c2) {
    double cr;

    if (c1 <= 0.5)
      cr = c2 - (1 - 2*c1)*c2*(1 - c2);
    else
      cr = c2 + (2*c1 - 1)*(F(c2) - c2);

    return std::min(std::max(cr, 0.0), 1.0);
  }
};

color
CProcessing::
blendColor(const color &c1, const color &c2, int mode)
{
  if      (mode == ADD)
    return color(c1.mode,
                 BlendAdd::calc(c1.c1, c2.c1),
                 BlendAdd::calc(c1.c2, c2.c2),
                 BlendAdd::calc(c1.c3, c2.c3),
                 BlendAdd::calc(c1.a , c2.a ));
  else if (mode == SOFT_LIGHT) {
    return color(c1.mode,
                 BlendSoftLight::calc(c2.c1, c1.c1),
                 BlendSoftLight::calc(c2.c2, c1.c2),
                 BlendSoftLight::calc(c2.c3, c1.c3),
                 BlendSoftLight::calc(c2.a , c1.a ));
  }
  else {
    unhandled();
    return c1;
  }
}

void
CProcessing::
smooth()
{
  graphics_.smooth();
}

void
CProcessing::
noSmooth()
{
  graphics_.noSmooth();
}

void
CProcessing::
beginShape(int mode)
{
  graphics_.beginShape(mode);
}

void
CProcessing::
vertex(double x, double y)
{
  graphics_.vertex(x, y);
}

void
CProcessing::
curveVertex(double x, double y)
{
  graphics_.curveVertex(x, y);
}

void
CProcessing::
bezierVertex(double x1, double y1, double x2, double y2, double x3, double y3)
{
  graphics_.bezierVertex(x1, y1, x2, y2, x3, y3);
}

double
CProcessing::
bezierPoint(double a, double b, double c, double d, double t)
{
  return graphics_.bezierPoint(a, b, c, d, t);
}

void
CProcessing::
endShape(int closeType)
{
  graphics_.endShape(closeType);
}

PShapeP
CProcessing::
loadShape(StringP name)
{
  return new PShape(name);
}

PShapeP
CProcessing::
loadShape(const char *name)
{
  return new PShape(new String(name));
}

void
CProcessing::
shape(PShapeP s, double x, double y)
{
  graphics_.shape(s, x, y);
}

void
CProcessing::
shape(PShapeP s, double x, double y, double w, double h)
{
  graphics_.shape(s, x, y, w, h);
}

//------

PImageP
CProcessing::
loadImage(const char *name)
{
  return new PImage(name);
}

PImageP
CProcessing::
loadImage(StringP name)
{
  return new PImage(name);
}

PImageP
CProcessing::
requestImage(StringP name)
{
  return new PImage(name);
}

PImageP
CProcessing::
createImage(int width, int height, int format)
{
  return new PImage(width, height, format);
}

void
CProcessing::
image(PImageP image, double x, double y)
{
  graphics_.image(image, x, y);
}

void
CProcessing::
image(PImageP image, double x, double y, double w, double h)
{
  graphics_.image(image, x, y, w, h);
}

void
CProcessing::
image(PGraphicsP g, double x, double y)
{
  graphics_.image(g, x, y);
}

void
CProcessing::
image(PGraphicsP g, double x, double y, double w, double h)
{
  graphics_.image(g, x, y, w, h);
}

void
CProcessing::
copy(PImageP src, int x, int y, int w, int h, int dx, int dy, int dw, int dh)
{
  graphics_.copy(src, x, y, w, h, dx, dy, dw, dh);
}

void
CProcessing::
copy(PGraphicsP src, int x, int y, int w, int h, int dx, int dy, int dw, int dh)
{
  graphics_.copy(src, x, y, w, h, dx, dy, dw, dh);
}

void
CProcessing::
imageMode(int mode)
{
  graphics_.imageMode(mode);
}

void
CProcessing::
textAlign(int align)
{
  graphics_.textAlign(align);
}

void
CProcessing::
textAlign(int halign, int valign)
{
  graphics_.textAlign(halign, valign);
}

void
CProcessing::
textFont(PFontP font)
{
  graphics_.textFont(font);
}

void
CProcessing::
textFont(PFontP font, int size)
{
  graphics_.textFont(font, size);
}

void
CProcessing::
text(char c, double x, double y)
{
  graphics_.text(c, x, y);
}

void
CProcessing::
text(const char *s, double x, double y)
{
  graphics_.text(s, x, y);
}

void
CProcessing::
text(StringP str, double x, double y)
{
  graphics_.text(str, x, y);
}

void
CProcessing::
text(StringP s, double x, double y, double w, double h)
{
  graphics_.text(s, x, y, w, h);
}

double
CProcessing::
red(const Pixels::Ref &ref)
{
  return ref.red()/graphics_.cFactor1_;
}

double
CProcessing::
green(const Pixels::Ref &ref)
{
  return ref.green()/graphics_.cFactor2_;
}


double
CProcessing::
blue(const Pixels::Ref &ref)
{
  return ref.blue()/graphics_.cFactor3_;
}


double
CProcessing::
alpha(const Pixels::Ref &ref)
{
  return ref.alpha()/graphics_.aFactor_;
}

double
CProcessing::
red(const color &c)
{
  return c.red()/graphics_.cFactor1_;
}

double
CProcessing::
green(const color &c)
{
  return c.green()/graphics_.cFactor2_;
}


double
CProcessing::
blue(const color &c)
{
  return c.blue()/graphics_.cFactor3_;
}


double
CProcessing::
alpha(const color &c)
{
  return c.alpha()/graphics_.aFactor_;
}

double
CProcessing::
brightness(const color &c)
{
  return c.brightness()/graphics_.aFactor_;
}

//------

double
CProcessing::
radians(double a)
{
  return M_PI*(a/180.0);
}

double
CProcessing::
degrees(double a)
{
  return 180.0*(a/M_PI);
}

//------

PFontP
CProcessing::
createFont(StringP name, int size, bool smooth, const CharArray1D &chars)
{
  return new PFont(name, size, smooth, chars);
}

PFontP
CProcessing::
loadFont(const char *name)
{
  return new PFont(new String(name));
}

PFontP
CProcessing::
loadFont(StringP name)
{
  return new PFont(name);
}

//------

PGraphicsP
CProcessing::
createGraphics(int width, int height, const char *mode)
{
  return new PGraphics(width, height, mode);
}

//------

void
CProcessing::
loadPixels()
{
  graphics_.loadPixels();
}

void
CProcessing::
updatePixels()
{
  graphics_.updatePixels();
}

//------

int
CProcessing::
second()
{
  time_t t = time(NULL);

  return t % 60;
}

int
CProcessing::
millis()
{
  CHRTime endTime = COSTime::getHRTime();

  CHRTime dtime = COSTime::diffHRTime(startTime_, endTime);

  return int(dtime.getMSecs());
}

//------

int
CProcessing::
floor(float f)
{
  if (f >= 0) return int(f + 0.001);
  else        return int(f - 1.001);
}

int
CProcessing::
ceil(float f)
{
  if (f >= 0) return int(f + 1.001);
  else        return int(f - 0.001);
}

double
CProcessing::
sq(double x)
{
  return x*x;
}

double
CProcessing::
norm(double x, double low, double high)
{
  return (x - low)/(high - low);
}

double
CProcessing::
mag(double x, double y)
{
  return sqrt(x*x + y*y);
}

double
CProcessing::
mag(double x, double y, double z)
{
  return sqrt(x*x + y*y + z*z);
}

double
CProcessing::
lerp(double value1, double value2, double amt)
{
  return value1 + (value2 - value1)*amt;
}

double
CProcessing::
map(double value, double low1, double high1, double low2, double high2)
{
  return lerp(low2, high2, norm(value, low1, high1));
}

double
CProcessing::
random(double high)
{
  return random(0.0, high);
}

double
CProcessing::
random(double low, double high)
{
  return CMathRand::randInRange(low, high);
}

void
CProcessing::
randomSeed(double seed)
{
  return CMathRand::seedRand(seed);
}

double
CProcessing::
noise(double x)
{
  return CMathGen::noise(x, x, x);
}

double
CProcessing::
noise(double x, double y)
{
  return CMathGen::noise(x, y, x + y);
}

double
CProcessing::
noise(double x, double y, double z)
{
  return CMathGen::noise(x, y, z);
}

double
CProcessing::
dist(double x1, double y1, double x2, double y2)
{
  double dx = x2 - x1;
  double dy = y2 - y1;

  return sqrt(dx*dx + dy*dy);
}

double
CProcessing::
modulus(double a, double b)
{
  return CMathGen::modulus(a, b);
}

double
CProcessing::
constrain(double v, double lower, double upper)
{
  return (v >= lower ? (v <= upper ? v : upper) : lower);
}

void
CProcessing::
pushMatrix()
{
  graphics_.pushMatrix();
}

void
CProcessing::
popMatrix()
{
  graphics_.popMatrix();
}

void
CProcessing::
perspective(double, double, double, double)
{
  // TODO
  unhandled();
}

void
CProcessing::
lights()
{
  unhandled();
}

void
CProcessing::
directionalLight(double /*v1*/, double /*v2*/, double /*v3*/,
                 double /*nx*/, double /*ny*/, double /*nz*/)
{
  unhandled();
}


void
CProcessing::
save(const char *name)
{
  graphics_.save(name);
}

void
CProcessing::
save(StringP name)
{
  graphics_.save(name);
}

StringP
CProcessing::
nf(int i, int digits)
{
  std::vector<char> str;

  str.resize(abs(digits) + 32);

  sprintf(&str[0], "%0*d", digits, i);

  return new String(&str[0]);
}

StringP
CProcessing::
nf(float f, int left, int right)
{
  std::vector<char> str;

  str.resize(abs(left) + abs(right) + 32);

  sprintf(&str[0], "%0*.*f", left, right, f);

  return new String(&str[0]);
}

void
CProcessing::
unhandled()
{
  std::cerr << "Unhandled" << std::endl;
}

//-------

void
Stroke::
setPen(QPainter *painter)
{
  Qt::PenCapStyle qcap(Qt::SquareCap);

  switch (cap) {
    case SQUARE : qcap = Qt::FlatCap  ; break;
    case ROUND  : qcap = Qt::RoundCap ; break;
    case PROJECT: qcap = Qt::SquareCap; break;
    default:                            break;
  }

  Qt::PenJoinStyle qjoin(Qt::MiterJoin);

  switch (join) {
    case BEVEL: qjoin = Qt::BevelJoin; break;
    case MITER: qjoin = Qt::MiterJoin; break;
    case ROUND: qjoin = Qt::RoundJoin; break;
    default:                           break;
  }

  QPen pen(c.qcolor(), width, Qt::SolidLine, qcap, qjoin);

  painter->setPen(pen);
}

//-------

void
Fill::
setBrush(QPainter *painter)
{
  painter->setBrush(c.qcolor());
}

//-------

Pixels::
Pixels() :
 w_(100), h_(100), format_(ARGB)
{
  image_   = QImage(w_, h_, QImage::Format_ARGB32);
  painter_ = NULL;
}

Pixels::
Pixels(const Pixels &p) :
 image_ (p.image_),
 w_     (p.w_),
 h_     (p.h_),
 format_(p.format_)
{
  painter_ = NULL;
}

Pixels::
~Pixels()
{
  delete painter_;
}

Pixels &
Pixels::
operator=(const Pixels &p)
{
  delete painter_;

  painter_ = NULL;

  image_  = p.image_;
  w_      = p.w_;
  h_      = p.h_;
  format_ = p.format_;

  return *this;
}

bool
Pixels::
load(StringP name)
{
  QImage image;

  image = QImage(name->c_str());

  if (image.isNull()) {
    std::string name1 = "data/" + name->str();

    image = QImage(name1.c_str());
  }

  if (image.isNull())
    std::cerr << "Failed to load " << name->str() << std::endl;

  setImage(image);

  return ! image_.isNull();
}

void
Pixels::
resize(int w, int h)
{
  QImage image(w, h, QImage::Format_ARGB32);

  setImage(image);
}

void
Pixels::
setImage(QImage image)
{
  delete painter_;

  painter_ = NULL;

  image_ = image;
  w_     = image_.width();
  h_     = image_.height();
}

QPainter *
Pixels::
getPainter() const
{
  if (! painter_) {
    Pixels *th = const_cast<Pixels *>(this);

    th->painter_ = new QPainter(&th->image_);
  }

  return painter_;
}

void
Pixels::
draw(QPainter *painter, double x, double y) const
{
  painter->drawImage(x, y, image_);
}

Pixels::Ref
Pixels::
operator[](int i)
{
  int y = i / w_;
  int x = i - y*w_;

  return Ref(*this, x, y);
}

void
Pixels::
setPixel(int x, int y, const color &c)
{
#if 0
  if (format_ == ARGB)
    image_.setPixel(x, y, c.qcolor().rgba());
  else
    image_.setPixel(x, y, c.qcolor().rgb());
#else
  if (format_ == ARGB)
    image_.setPixel(x, y, c.toInt());
  else
    image_.setPixel(x, y, c.toIntNoAlpha());
#endif
}

color
Pixels::
getPixel(int x, int y) const
{
  QRgb rgb = image_.pixel(x, y);

  return color(RGB, qRed(rgb)/255.0, qGreen(rgb)/255.0, qBlue(rgb)/255.0, qAlpha(rgb)/255.0);
}

void
Pixels::
fill(const color &c)
{
#if 0
  if (format_ == ARGB)
    image_.fill(c.qcolor().rgba());
  else
    image_.fill(c.qcolor().rgb());
#else
  if (format_ == ARGB)
    image_.fill(c.toInt());
  else
    image_.fill(c.toIntNoAlpha());
#endif
}

void
Pixels::
save(const char *name)
{
  image_.save(name);
}

void
Pixels::Ref::
operator=(uint c)
{
  color c1 = color::fromInt(c);

  pixels.setPixel(x, y, c1);
}

void
Pixels::Ref::
operator=(const color &c)
{
  pixels.setPixel(x, y, c);
}

void
Pixels::Ref::
assign(const Ref &ref)
{
  pixels.setPixel(x, y, ref.pixels.getPixel(ref.x, ref.y));
}

double
Pixels::Ref::
red() const
{
  return pixels.getPixel(x, y).red();
}

double
Pixels::Ref::
green() const
{
  return pixels.getPixel(x, y).green();
}

double
Pixels::Ref::
blue() const
{
  return pixels.getPixel(x, y).blue();
}

double
Pixels::Ref::
alpha() const
{
  return pixels.getPixel(x, y).alpha();
}

Pixels::Ref::
operator unsigned int() const
{
  return pixels.getPixel(x, y).toInt();
}

Pixels::Ref::
operator color() const
{
  return pixels.getPixel(x, y);
}

//-------

color::
color() :
 mode(RGB), c1(0.0), c2(0.0), c3(0.0), a(1.0)
{
}

color::
color(double gray) :
 mode(RGB), c1(gray*graphics_.cFactor1_), c2(c1), c3(c1), a(1.0)
{
  c1 = constrain(c1, 0.0, 1.0);
  c2 = constrain(c2, 0.0, 1.0);
  c3 = constrain(c3, 0.0, 1.0);
}

color::
color(double gray, double alpha) :
 mode(RGB), c1(gray*graphics_.cFactor1_), c2(c1), c3(c1), a(alpha*graphics_.aFactor_)
{
  c1 = constrain(c1, 0.0, 1.0);
  c2 = constrain(c2, 0.0, 1.0);
  c3 = constrain(c3, 0.0, 1.0);
}

color::
color(double v1, double v2, double v3) :
 mode(graphics_.colorMode_), c1(v1*graphics_.cFactor1_), c2(v2*graphics_.cFactor2_),
 c3(v3*graphics_.cFactor3_), a(1.0)
{
  c1 = constrain(c1, 0.0, 1.0);
  c2 = constrain(c2, 0.0, 1.0);
  c3 = constrain(c3, 0.0, 1.0);
}

color::
color(double v1, double v2, double v3, double alpha) :
 mode(graphics_.colorMode_), c1(v1*graphics_.cFactor1_), c2(v2*graphics_.cFactor2_),
 c3(v3*graphics_.cFactor3_), a(alpha*graphics_.aFactor_)
{
  c1 = constrain(c1, 0.0, 1.0);
  c2 = constrain(c2, 0.0, 1.0);
  c3 = constrain(c3, 0.0, 1.0);
  a  = constrain(a , 0.0, 1.0);
}

color::
color(StringP name) :
 mode(RGB)
{
  CRGBName::lookup(name->str(), &c1, &c2, &c3, &a);
}

color::
color(StringP name, double alpha) :
 mode(RGB)
{
  CRGBName::lookup(name->str(), &c1, &c2, &c3, &a);

  a = alpha;
}

color::
color(int mode1, double cc1, double cc2, double cc3, double aa) :
 mode(mode1), c1(cc1), c2(cc2), c3(cc3), a(aa)
{
}

color::
color(const color &c) :
 mode(c.mode), c1(c.c1), c2(c.c2), c3(c.c3), a(c.a)
{
}

color &
color::
operator=(const color &c)
{
  mode = c.mode;
  c1   = c.c1;
  c2   = c.c2;
  c3   = c.c3;
  a    = c.a;

  return *this;
}

double
color::
red() const
{
  if (mode == RGB)
    return c1;
  else {
    CHSB hsb(c1, c2, c3);

    CRGB rgb = CRGBUtil::HSBtoRGB(hsb);

    return rgb.getRed();
  }
}

double
color::
green() const
{
  if (mode == RGB)
    return c2;
  else {
    CHSB hsb(c1, c2, c3);

    CRGB rgb = CRGBUtil::HSBtoRGB(hsb);

    return rgb.getGreen();
  }
}

double
color::
blue() const
{
  if (mode == RGB)
    return c3;
  else {
    CHSB hsb(c1, c2, c3);

    CRGB rgb = CRGBUtil::HSBtoRGB(hsb);

    return rgb.getBlue();
  }
}

double
color::
alpha() const
{
  return a;
}

double
color::
brightness() const
{
  if (mode == RGB)
    return CRGBUtil::RGBtoHSB(CRGB(c1, c2, c3)).getBrightness();
  else
    return CHSB(c1, c2, c3).getBrightness();
}

QColor
color::
qcolor() const
{
  int qr, qg, qb, qa;

  if (mode == RGB) {
    qr = min(max(c1, 0.0), 1.0)*255;
    qg = min(max(c2, 0.0), 1.0)*255;
    qb = min(max(c3, 0.0), 1.0)*255;
  }
  else {
    CHSB hsb(c1, c2, c3);

    CRGB rgb = CRGBUtil::HSBtoRGB(hsb);

    qr = min(max(rgb.getRed  (), 0.0), 1.0)*255;
    qg = min(max(rgb.getGreen(), 0.0), 1.0)*255;
    qb = min(max(rgb.getBlue (), 0.0), 1.0)*255;
  }

  qa = min(max(a, 0.0), 1.0)*255;

  //assert(qr >= 0 && qr <= 255);
  //assert(qg >= 0 && qg <= 255);
  //assert(qb >= 0 && qb <= 255);
  //assert(qa >= 0 && qa <= 255);

  return QColor(qr, qg, qb, qa);
}

uint
color::
toInt() const
{
  uint ir, ig, ib;

  uint ia = 255*a;

  if (mode == RGB) {
    ir = 255*c1;
    ig = 255*c2;
    ib = 255*c3;
  }
  else {
    CHSB hsb(c1, c2, c3);

    CRGB rgb = CRGBUtil::HSBtoRGB(hsb);

    ir = rgb.getRed  ()*255;
    ig = rgb.getGreen()*255;
    ib = rgb.getBlue ()*255;
  }

  uint i = ((ia & 0xFF) << 24 |
            (ir & 0xFF) << 16 |
            (ig & 0xFF) << 8  |
            (ib & 0xFF)      );

  return i;
}

uint
color::
toIntNoAlpha() const
{
  uint ir, ig, ib;

  if (mode == RGB) {
    ir = 255*c1;
    ig = 255*c2;
    ib = 255*c3;
  }
  else {
    CHSB hsb(c1, c2, c3);

    CRGB rgb = CRGBUtil::HSBtoRGB(hsb);

    ir = rgb.getRed  ()*255;
    ig = rgb.getGreen()*255;
    ib = rgb.getBlue ()*255;
  }

  uint i = ((     0xFF) << 24 |
            (ir & 0xFF) << 16 |
            (ig & 0xFF) << 8  |
            (ib & 0xFF)      );

  return i;
}

color
color::
fromInt(uint c)
{
  uint a = (c >> 24) & 0xFF;
  uint r = (c >> 16) & 0xFF;
  uint g = (c >>  8) & 0xFF;
  uint b = (c      ) & 0xFF;

  return color(RGB, r/255.0, g/255.0, b/255.0, a/255.0);
}

//------

PImage::
PImage() :
 width  (0),
 height (0),
 format (ARGB),
 hasMask(false)
{
  pixels.setFormat(format);
}

PImage::
PImage(int width1, int height1, int format1) :
 width  (width1),
 height (height1),
 format (format1),
 hasMask(false)
{
  pixels.setFormat(format);

  pixels.resize(width, height);
}

PImage::
PImage(StringP name) :
 format (ARGB),
 hasMask(false)
{
  pixels.setFormat(format);

  pixels.load(name);

  width  = pixels.getWidth ();
  height = pixels.getHeight();
}

PImage::
PImage(const char *name) :
 format (ARGB),
 hasMask(false)
{
  pixels.setFormat(format);

  pixels.load(new String(name));

  width  = pixels.getWidth ();
  height = pixels.getHeight();
}

color
PImage::
get(int x, int y) const
{
  return pixels.getPixel(x, y);
}

void
PImage::
loadPixels()
{
  // TODO: This is a NOOP
  //unhandled();
}

void
PImage::
updatePixels()
{
  // TODO: This is a NOOP
  //unhandled();
}

void
PImage::
resize(int width1, int height1)
{
  QImage qimage = getImage().scaled(QSize(width1, height1));

  width  = width1;
  height = height1;

  pixels.setImage(qimage);
}

void
PImage::
mask(PImageP imask)
{
  maskPixels = imask->pixels;
  hasMask    = true;
}

QImage
PImage::
getImage() const
{
  if (hasMask)
    return CQImageFilter::mask(pixels.getImage(), maskPixels.getImage());
  else
    return pixels.getImage();
}

//------

PShape::
PShape() :
 obj_(NULL), styleEnabled_(true)
{
}

PShape::
PShape(StringP name) :
 obj_(NULL), styleEnabled_(true)
{
#ifdef USE_SVG
  if (! getSVG()->read(name->str()))
    return;

  obj_ = getSVG()->getRoot();
#endif
}

PShapeP
PShape::
getChild(const char *name)
{
  return getChild(StringP(new String(name)));
}

PShapeP
PShape::
getChild(StringP name)
{
#ifdef USE_SVG
  std::vector<CSVGObject *> children;

  if (obj_)
    obj_->getAllChildrenOfId(name->str(), children);

  PShape *shape { nullptr };

  if (! children.empty()) {
    shape = new PShape;

    shape->obj_ = children[0];
  }

  return PShapeP(shape);
#else
  return PShapeP();
#endif
}

void
PShape::
enableStyle()
{
  styleEnabled_ = true;
}

void
PShape::
disableStyle()
{
  styleEnabled_ = false;
}

void
PShape::
draw(PGraphicsP graphics, double x, double y) const
{
#ifdef USE_SVG
  draw(graphics, x, y, getSVG()->getWidth(), getSVG()->getHeight());
#endif
}

void
PShape::
draw(PGraphicsP graphics, double x, double y, double w, double h) const
{
#ifdef USE_SVG
  if (! obj_) return;

  double x1, y1, x2, y2;

  graphics->matrix2_.multiplyPoint(x    , y    , &x1, &y1);
  graphics->matrix2_.multiplyPoint(x + w, y + h, &x2, &y2);

  svgRenderer_->setSize(fabs(x2 - x1), fabs(y2 - y1));

  svgRenderer_->beginDraw();

  CSVGBlock *block = getSVG()->getRoot();

  svgRenderer_->setDataRange(block->getXMin(), block->getYMin(),
                             block->getXMax(), block->getYMax());

  //svgRenderer_->setViewMatrix(graphics->matrix2_);

  if (! styleEnabled_) {
    CRGBA rgba;

    //svgRender_->setStyleEnabled(false);

    graphics->stroke_.c.getRGBA(rgba);

    svgRenderer_->setStrokeColor(rgba);
    svgRenderer_->setLineWidth(graphics->stroke_.width);

    graphics->fill_.c.getRGBA(rgba);

    svgRenderer_->setFillColor(rgba);

    //svgRender_->draw(obj_);
  }
  else {
    //svgRender_->setStyleEnabled(true);

    //svgRender_->draw(obj_);
  }

  svgRenderer_->endDraw();

  //svgRenderer_->paint(graphics->pixels.getPainter(), x1, y1);
#endif
}

//------

PGraphics::
PGraphics() :
 width(100), height(100), mode_(P2D)
{
  bgType_ = COLOR_BG;

  colorMode_ = RGB;
  cFactor1_  = 1.0/255.0;
  cFactor2_  = 1.0/255.0;
  cFactor3_  = 1.0/255.0;
  aFactor_   = 1.0/255.0;

  ellipseMode_ = CENTER;
  imageMode_   = CORNER;
  rectMode_    = CORNER;

  font_ = new PFont;

  tinted_ = false;

  resetMatrix();

  projection_ = 100.0;

  pixels.resize(width, height);
}

PGraphics::
PGraphics(int width1, int height1, const char *mode) :
 width(width1), height(height1), mode_(mode)
{
  bgType_ = COLOR_BG;

  colorMode_ = RGB;
  cFactor1_  = 1.0/255.0;
  cFactor2_  = 1.0/255.0;
  cFactor3_  = 1.0/255.0;
  aFactor_   = 1.0/255.0;

  ellipseMode_ = CENTER;
  imageMode_   = CORNER;
  rectMode_    = CORNER;

  font_ = new PFont;

  tinted_ = false;

  resetMatrix();

  projection_ = 100.0;

  pixels.resize(width, height);
}

PGraphics::
PGraphics(const PGraphics &graphics) :
 PObject    (graphics),
 width      (graphics.width),
 height     (graphics.height),
 mode_      (graphics.mode_),
 bgType_    (graphics.bgType_),
 bg_        (graphics.bg_),
 bgImage_   (graphics.bgImage_),
 stroke_    (graphics.stroke_),
 fill_      (graphics.fill_),
 font_      (graphics.font_),
 colorMode_ (graphics.colorMode_),
 cFactor1_  (graphics.cFactor1_),
 cFactor2_  (graphics.cFactor2_),
 cFactor3_  (graphics.cFactor3_),
 aFactor_   (graphics.aFactor_),
 matrix2_   (graphics.matrix2_),
 matrix3_   (graphics.matrix3_),
 projection_(graphics.projection_),
 pixels     (graphics.pixels) {
}

PGraphics &
PGraphics::
operator=(const PGraphics &graphics)
{
  width       = graphics.width;
  height      = graphics.height;
  mode_       = graphics.mode_;
  bgType_     = graphics.bgType_;
  bg_         = graphics.bg_;
  bgImage_    = graphics.bgImage_;
  stroke_     = graphics.stroke_;
  fill_       = graphics.fill_;
  font_       = graphics.font_;
  colorMode_  = graphics.colorMode_;
  cFactor1_   = graphics.cFactor1_;
  cFactor2_   = graphics.cFactor2_;
  cFactor3_   = graphics.cFactor3_;
  aFactor_    = graphics.aFactor_,
  matrix2_    = graphics.matrix2_;
  matrix3_    = graphics.matrix3_;
  projection_ = graphics.projection_;
  pixels      = graphics.pixels;

  return *this;
}

void
PGraphics::
resize(int width1, int height1)
{
  width   = width1;
  height  = height1;

  pixels.resize(width, height);

  fillBackground();
}

void
PGraphics::
beginDraw()
{
  // set background ?

  resetMatrix();

  noTint();
}

void
PGraphics::
endDraw()
{
}

void
PGraphics::
colorMode(int mode)
{
  colorMode_ = mode;
}

void
PGraphics::
colorMode(int mode, double c)
{
  colorMode_ = mode;
  cFactor1_  = 1.0/c;
  cFactor2_  = 1.0/c;
  cFactor3_  = 1.0/c;
  aFactor_   = 1.0/c;
}

void
PGraphics::
colorMode(int mode, double c1, double c2, double c3)
{
  colorMode_ = mode;
  cFactor1_  = 1.0/c1;
  cFactor2_  = 1.0/c2;
  cFactor3_  = 1.0/c3;
}

void
PGraphics::
colorMode(int mode, double c1, double c2, double c3, double a)
{
  colorMode_ = mode;
  cFactor1_  = 1.0/c1;
  cFactor2_  = 1.0/c2;
  cFactor3_  = 1.0/c3;
  aFactor_   = 1.0/a;
}

void
PGraphics::
background(double gray)
{
  bg_     = color(gray);
  bgType_ = COLOR_BG;

  fillBackground();
}

void
PGraphics::
background(double gray, double alpha)
{
  bg_     = color(gray, alpha);
  bgType_ = COLOR_BG;

  fillBackground();
}

void
PGraphics::
background(double c1, double c2, double c3)
{
  bg_     = color(c1, c2, c3);
  bgType_ = COLOR_BG;

  fillBackground();
}

void
PGraphics::
background(double c1, double c2, double c3, double alpha)
{
  bg_     = color(c1, c2, c3, alpha);
  bgType_ = COLOR_BG;

  fillBackground();
}

void
PGraphics::
background(StringP name)
{
  bg_     = color(name);
  bgType_ = COLOR_BG;

  fillBackground();
}

void
PGraphics::
background(StringP name, double alpha)
{
  bg_     = color(name, alpha);
  bgType_ = COLOR_BG;

  fillBackground();
}

void
PGraphics::
background(const color &c)
{
  bg_     = c;
  bgType_ = COLOR_BG;

  fillBackground();
}

void
PGraphics::
background(const color &c, double alpha)
{
  bg_     = c;
  bgType_ = COLOR_BG;

  bg_.setAlpha(alpha*aFactor_);

  fillBackground();
}

void
PGraphics::
background(PImageP image)
{
  bgImage_ = image;
  bgType_  = IMAGE_BG;

  fillBackground();
}

void
PGraphics::
fillBackground()
{
  if (bgType_ == IMAGE_BG)
    image(bgImage_, 0, 0);
  else
    pixels.fill(bg_);
}

void
PGraphics::
translate(double dx, double dy)
{
  translate(dx, dy, 0.0);
}

void
PGraphics::
translate(double dx, double dy, double dz)
{
  matrix2_ *= CMatrix2D::translation(dx, dy);
  matrix3_ *= CMatrix3D::translation(dx, dy, dz);

  updateTransform();
}

void
PGraphics::
rotate(double angle)
{
  matrix2_ *= CMatrix2D::rotation(angle);

  updateTransform();
}

void
PGraphics::
rotateX(double angle)
{
  matrix3_ *= CMatrix3D::rotation(CMathGen::X_AXIS_3D, angle);

  updateTransform();
}

void
PGraphics::
rotateY(double angle)
{
  matrix3_ *= CMatrix3D::rotation(CMathGen::Y_AXIS_3D, angle);

  updateTransform();
}

void
PGraphics::
rotateZ(double angle)
{
  matrix3_ *= CMatrix3D::rotation(CMathGen::Z_AXIS_3D, angle);

  updateTransform();
}

void
PGraphics::
scale(double s)
{
  matrix2_ *= CMatrix2D::scale(s);
  matrix3_ *= CMatrix3D::scale(s);

  updateTransform();
}

void
PGraphics::
scale(double sx, double sy)
{
  matrix2_ *= CMatrix2D::scale(sx, sy);
  matrix3_ *= CMatrix3D::scale(sx, sy, 1);

  updateTransform();
}

void
PGraphics::
resetMatrix()
{
  matrix2_.setIdentity();
  matrix3_.setIdentity();

  updateTransform();
}

void
PGraphics::
updateTransform()
{
  double m[6];

  matrix2_.getValues(m, 6);

  pixels.getPainter()->setWorldTransform(QTransform(m[0], m[1], m[2], m[3], m[4], m[5]));
}

void
PGraphics::
stroke(double gray)
{
  stroke_.c = color(gray);

  stroke_.active = true;
}

void
PGraphics::
stroke(double gray, double alpha)
{
  stroke_.c = color(gray, alpha);

  stroke_.active = true;
}

void
PGraphics::
stroke(double c1, double c2, double c3)
{
  stroke_.c = color(c1, c2, c3);

  stroke_.active = true;
}

void
PGraphics::
stroke(double c1, double c2, double c3, double alpha)
{
  stroke_.c = color(c1, c2, c3, alpha);

  stroke_.active = true;
}

void
PGraphics::
stroke(StringP name)
{
  stroke_.c = color(name);

  stroke_.active = true;
}

void
PGraphics::
stroke(StringP name, double alpha)
{
  stroke_.c = color(name, alpha);

  stroke_.active = true;
}

void
PGraphics::
stroke(const color &c)
{
  stroke_.c = c;

  stroke_.active = true;
}

void
PGraphics::
stroke(const color &c, double alpha)
{
  stroke_.c = c;

  stroke_.c.setAlpha(alpha*aFactor_);

  stroke_.active = true;
}

void
PGraphics::
noStroke()
{
  stroke_.active = false;
}

void
PGraphics::
fill(double gray)
{
  fill_.c = color(gray);

  fill_.active = true;
}

void
PGraphics::
fill(double gray, double alpha)
{
  fill_.c = color(gray, alpha);

  fill_.active = true;
}

void
PGraphics::
fill(double c1, double c2, double c3)
{
  fill_.c = color(c1, c2, c3);

  fill_.active = true;
}

void
PGraphics::
fill(double c1, double c2, double c3, double alpha)
{
  fill_.c = color(c1, c2, c3, alpha);

  fill_.active = true;
}

void
PGraphics::
fill(StringP name)
{
  fill_.c = color(name);

  fill_.active = true;
}

void
PGraphics::
fill(StringP name, double alpha)
{
  fill_.c = color(name, alpha);

  fill_.active = true;
}

void
PGraphics::
fill(const color &c)
{
  fill_.c = c;

  fill_.active = true;
}

void
PGraphics::
fill(const color &c, double alpha)
{
  fill_.c = c;

  fill_.c.setAlpha(alpha*aFactor_);

  fill_.active = true;
}

void
PGraphics::
noFill()
{
  fill_.active = false;
}

void
PGraphics::
strokeWeight(double width)
{
  stroke_.width = width;
}

void
PGraphics::
strokeCap(int cap)
{
  stroke_.cap = cap;
}

void
PGraphics::
strokeJoin(int join)
{
  stroke_.join = join;
}

void
PGraphics::
line(double x1, double y1, double x2, double y2)
{
  if (stroke_.active) {
    stroke_.setPen(pixels.getPainter());

    pixels.getPainter()->drawLine(x1, y1, x2, y2);
  }
}

void
PGraphics::
line(double x1, double y1, double z1, double x2, double y2, double z2)
{
  double px1, py1, pz1, px2, py2, pz2;

  matrix3_.multiplyPoint(x1, y1, z1, &px1, &py1, &pz1);
  matrix3_.multiplyPoint(x2, y2, z2, &px2, &py2, &pz2);

  if (project(px1, py1, pz1, &px1, &py1) &&
      project(px2, py2, pz2, &px2, &py2))
    line(px1, py1, px2, py2);
}

void
PGraphics::
triangle(double x1, double y1, double x2, double y2, double x3, double y3)
{
  if (stroke_.active)
    stroke_.setPen(pixels.getPainter());
  else
    pixels.getPainter()->setPen(QPen(Qt::NoPen));

  if (fill_.active)
    fill_.setBrush(pixels.getPainter());
  else
    pixels.getPainter()->setBrush(QBrush(Qt::NoBrush));

  QPolygon poly(3);

  poly.setPoint(0, x1, y1);
  poly.setPoint(1, x2, y2);
  poly.setPoint(2, x3, y3);

  pixels.getPainter()->drawPolygon(poly);
}

void
PGraphics::
quad(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{
  QPolygon poly(4);

  poly.setPoint(0, x1, y1);
  poly.setPoint(1, x2, y2);
  poly.setPoint(2, x3, y3);
  poly.setPoint(3, x4, y4);

  if (fill_.active)
    fill_.setBrush(pixels.getPainter());
  else
    pixels.getPainter()->setBrush(QBrush(Qt::NoBrush));

  if (stroke_.active)
    stroke_.setPen(pixels.getPainter());
  else
    pixels.getPainter()->setPen(QPen(Qt::NoPen));

  pixels.getPainter()->drawPolygon(poly);
}

void
PGraphics::
curve(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{
  QPainterPath path;

  path.moveTo(x1, y1);

  path.cubicTo(x2, y2, x3, y3, x4, y4);

//if (fill_.active) {
//  fill_.setBrush(pixels.getPainter());

//  pixels.getPainter()->fillPath(path, pixels.getPainter()->brush());
//}

  if (stroke_.active) {
    stroke_.setPen(pixels.getPainter());

    pixels.getPainter()->strokePath(path, pixels.getPainter()->pen());
  }
}

void
PGraphics::
rect(double p1, double p2, double p3, double p4)
{
  double x, y, w, h;

  if      (rectMode_ == CORNER) {
    x = p1; y = p2;
    w = p3; h = p4;
  }
  else if (rectMode_ == CORNERS) {
    x = std::min(p1, p3); y = std::min(p2, p4);
    w = abs(p3 - p1)    ; h = abs(p4 - p2);
  }
  else if (rectMode_ == CENTER) {
    x = p1 - p3/2; y = p2 = p4/2;
    w = p3       ; h = p4;
  }
  else if (rectMode_ == RADIUS) {
    x = p1 - p3; y = p2 - p4;
    w = p3*2   ; h = p4*2;
  }
  else
    assert(false);

  if (fill_.active)
    fill_.setBrush(pixels.getPainter());
  else
    pixels.getPainter()->setBrush(QBrush(Qt::NoBrush));

  if (stroke_.active)
    stroke_.setPen(pixels.getPainter());
  else
    pixels.getPainter()->setPen(QPen(Qt::NoPen));

  pixels.getPainter()->drawRect(QRect(x, y, w, h));

//pixels.getPainter()->fillRect(QRect(x, y, w, h), QBrush(fill_.c.qcolor()));
}

void
PGraphics::
ellipse(double p1, double p2, double p3, double p4)
{
  double x, y, w, h;

  if      (ellipseMode_ == CORNER) {
    x = p1; y = p2;
    w = p3; h = p4;
  }
  else if (ellipseMode_ == CORNERS) {
    x = std::min(p1, p3); y = std::min(p2, p4);
    w = fabs(p3 - p1)   ; h = fabs(p4 - p2);
  }
  else if (ellipseMode_ == CENTER) {
    x = p1 - p3/2; y = p2 - p4/2;
    w = p3       ; h = p4;
  }
  else if (ellipseMode_ == RADIUS) {
    x = p1 - p3; y = p2 - p4;
    w = p3*2   ; h = p4*2;
  }
  else
    assert(false);

  if (fill_.active)
    fill_.setBrush(pixels.getPainter());
  else
    pixels.getPainter()->setBrush(QBrush(Qt::NoBrush));

  //QPainterPath path;

  //path.addEllipse(QRect(x, y, w, h));

  //pixels.getPainter()->fillPath(path, QBrush(fill_.c.qcolor()));

  if (stroke_.active)
    stroke_.setPen(pixels.getPainter());
  else
    pixels.getPainter()->setPen(QPen(Qt::NoPen));

  pixels.getPainter()->drawEllipse(QRect(x, y, w, h));
}

void
PGraphics::
ellipseMode(int mode)
{
  ellipseMode_ = mode;
}

void
PGraphics::
arc(double p1, double p2, double p3, double p4, double angle1, double angle2)
{
  double x, y, w, h;

  if      (ellipseMode_ == CORNER) {
    x = p1; y = p2;
    w = p3; h = p4;
  }
  else if (ellipseMode_ == CORNERS) {
    x = std::min(p1, p3); y = std::min(p2, p4);
    w = fabs(p3 - p1)   ; h = fabs(p4 - p2);
  }
  else if (ellipseMode_ == CENTER) {
    x = p1 - p3/2; y = p2 - p4/2;
    w = p3       ; h = p4;
  }
  else if (ellipseMode_ == RADIUS) {
    x = p1 - p3; y = p2 - p4;
    w = p3*2   ; h = p4*2;
  }
  else
    assert(false);

  angle1 = 180.0*angle1/M_PI;
  angle2 = 180.0*angle2/M_PI;

  QPainterPath path;

  path.moveTo(x + w/2, y + h/2);

  //path.arcMoveTo(x, y, width, height, angle1);

  path.arcTo(x, y, width, height, angle1, angle2 - angle1);

  //path.lineTo(x + w/2, y + h/2);

  path.closeSubpath();

  if (graphics_.fill_.active)
    graphics_.pixels.getPainter()->fillPath(path, QBrush(graphics_.fill_.c.qcolor()));

  if (graphics_.stroke_.active)
    graphics_.pixels.getPainter()->strokePath(path, QPen(graphics_.stroke_.c.qcolor()));
}

void
PGraphics::
bezier(double x1, double y1, double cx1, double cy1, double cx2, double cy2, double x2, double y2)
{
  QPainterPath path;

  path.moveTo(x1, y1);

  path.cubicTo(cx1, cy1, cx2, cy2, x2, y2);

  if (graphics_.fill_.active)
    graphics_.pixels.getPainter()->fillPath(path, QBrush(graphics_.fill_.c.qcolor()));

  if (graphics_.stroke_.active)
    graphics_.pixels.getPainter()->strokePath(path, QPen(graphics_.stroke_.c.qcolor()));
}

void
PGraphics::
image(PImageP i, double x, double y)
{
  QImage qimage = i->getImage();

  if      (imageMode_ == CORNER) {
  }
  else if (imageMode_ == CENTER) {
    x -= i->width /2;
    y -= i->height/2;
  }

  if (tinted_) {
    CRGBA rgba; tintColor_.getRGBA(rgba);

    QImage qimage1 = CQImageFilter::tint(qimage, rgba);

    pixels.getPainter()->drawImage(x, y, qimage1);
  }
  else
    pixels.getPainter()->drawImage(x, y, qimage);
}

void
PGraphics::
image(PImageP i, double x, double y, double w, double h)
{
  QImage qimage = i->getImage();

  QRect rect;

  if      (imageMode_ == CORNER)
    rect = QRect(x, y, w, h);
  else if (imageMode_ == CENTER)
    rect = QRect(x - w/2, y - h/2, w, h);

  if (tinted_) {
    CRGBA rgba; tintColor_.getRGBA(rgba);

    QImage qimage1 = CQImageFilter::tint(qimage, rgba);

    pixels.getPainter()->drawImage(rect, qimage1);
  }
  else
    pixels.getPainter()->drawImage(rect, qimage);
}

void
PGraphics::
image(PGraphicsP g, double x, double y)
{
  QImage qimage = g->pixels.getImage();

  if      (imageMode_ == CORNER) {
  }
  else if (imageMode_ == CENTER) {
    x -= qimage.width ()/2;
    y -= qimage.height()/2;
  }

  if (tinted_) {
    CRGBA rgba; tintColor_.getRGBA(rgba);

    QImage qimage1 = CQImageFilter::tint(qimage, rgba);

    pixels.getPainter()->drawImage(x, y, qimage1);
  }
  else
    pixels.getPainter()->drawImage(x, y, qimage);
}

void
PGraphics::
image(PGraphicsP g, double x, double y, double w, double h)
{
  QImage qimage = g->pixels.getImage();

  QRect rect;

  if      (imageMode_ == CORNER)
    rect = QRect(x, y, w, h);
  else if (imageMode_ == CENTER)
    rect = QRect(x - w/2, y - h/2, w, h);

  if (tinted_) {
    CRGBA rgba; tintColor_.getRGBA(rgba);

    QImage qimage1 = CQImageFilter::tint(qimage, rgba);

    pixels.getPainter()->drawImage(rect, qimage1);
  }
  else
    pixels.getPainter()->drawImage(rect, qimage);
}

void
PGraphics::
copy(PImageP src, int x, int y, int w, int h, int dx, int dy, int dw, int dh)
{
  QImage qimage = src->getImage();

  QRect rect(x, y, w, h);

  QImage qimage1 = qimage.copy(rect);

  QRect drect(dx, dy, dw, dh);

  pixels.getPainter()->drawImage(drect, qimage1);
}

void
PGraphics::
copy(PGraphicsP src, int x, int y, int w, int h, int dx, int dy, int dw, int dh)
{
  QImage qimage = src->pixels.getImage();

  QRect rect(x, y, w, h);

  QImage qimage1 = qimage.copy(rect);

  QRect drect(dx, dy, dw, dh);

  pixels.getPainter()->drawImage(drect, qimage1);
}

void
PGraphics::
set(int x, int y, const color &c)
{
  pixels.getPainter()->setPen(c.qcolor());

  pixels.getPainter()->drawPoint(x, y);
}

void
PGraphics::
set(int x, int y, uint pixel)
{
  color c = color::fromInt(pixel);

  pixels.getPainter()->setPen(c.qcolor());

  pixels.getPainter()->drawPoint(x, y);
}

color
PGraphics::
get(int x, int y)
{
  return pixels.getPixel(x, y);
}

void
PGraphics::
tint(double gray, double alpha)
{
  tint(color(gray, alpha));
}

void
PGraphics::
tint(const color &c)
{
  tintColor_ = c;
  tinted_    = true;
}

void
PGraphics::
noTint()
{
  tinted_ = false;
}

void
PGraphics::
imageMode(int mode)
{
  imageMode_ = mode;
}

void
PGraphics::
textAlign(int align)
{
  if      (align == LEFT)
    textAlign(LEFT, CENTER);
  else if (align == CENTER)
    textAlign(CENTER, CENTER);
  else if (align == RIGHT)
    textAlign(RIGHT, CENTER);
  else
    CProcessing::unhandled();
}

void
PGraphics::
textAlign(int halign, int valign)
{
  textHAlign_ = halign;
  textVAlign_ = valign;
}

void
PGraphics::
textFont(PFontP font)
{
  font_ = font;
}

void
PGraphics::
textFont(PFontP font, int size)
{
  font_ = font;

  font_->setSize(size);
}

void
PGraphics::
text(char c, double x, double y)
{
  StringP s = new String(&c, 1);

  text(s, x, y);
}

void
PGraphics::
text(const char *s, double x, double y)
{
  StringP ss = new String(s);

  text(ss, x, y);
}

void
PGraphics::
text(StringP s, double x, double y)
{
  QString text(s->c_str());

  QFont qfont = font_->getQFont();

  delete path_;

  path_ = new QPainterPath();

  path_->addText(QPoint(0, 0), qfont, text);

  QRectF rect = path_->boundingRect();

  if      (textHAlign_ == LEFT  ) x -= rect.x();
  else if (textHAlign_ == CENTER) x -= rect.x() + rect.width ()/2;
  else if (textHAlign_ == RIGHT ) x -= rect.x() + rect.width ();

  if      (textVAlign_ == BOTTOM) y -= rect.y();
  else if (textVAlign_ == CENTER) y -= rect.y() + rect.height()/2;
  else if (textVAlign_ == TOP   ) y -= rect.y() + rect.height();

  delete path_;

  path_ = new QPainterPath();

  path_->addText(QPoint(x, y), qfont, text);

  if (font_->getSmooth())
    pixels.getPainter()->setRenderHint(QPainter::Antialiasing);

  if (fill_.active) {
    pixels.getPainter()->fillPath(*path_, fill_.c.qcolor());
  }

//if (stroke_.active) {
//  stroke_.setPen(pixels.getPainter());

//  pixels.getPainter()->drawPath(*path_);
//}
}

void
PGraphics::
text(StringP s, double x, double y, double w, double h)
{
  assert(w > 0 && h > 0);

  text(s, x, y);
}

void
PGraphics::
rectMode(int mode)
{
  rectMode_ = mode;
}

void
PGraphics::
box(double size)
{
  static double cube_normal[6][3] = {
    {-1.0,  0.0,  0.0},
    { 0.0,  1.0,  0.0},
    { 1.0,  0.0,  0.0},
    { 0.0, -1.0,  0.0},
    { 0.0,  0.0,  1.0},
    { 0.0,  0.0, -1.0}
  };

  static int cube_faces[6][4] = {
    {0, 1, 2, 3},
    {3, 2, 6, 7},
    {7, 6, 5, 4},
    {4, 5, 1, 0},
    {5, 6, 2, 1},
    {7, 4, 0, 3}
  };

  if (drawMode == P3D) {
    double v[8][3];

    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] =  size / 2;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] =  size / 2;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] =  size / 2;

    for (int i = 5; i >= 0; --i) {
      glBegin(GL_POLYGON);

      glNormal3dv(&cube_normal[i][0]);

      glVertex3dv(&v[cube_faces[i][0]][0]);
      glVertex3dv(&v[cube_faces[i][1]][0]);
      glVertex3dv(&v[cube_faces[i][2]][0]);
      glVertex3dv(&v[cube_faces[i][3]][0]);

      glEnd();
    }
  }
  else {
    CPoint3D p1(-size, -size,  size);
    CPoint3D p2( size, -size,  size);
    CPoint3D p3( size,  size,  size);
    CPoint3D p4(-size,  size,  size);
    CPoint3D p5(-size, -size, -size);
    CPoint3D p6( size, -size, -size);
    CPoint3D p7( size,  size, -size);
    CPoint3D p8(-size,  size, -size);

    line(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
    line(p2.x, p2.y, p2.z, p3.x, p3.y, p3.z);
    line(p3.x, p3.y, p3.z, p4.x, p4.y, p4.z);
    line(p4.x, p4.y, p4.z, p1.x, p1.y, p1.z);

    line(p5.x, p5.y, p5.z, p6.x, p6.y, p6.z);
    line(p6.x, p6.y, p6.z, p7.x, p7.y, p7.z);
    line(p7.x, p7.y, p7.z, p8.x, p8.y, p8.z);
    line(p8.x, p8.y, p8.z, p5.x, p5.y, p5.z);

    line(p1.x, p1.y, p1.z, p5.x, p5.y, p5.z);
    line(p2.x, p2.y, p2.z, p6.x, p6.y, p6.z);
    line(p3.x, p3.y, p3.z, p7.x, p7.y, p7.z);
    line(p4.x, p4.y, p4.z, p8.x, p8.y, p8.z);
  }
}

bool
PGraphics::
project(double x, double y, double z, double *px, double *py)
{
  if (z > 0) return false;

  double factor = -projection_/z;

  *px = x*factor;
  *py = y*factor;

  return true;
}

void
PGraphics::
filter(int mode)
{
  if      (mode == BLUR) {
    // gausian blur
    QImage src = pixels.getImage();

    QImage dst = CQImageFilter::gaussianBlur(src, 1, 1, 2, 2);

    pixels.setImage(dst);
  }
  else if (mode == ERODE) {
    QImage src = pixels.getImage();

    QImage dst = CQImageFilter::erode(src);

    pixels.setImage(dst);
  }
  else if (mode == DILATE) {
    QImage src = pixels.getImage();

    QImage dst = CQImageFilter::dilate(src);

    pixels.setImage(dst);
  }
  else
    CProcessing::unhandled();
}

void
PGraphics::
smooth()
{
}

void
PGraphics::
noSmooth()
{
}

void
PGraphics::
beginShape(int mode)
{
  shapeMode_ = mode;

  shapePointList_.clear();
}

void
PGraphics::
vertex(double x, double y)
{
  shapePointList_.push_back(ShapePoint(x, y));
}

void
PGraphics::
curveVertex(double x, double y)
{
  shapePointList_.push_back(ShapePoint(x, y));
}

void
PGraphics::
bezierVertex(double x1, double y1, double x2, double y2, double x3, double y3)
{
  shapePointList_.push_back(ShapePoint(x1, y1, x2, y2, x3, y3));
}

double
PGraphics::
bezierPoint(double a, double b, double c, double d, double t)
{
  double u = (1.0 - t);

  double tt  = t*t;
  double ttt = tt*t;

  double uu  = u*u;
  double uuu = uu*u;

  return a*uuu + 3.0*b*t*uu + 3.0*c*tt*u + d*ttt;
}

void
PGraphics::
endShape(int closeType)
{
  ShapePointList::const_iterator p1, p2, p3;

  if   (shapeMode_ == POLYGON) {
    delete path_;

    path_ = new QPainterPath();

    for (p1 = shapePointList_.begin(), p2 = shapePointList_.end(); p1 != p2; ++p1) {
      const ShapePoint &point = *p1;

      if (point.type == ShapePoint::POINT) {
        if (path_->isEmpty())
          path_->moveTo(point.x1, point.y1);
        else
          path_->lineTo(point.x1, point.y1);
      }
      else
        path_->cubicTo(point.x1, point.y1, point.x2, point.y2, point.x3, point.y3);
    }

    if (closeType == CLOSE)
      path_->closeSubpath();

    if (stroke_.active)
      stroke_.setPen(pixels.getPainter());
    else
      pixels.getPainter()->setPen(QPen(Qt::NoPen));

    if (fill_.active)
      fill_.setBrush(pixels.getPainter());
    else
      pixels.getPainter()->setBrush(QBrush(Qt::NoBrush));

    //pixels.getPainter()->fillPath(*path_, fill_.c.qcolor());

    pixels.getPainter()->drawPath(*path_);
  }
  else if (shapeMode_ == POINTS) {
    pixels.getPainter()->setPen(stroke_.c.qcolor());

    for (p1 = shapePointList_.begin(), p2 = shapePointList_.end(); p1 != p2; ++p1) {
      const ShapePoint &point = *p1;

      pixels.getPainter()->drawPoint(point.x1, point.y1);
    }
  }
  else if (shapeMode_ == LINES) {
    pixels.getPainter()->setPen(stroke_.c.qcolor());

    for (p1 = shapePointList_.begin(), p2 = shapePointList_.end(); p1 != p2; ++p1) {
      const ShapePoint &point1 = *p1;

      ++p1; if (p1 == p2) break;

      const ShapePoint &point2 = *p1;

      pixels.getPainter()->drawLine(point1.x1, point1.y1, point2.x1, point2.y1);
    }
  }
  else if (shapeMode_ == LINE_STRIP) {
    pixels.getPainter()->setPen(stroke_.c.qcolor());

    int num_lines = 0;

    if (shapePointList_.size() >= 2)
      num_lines = shapePointList_.size() - 1;

    p2 = shapePointList_.begin();
    p3 = shapePointList_.end  ();

    for (int i = 0; i < num_lines; ++i) {
      p1 = p2++;

      const ShapePoint &point1 = *p1;
      const ShapePoint &point2 = *p2;

      pixels.getPainter()->drawLine(point1.x1, point1.y1, point2.x1, point2.y1);
    }
  }
  else if (shapeMode_ == TRIANGLES) {
    if (stroke_.active)
      stroke_.setPen(pixels.getPainter());
    else
      pixels.getPainter()->setPen(QPen(Qt::NoPen));

    if (fill_.active)
      fill_.setBrush(pixels.getPainter());
    else
      pixels.getPainter()->setBrush(QBrush(Qt::NoBrush));

    for (p1 = shapePointList_.begin(), p2 = shapePointList_.end(); p1 != p2; ++p1) {
      const ShapePoint &point1 = *p1;

      ++p1; if (p1 == p2) break;

      const ShapePoint &point2 = *p1;

      ++p1; if (p1 == p2) break;

      const ShapePoint &point3 = *p1;

      QPolygon poly(3);

      poly.setPoint(0, point1.x1, point1.y1);
      poly.setPoint(1, point2.x1, point2.y1);
      poly.setPoint(2, point3.x1, point3.y1);

      pixels.getPainter()->drawPolygon(poly);
    }
  }
  else if (shapeMode_ == QUADS) {
    if (stroke_.active)
      stroke_.setPen(pixels.getPainter());
    else
      pixels.getPainter()->setPen(QPen(Qt::NoPen));

    if (fill_.active)
      fill_.setBrush(pixels.getPainter());
    else
      pixels.getPainter()->setBrush(QBrush(Qt::NoBrush));

    for (p1 = shapePointList_.begin(), p2 = shapePointList_.end(); p1 != p2; ++p1) {
      const ShapePoint &point1 = *p1;

      ++p1; if (p1 == p2) break;

      const ShapePoint &point2 = *p1;

      ++p1; if (p1 == p2) break;

      const ShapePoint &point3 = *p1;

      ++p1; if (p1 == p2) break;

      const ShapePoint &point4 = *p1;

      QPolygon poly(4);

      poly.setPoint(0, point1.x1, point1.y1);
      poly.setPoint(1, point2.x1, point2.y1);
      poly.setPoint(2, point3.x1, point3.y1);
      poly.setPoint(3, point4.x1, point4.y1);

      pixels.getPainter()->drawPolygon(poly);
    }
  }
  else
    CProcessing::unhandled();
}

void
PGraphics::
shape(PShapeP shape, double x, double y)
{
  shape->draw(this, x, y);
}

void
PGraphics::
shape(PShapeP shape, double x, double y, double w, double h)
{
  shape->draw(this, x, y, w, h);
}

void
PGraphics::
pushMatrix()
{
  matrix2Stack_.push_back(matrix2_);
  matrix3Stack_.push_back(matrix3_);
}

void
PGraphics::
popMatrix()
{
  if (! matrix2Stack_.empty()) {
    matrix2_ = matrix2Stack_.back();
    matrix3_ = matrix3Stack_.back();

    matrix2Stack_.pop_back();
    matrix3Stack_.pop_back();

    updateTransform();
  }
}

void
PGraphics::
loadPixels()
{
  // TODO: This is a NOOP
  //unhandled();
}

void
PGraphics::
updatePixels()
{
  // TODO: This is a NOOP
  //unhandled();
}

void
PGraphics::
save(const char *name)
{
  pixels.save(name);
}

void
PGraphics::
save(StringP name)
{
  pixels.save(name->c_str());
}

//---------

PFont::
PFont() :
 name_(new String("Helvetica")), size_(12), smooth_(false)
{
}

PFont::
PFont(StringP name, int size, bool smooth, const CharArray1D &chars) :
 name_(name), size_(size), smooth_(smooth), chars_(chars)
{
}

PFont::
PFont(StringP name) :
 name_(name), size_(12), smooth_(false)
{
}

PFont::
PFont(const PFont &font) :
 PObject(font        ),
 name_  (font.name_  ),
 size_  (font.size_  ),
 smooth_(font.smooth_),
 chars_ (font.chars_ )
{
}

PFont &
PFont::
operator=(const PFont &font)
{
  name_   = font.name_;
  size_   = font.size_;
  smooth_ = font.smooth_;
  chars_  = font.chars_;

  return *this;
}

void
PFont::
setSize(int size)
{
  size_ = size;
}

QFont
PFont::
getQFont() const
{
  return QFont(name_->c_str(), size_);
}

bool
PFont::
getSmooth() const
{
  return smooth_;
}

//---------

int
main(int argc, char **argv)
{
  QApplication app(argc, argv);

  CProcessing::doInit();

  timer_ = new CProcessingTimer;

  return app.exec();
}
