#include "yf_shared_mutex.h"

namespace yf {
shared_mutex::shared_mutex() : _mutex(PTHREAD_RWLOCK_INITIALIZER) {}

shared_mutex::~shared_mutex() { pthread_rwlock_destroy(&_mutex); }

void shared_mutex::lock() { pthread_rwlock_wrlock(&_mutex); }

bool shared_mutex::try_lock() { return pthread_rwlock_trywrlock(&_mutex) == 0; }

void shared_mutex::unlock() { pthread_rwlock_unlock(&_mutex); }

void shared_mutex::shared_lock() { pthread_rwlock_rdlock(&_mutex); }

bool shared_mutex::try_shared_lock() {
  return pthread_rwlock_tryrdlock(&_mutex) == 0;
}

void shared_mutex::shared_unlock() { unlock(); }
} // namespace yf
