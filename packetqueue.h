#ifndef PACKETQUEUE_H
#define PACKETQUEUE_H

#include "mutex.h"
#include "condition.hpp"
#include <queue>
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "SDL.h"
#include "SDL_thread.h"
}
typedef struct AVPacketItem {
    AVPacket pkt;
    double pts;
    double dts;
    double duration;
} AVPacketItem;

template<typename T>
class PacketQueue
{
public:
  PacketQueue()
    : mutex_(),
      notEmpty_(mutex_),
      queue_(),
      forceExit_(false)
  {
  }

  void put(const T& x)
  {
    MutexLockGuard lock(mutex_);
    queue_.push_back(x);
    notEmpty_.notify(); // wait morphing saves us
  }


  T take()
  {
    MutexLockGuard lock(mutex_);
    // always use a while-loop, due to spurious wakeup
    while (queue_.empty())
    {
      notEmpty_.wait();
      if(forceExit_)            // 强制退出
      {
          forceExit_ = false;
          return nullptr;
      }
    }

    T front(queue_.front());
    queue_.pop_front();
    return front;
  }

  size_t size() const
  {
    MutexLockGuard lock(mutex_);
    return queue_.size();
  }

  void trigerForceExit()
  {
      forceExit_ = true;
      notEmpty_.notify(); // wait morphing saves us
  }
private:
    mutable MutexLock mutex_;
    Condition         notEmpty_;
    std::deque<T>     queue_;
    bool forceExit_;
};

#endif // PACKETQUEUE_H
