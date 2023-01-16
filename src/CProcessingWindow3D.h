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

  void initializeGL() override;

  void resizeGL(int w, int h) override;

  void paintGL() override;

  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void mouseReleaseEvent(QMouseEvent *) override;

  void keyPressEvent(QKeyEvent *) override;
  void keyReleaseEvent(QKeyEvent *) override;

 private:
  CQGLControl        *control_;
  CQGLControlToolBar *toolbar_;
};

#endif
