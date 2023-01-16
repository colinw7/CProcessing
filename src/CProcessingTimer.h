#ifndef CProcessingTimer_H
#define CProcessingTimer_H

#include <QObject>

class QTimerEvent;

class CProcessingTimer : public QObject {
  Q_OBJECT

 public:
  CProcessingTimer();

  void timerEvent(QTimerEvent *) override;

  void startTimer(int fps);
  void stopTimer();

 private:
  int id_;
};

#endif
