#include "yf_mutex.h"

#include <pthread.h>

namespace yf {
mutex::mutex() : _mutex(PTHREAD_MUTEX_INITIALIZER) {}

mutex::~mutex() { pthread_mutex_destroy(&_mutex); }

void mutex::lock() { pthread_mutex_lock(&_mutex); }

void mutex::unlock() { pthread_mutex_unlock(&_mutex); }

bool mutex::trylock() { return pthread_mutex_trylock(&_mutex) == 0; }

atomic_mutex::atomic_mutex() : _mutex{true} {}

void atomic_mutex::lock() {
    bool islock = true;
    while (!_mutex.compare_exchange_weak(islock, false)) {
        islock = true;
    }
}

void atomic_mutex::unlock() { _mutex.store(true); }
}  // namespace yf
