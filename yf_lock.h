#ifndef __YF_LOCK_H__
#define __YF_LOCK_H__

#include <type_traits>

#include "yf_mutex.h"

namespace yf {
template <typename T, typename = typename std::enable_if<
                          std::is_base_of<mutex_basic, T>::value>::type>
class lock_guard {
   public:
    typedef T mutex_type;
    typedef T& mutex_reference;
    typedef T* mutex_pointer;

   public:
    lock_guard(T& m) : _mutex(m) { _mutex.lock(); }
    ~lock_guard() { _mutex.unlock(); }
    mutex_reference get() { return _mutex; }

   private:
    T& _mutex;
};
}  // namespace yf

#endif
