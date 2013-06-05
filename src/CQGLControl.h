#ifndef CQGLControl_H
#define CQGLControl_H

#include <QWidget>
#include <CPoint3D.h>

class QGLWidget;
class CQToolButton;
class CQGLControlToolBar;

class CQGLControl : public QObject {
  Q_OBJECT

 public:
  CQGLControl(QGLWidget *w);

  void setSize(double size=2.0);
  void setCenter(double x=0.0, double y=0.0);

  void handleResize(int w, int h);

  void handleMousePress(QMouseEvent *e);
  void handleMouseRelease(QMouseEvent *);
  bool handleMouseMotion(QMouseEvent *e);

  CQGLControlToolBar *createToolBar();

  void updateGL();

  bool getDepthTest  () const { return depth_test_  ; }
  bool getCullFace   () const { return cull_face_   ; }
  bool getLighting   () const { return lighting_    ; }
  bool getOutline    () const { return outline_     ; }
  bool getFrontFace  () const { return front_face_  ; }
  bool getSmoothShade() const { return smooth_shade_; }

  void setDepthTest  (bool b) { depth_test_   = b; }
  void setCullFace   (bool b) { cull_face_    = b; }
  void setLighting   (bool b) { lighting_     = b; }
  void setOutline    (bool b) { outline_      = b; }
  void setFrontFace  (bool b) { front_face_   = b; }
  void setSmoothShade(bool b) { smooth_shade_ = b; }

 private:
  void getMousePos(int x, int y, double *px, double *py, double *pz);
  void getMatrix();

 signals:
  void stateChanged();

 private slots:
  void depthSlot(bool);
  void cullSlot(bool);
  void lightSlot(bool);
  void outlineSlot(bool);
  void frontSlot(bool);
  void smoothSlot(bool);

 private:
  QGLWidget          *w_;

  CQGLControlToolBar *toolbar_;

  bool depth_test_;
  bool cull_face_;
  bool lighting_;
  bool outline_;
  bool front_face_;
  bool smooth_shade_;

  double   left_, right_, bottom_, top_;
  double   size_;
  double   center_x_, center_y_;
  double   znear_, zfar_;
  int      mouse_x_, mouse_y_;
  bool     mouse_left_, mouse_middle_, mouse_right_;
  double   drag_pos_x_, drag_pos_y_, drag_pos_z_;
  double   matrix_[16];
  double   imatrix_[16];
  CPoint3D ref_point_; /* Configurable center point for zooming and rotation */
};

class CQGLControlToolBar : public QWidget {
  Q_OBJECT

 public:
  CQGLControlToolBar(CQGLControl *control);

 signals:
  void stateChanged();

 private:
  CQGLControl  *control_;
  CQToolButton *depthButton_;
  CQToolButton *cullButton_;
  CQToolButton *lightButton_;
  CQToolButton *outlineButton_;
  CQToolButton *frontButton_;
  CQToolButton *smoothButton_;
};

#endif
