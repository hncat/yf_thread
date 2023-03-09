#ifndef __YF_COND_VAR_H__
#define __YF_COND_VAR_H__

#include <pthread.h>

#include "yf_lock.h"
#include "yf_mutex.h"
#include "yf_type_traits.h"

namespace yf {
class conditional_variable {
   public:
    conditional_variable();
    ~conditional_variable();
    void wait(lock_gurd<mutex> &lock);
    template <typename Func, typename... Args,
              typename = typename std::enable_if<
                  !is_result_void<Func, Args...>::value>::type>
    void wait_for(lock_gurd<mutex> &lock, Func &&func, Args &&...args) {
        while (func(std::forward<Args>(args)...)) {
            wait(lock);
        }
    }
    void notfiy_one();
    void notify_all();

   private:
    pthread_cond_t _cond;
};
}  // namespace yf

#endif
