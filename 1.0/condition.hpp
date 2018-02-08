#ifndef CONDITION_HPP
#define CONDITION_HPP

#include <QWaitCondition>
#include "mutex.h"
class Condition
{
 public:
  explicit Condition(MutexLock& mutex)
    : mutex_(mutex)
  {

  }

  ~Condition()
  {

  }

  void wait()
  {
        MutexLock::UnassignGuard ug(mutex_);
        pcond_.wait(mutex_.getPthreadMutex());
  }

  // returns true if time out, false otherwise.
  //bool waitForSeconds(double seconds);

  void notify()
  {
        pcond_.wakeOne();
  }

  void notifyAll()
  {
    pcond_.wakeAll();
  }

 private:
  MutexLock& mutex_;
  QWaitCondition  pcond_;
};

#endif // CONDITION_HPP
