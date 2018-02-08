#ifndef MUTEX_H
#define MUTEX_H
#include <QThread>
#include <QMutex>

// Use as data member of a class, eg.
//
// class Foo
// {
//  public:
//   int size() const;
//
//  private:
//   mutable MutexLock mutex_;
//   std::vector<int> data_; // GUARDED BY mutex_
// };
class MutexLock
{
 public:
  MutexLock()
    : holder_(nullptr)
  {
  }

  ~MutexLock()
  {

  }

  // must be called when locked, i.e. for assertion
  bool isLockedByThisThread() const
  {
    return holder_ == QThread::currentThreadId();
  }

  void assertLocked() const
  {

  }

  // internal usage

  void lock()
  {
    mutex_.lock();
    assignHolder();
  }

  void unlock()
  {
    unassignHolder();
    mutex_.unlock();
  }

  QMutex* getPthreadMutex() /* non-const */
  {
    return &mutex_;
  }

 private:
  friend class Condition;

  class UnassignGuard
  {
   public:
    UnassignGuard(MutexLock& owner)
      : owner_(owner)
    {
      owner_.unassignHolder();
    }

    ~UnassignGuard()
    {
      owner_.assignHolder();
    }

   private:
    MutexLock& owner_;
  };

  void unassignHolder()
  {
    holder_ = nullptr;
  }

  void assignHolder()
  {
    holder_ = QThread::currentThreadId();
  }

  QMutex mutex_;
  Qt::HANDLE holder_;
};

// Use as a stack variable, eg.
// int Foo::size() const
// {
//   MutexLockGuard lock(mutex_);
//   return data_.size();
// }
class MutexLockGuard
{
 public:
  explicit MutexLockGuard(MutexLock& mutex)
    : mutex_(mutex)
  {
    mutex_.lock();
  }

  ~MutexLockGuard()
  {
    mutex_.unlock();
  }

 private:

  MutexLock& mutex_;
};


// Prevent misuse like:
// MutexLockGuard(mutex_);
// A tempory object doesn't hold the lock for long!
#define MutexLockGuard(x) error "Missing guard object name"
#endif // MUTEX_H
