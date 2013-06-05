#ifndef CProcessingWindow3D_H
#define CProcessingWindow3D_H

#include <QGLWidget>

class QPaintEvent;
class QResizeEvent;
class QMouseEvent;
class QKeyEvent;

class CQGLControl;
class CQGLControlToolBar;

class CProcessingWindow3D : public QGLWidget {
  Q_OBJECT

 public:
  CProcessingWindow3D(int w, int h);

  void initializeGL();

  void resizeGL(int w, int h);

  void paintGL();

  void mousePressEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);
  void mouseReleaseEvent(QMouseEvent *);

  void keyPressEvent(QKeyEvent *);
  void keyReleaseEvent(QKeyEvent *);

 private:
  CQGLControl        *control_;
  CQGLControlToolBar *toolbar_;
};

#endif
