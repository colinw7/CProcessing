#include <CProcessingTimer.h>
#include <CProcessing.h>

CProcessingTimer::
CProcessingTimer()
{
  startTimer(10);
}

void
CProcessingTimer::
timerEvent(QTimerEvent *)
{
  CProcessing::doLoop();
}

void
CProcessingTimer::
startTimer(int fps)
{
  double delay = 1000.0/std::max(fps, 1);

  id_ = QObject::startTimer(delay);
}

void
CProcessingTimer::
stopTimer()
{
  if (id_)
    killTimer(id_);

  id_ = 0;
}
