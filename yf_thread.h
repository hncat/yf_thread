#ifndef __YF_THREAD_H__
#define __YF_THREAD_H__

#include <assert.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <memory>
#include <tuple>
#include <type_traits>

namespace yf {

template <typename... Args, std::size_t... I>
void threadCallback(std::shared_ptr<std::tuple<Args...>> &pack,
                    std::index_sequence<I...>) {
    std::get<0> (*pack)(std::get<I + 1>(*pack)...);
}

template <typename Obj, typename... Args, std::size_t... I,
          typename = std::enable_if_t<std::is_class<Obj>::value>>
void threadCallback(Obj *obj, std::shared_ptr<std::tuple<Args...>> &pack,
                    std::index_sequence<I...>) {
    (obj->*std::get<0>(*pack))(std::get<I + 1>(*pack)...);
}

class thread {
   public:
    thread(const thread &) = delete;
    thread &operator=(const thread &) = delete;
    template <typename Func, typename... Args>
    thread(Func &&func, Args &&...args) : _ptid(0), _joinable(false) {
        using pack_type = std::tuple<Func, Args...>;
        static std::shared_ptr<pack_type> taskPack;
        taskPack.reset(new pack_type{std::forward<Func>(func), std::forward<Args>(args)...});
        struct ThreadFunc {
            static void *_run(void *) {
                auto _taskPack = std::move(taskPack);
                threadCallback(_taskPack,
                               std::make_index_sequence<sizeof...(Args)>());
                _taskPack.reset();
                return nullptr;
            }
        };
        int ret = pthread_create(&_ptid, nullptr, ThreadFunc::_run, nullptr);
        assert(ret == 0);
        _joinable = true;
    }
    template <typename Func, typename Obj, typename... Args,
              typename = std::enable_if_t<
                  std::is_member_function_pointer<std::decay_t<Func>>::value &&
                  std::is_class<Obj>::value>>
    thread(Func &&func, Obj *obj, Args &&...args) {
        using pack_type = std::tuple<Func, Args...>;
        static std::shared_ptr<pack_type> taskPack;
        taskPack.reset(new pack_type{std::forward<Func>(func), std::forward<Args>(args)...});
        static auto *objTmp = obj;
        struct ThreadFunc {
            static void *_run(void *) {
                auto _taskPack = std::move(taskPack);
                threadCallback(objTmp, _taskPack,
                               std::make_index_sequence<sizeof...(Args)>());
                _taskPack.reset();
                return nullptr;
            }
        };
        int ret = pthread_create(&_ptid, nullptr, ThreadFunc::_run, nullptr);
        assert(ret == 0);
        _joinable = true;
    }
    ~thread() = default;
    void join();
    void detach();
    bool joinable() const;
    pthread_t getPthreadId();
    static uint64_t getThreadId();

   private:
    pthread_t _ptid;
    bool _joinable;
};
};  // namespace yf

#endif
