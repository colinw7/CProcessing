#ifndef CProcessingWindow_H
#define CProcessingWindow_H

#include <QWidget>

class QPaintEvent;
class QResizeEvent;
class QMouseEvent;
class QKeyEvent;

class CProcessingWindow : public QWidget {
  Q_OBJECT

 public:
  CProcessingWindow(int w, int h);

  void paintEvent(QPaintEvent *) override;
  void resizeEvent(QResizeEvent *) override;

  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void mouseReleaseEvent(QMouseEvent *) override;

  void keyPressEvent(QKeyEvent *) override;
  void keyReleaseEvent(QKeyEvent *) override;
};

#endif
