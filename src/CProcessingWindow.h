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

  void paintEvent(QPaintEvent *);
  void resizeEvent(QResizeEvent *);

  void mousePressEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);
  void mouseReleaseEvent(QMouseEvent *);

  void keyPressEvent(QKeyEvent *);
  void keyReleaseEvent(QKeyEvent *);
};

#endif
