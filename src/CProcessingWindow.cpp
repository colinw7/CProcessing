#include <CProcessingWindow.h>
#include <CProcessing.h>

#include <QResizeEvent>
#include <QMouseEvent>
#include <QKeyEvent>

CProcessingWindow::
CProcessingWindow(int w, int h)
{
  resize(w, h);

  setMouseTracking(true);
}

void
CProcessingWindow::
paintEvent(QPaintEvent *)
{
  CProcessing::doDraw();
}

void
CProcessingWindow::
resizeEvent(QResizeEvent *e)
{
  CProcessing::doResize(e->size().width(), e->size().height());
}

void
CProcessingWindow::
mousePressEvent(QMouseEvent *e)
{
  CProcessing::mousePress(e->pos().x(), e->pos().y());
}

void
CProcessingWindow::
mouseMoveEvent(QMouseEvent *e)
{
  CProcessing::mouseMotion(e->pos().x(), e->pos().y());
}

void
CProcessingWindow::
mouseReleaseEvent(QMouseEvent *e)
{
  CProcessing::mouseRelease(e->pos().x(), e->pos().y());
}

void
CProcessingWindow::
keyPressEvent(QKeyEvent *event)
{
  QString str = event->text();

  if (str.length() != 1)
    return;

  CProcessing::keyPress(event->key(), str[0].toAscii());
}

void
CProcessingWindow::
keyReleaseEvent(QKeyEvent *event)
{
  QString str = event->text();

  if (str.length() != 1)
    return;

  CProcessing::keyRelease(event->key(), str[0].toAscii());
}
