#include "yf_mutex.h"

#include <assert.h>
#include <errno.h>

namespace yf {
mutex::mutex() : _mutex(PTHREAD_MUTEX_INITIALIZER) {}

mutex::~mutex() { pthread_mutex_destroy(&_mutex); }

void mutex::lock() {
  int err = pthread_mutex_lock(&_mutex);
  assert(err == 0);
}

bool mutex::try_lock() { return pthread_mutex_trylock(&_mutex); }

void mutex::unlock() { pthread_mutex_unlock(&_mutex); }

atomic_mutex::atomic_mutex() : _mutex{true} {}

void atomic_mutex::lock() {
  bool islock = true;
  while (!_mutex.compare_exchange_weak(islock, false)) {
    islock = true;
  }
}

bool atomic_mutex::try_lock() {
  bool islock = true;
  return _mutex.compare_exchange_weak(islock, false);
}

void atomic_mutex::unlock() { _mutex.store(true); }

spin_lock::spin_lock(bool process_shared) {
  int err;
  if (process_shared) {
    err = pthread_spin_init(&_spin, PTHREAD_PROCESS_SHARED);
  } else {
    err = pthread_spin_init(&_spin, PTHREAD_PROCESS_PRIVATE);
  }
  assert(err == 0 || err == EAGAIN);
}

spin_lock::~spin_lock() { pthread_spin_destroy(&_spin); }

void spin_lock::lock() {
  int err = pthread_spin_lock(&_spin);
  assert(err != EDEADLK);
}

bool spin_lock::try_lock() {
  int err = pthread_spin_trylock(&_spin);
  if (err == EBUSY) {
    return false;
  }
  assert(err == 0);
  return true;
}

void spin_lock::unlock() { pthread_spin_unlock(&_spin); }
} // namespace yf
