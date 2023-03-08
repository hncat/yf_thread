#ifndef __YF_MUTEX_H__
#define __YF_MUTEX_H__

#include <pthread.h>

#include <atomic>

namespace yf {
class mutex_basic {
   protected:
    mutex_basic() = default;
    virtual ~mutex_basic() = default;

   public:
    mutex_basic(const mutex_basic &) = delete;
    mutex_basic &operator=(const mutex_basic &) = delete;
    mutex_basic(mutex_basic &&) = delete;
    mutex_basic &operator=(mutex_basic &&) = delete;
    virtual void lock() = 0;
    virtual void unlock() = 0;
};

class mutex : private mutex_basic {
   public:
    mutex();
    ~mutex();
    void lock() override;
    void unlock() override;

   private:
    pthread_mutex_t _mutex;
};

class atomic_mutex : public mutex_basic {
   public:
    atomic_mutex();
    ~atomic_mutex() = default;
    void lock() override;
    void unlock() override;

   private:
    std::atomic<bool> _mutex;
};
}  // namespace yf
#endif