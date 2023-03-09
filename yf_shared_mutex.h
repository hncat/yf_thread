#ifndef __YF_SHARED_MUTEX_H__
#define __YF_SHARED_MUTEX_H__

#include <pthread.h>

namespace yf {
class shared_mutex {
public:
  shared_mutex();
  ~shared_mutex();
  void lock();
  bool try_lock();
  void unlock();
  void shared_lock();
  bool try_shared_lock();
  void shared_unlock();
  shared_mutex(const shared_mutex &) = delete;
  shared_mutex &operator=(const shared_mutex &) = delete;

private:
  pthread_rwlock_t _mutex;
};
} // namespace yf

#endif
