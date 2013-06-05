#include <CProcessingWindow3D.h>
#include <CProcessing.h>
#include <CQGLControl.h>

#include <QResizeEvent>
#include <QMouseEvent>
#include <QKeyEvent>

CProcessingWindow3D::
CProcessingWindow3D(int w, int h)
{
  resize(w, h);

  setMouseTracking(true);

  control_ = new CQGLControl(this);

  toolbar_ = new CQGLControlToolBar(control_);

  toolbar_->show();
}

void
CProcessingWindow3D::
initializeGL()
{
  QGLWidget::initializeGL();
}

void
CProcessingWindow3D::
resizeGL(int w, int h)
{
  control_->handleResize(w, h);

  CProcessing::doResize(w, h);
}

void
CProcessingWindow3D::
paintGL()
{
  static GLfloat pos[4] = {5.0, 5.0, 10.0, 0.0};

  glLightfv(GL_LIGHT0, GL_POSITION, pos);
  glEnable(GL_LIGHT0);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  control_->getDepthTest  () ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
  control_->getCullFace   () ? glEnable(GL_CULL_FACE)  : glDisable(GL_CULL_FACE);
  control_->getLighting   () ? glEnable(GL_LIGHTING)   : glDisable(GL_LIGHTING);
  control_->getOutline    () ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) :
                               glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  control_->getFrontFace  () ? glFrontFace(GL_CW) : glFrontFace(GL_CCW);
  control_->getSmoothShade() ? glShadeModel(GL_SMOOTH) : glShadeModel(GL_FLAT);

  CProcessing::doDraw3D();
}

void
CProcessingWindow3D::
mousePressEvent(QMouseEvent *e)
{
  control_->handleMousePress(e);

  CProcessing::mousePress(e->pos().x(), e->pos().y());

  update();
}

void
CProcessingWindow3D::
mouseMoveEvent(QMouseEvent *e)
{
  control_->handleMouseMotion(e);

  CProcessing::mouseMotion(e->pos().x(), e->pos().y());

  update();
}

void
CProcessingWindow3D::
mouseReleaseEvent(QMouseEvent *e)
{
  control_->handleMouseRelease(e);

  CProcessing::mouseRelease(e->pos().x(), e->pos().y());

  update();
}

void
CProcessingWindow3D::
keyPressEvent(QKeyEvent *event)
{
  QString str = event->text();

  if (str.length() != 1)
    return;

  CProcessing::keyPress(event->key(), str[0].toAscii());
}

void
CProcessingWindow3D::
keyReleaseEvent(QKeyEvent *event)
{
  QString str = event->text();

  if (str.length() != 1)
    return;

  CProcessing::keyRelease(event->key(), str[0].toAscii());
}
