#ifndef __YF_LOCK_H__
#define __YF_LOCK_H__

#include "yf_mutex.h"
#include <type_traits>

namespace yf {
template <typename T, typename = typename std::enable_if<
                          std::is_base_of<mutex_basic, T>::value>::type>
class lock_gurd {
   public:
    typedef T mutex_type;
    typedef T& mutex_reference;
    typedef T* mutex_pointer;

   public:
    lock_gurd(T& m) : _mutex(m) {
        _mutex.lock();
    }
    ~lock_gurd() {
        _mutex.unlock();
    }

   private:
    T& _mutex;
};
}  // namespace yf

#endif
