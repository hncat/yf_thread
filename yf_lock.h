#ifndef __YF_LOCK_H__
#define __YF_LOCK_H__

#include <type_traits>

#include "yf_mutex.h"
#include "yf_shared_mutex.h"

namespace yf {
#define assert_unlock(__mutex, __owns) assert(__mutex &&__owns)
#define assert_lock(__mutex, __owns) assert_lock(__mutex, !__owns)

template <typename Mutex> class lock_guard {
public:
  typedef Mutex mutex_type;
  typedef Mutex &mutex_reference;
  typedef Mutex *mutex_pointer;

public:
  explicit lock_guard(mutex_reference m) : _mutex(m) { _mutex.lock(); }
  lock_guard(mutex_reference &m, adopt_lock_t) : _mutex(m) {}
  ~lock_guard() { _mutex.unlock(); }
  lock_guard(const lock_guard &) = delete;
  lock_guard &operator=(const lock_guard &) = delete;
  mutex_reference get() { return _mutex; }

private:
  mutex_reference _mutex;
};

template <typename Mutex> class unique_lock {
public:
  typedef Mutex mutex_type;
  typedef Mutex &mutex_reference;
  typedef Mutex *mutex_pointer;

public:
  unique_lock() : _mutex(nullptr), _owns(false) {}
  explicit unique_lock(mutex_reference mutex)
      : _mutex(std::addressof(mutex)), _owns(false) {
    _mutex->lock();
    _owns = true;
  }
  unique_lock(mutex_reference mutex, defer_lock_t)
      : _mutex(std::addressof(mutex)), _owns(false) {}
  unique_lock(mutex_reference mutex, adopt_lock_t)
      : _mutex(std::addressof(mutex)), _owns(true) {}
  unique_lock(mutex_reference mutex, try_lock_t)
      : _mutex(mutex), _owns(_mutex->try_lock()) {}
  unique_lock(unique_lock &&_lock) : _mutex(_lock._mutex), _owns(_lock._owns) {
    _lock._mutex = nullptr;
    _lock._owns = false;
  }
  unique_lock &operator=(unique_lock &&_lock) {
    if (this == std::addressof(_lock)) {
      return *this;
    }
    if (_owns) {
      _mutex->unlock();
    }
    _mutex = nullptr;
    _owns = false;
    _lock.swap(*this);
    return *this;
  }
  void lock() {
    assert_lock(_mutex, _owns);
    _mutex->lock();
    _owns = true;
  }
  bool try_lock() {
    assert_lock(_mutex, _owns);
    _owns = _mutex->try_lock();
    return _owns;
  }
  void unlock() {
    assert_unlock(_mutex, _owns);
    _mutex->unlock();
    _owns = false;
  }
  void swap(unique_lock &_lock) {
    std::swap(_mutex, _lock._mutex);
    std::swap(_owns, _lock._owns);
  }
  bool owns_lock() const { return _owns; }
  mutex_pointer get() { return _mutex; }
  ~unique_lock() {
    if (_mutex) {
      _mutex->unlock();
    }
  }
  unique_lock(const unique_lock &) = delete;
  unique_lock &operator=(const unique_lock &) = delete;

private:
  mutex_pointer _mutex;
  bool _owns;
};

template <typename Mutex>
void swap(unique_lock<Mutex> &__x, unique_lock<Mutex> &__y) {
  __x.swap(__y);
}

} // namespace yf

#endif
