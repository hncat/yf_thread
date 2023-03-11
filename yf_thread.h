#ifndef __YF_THREAD_H__
#define __YF_THREAD_H__

#include <assert.h>
#include <pthread.h>
#include <stdint.h>

#include <tuple>
#include <type_traits>

namespace yf {

template <typename... Args, std::size_t... I>
void threadCallback(std::tuple<Args...> *pack, std::index_sequence<I...>) {
  std::get<0> (*pack)(std::get<I + 1>(*pack)...);
}

template <typename Obj, typename... Args, std::size_t... I,
          typename = std::enable_if_t<std::is_class<Obj>::value>>
void threadCallback(Obj *obj, std::tuple<Args...> *pack,
                    std::index_sequence<I...>) {
  (obj->*std::get<1>(*pack))(std::get<I + 2>(*pack)...);
}

class thread {
public:
  thread(const thread &) = delete;
  thread &operator=(const thread &) = delete;
  thread(thread &&) = delete;
  thread &operator=(thread &&) = delete;
  template <typename Func, typename... Args,
            typename = decltype(std::declval<Func>()(std::declval<Args>()...))>
  thread(Func &&func, Args &&...args) : _ptid(0), _joinable(false) {
    using pack_type = std::tuple<Func, Args...>;
    auto taskPack{
        new pack_type{std::forward<Func>(func), std::forward<Args>(args)...}};
    struct ThreadFunc {
      static void *_run(void *arg) {
        auto _taskPack = static_cast<pack_type *>(arg);
        threadCallback(_taskPack, std::make_index_sequence<sizeof...(Args)>());
        delete _taskPack;
        return nullptr;
      }
    };
    int ret = pthread_create(&_ptid, nullptr, ThreadFunc::_run, taskPack);
    assert(ret == 0);
    _joinable = true;
  }
  template <typename Func, typename Obj, typename... Args,
            typename = std::enable_if_t<
                std::is_member_function_pointer<std::decay_t<Func>>::value &&
                std::is_class<std::decay_t<Obj>>::value>>
  thread(Func &&func, Obj *obj, Args &&...args) {
    using pack_type = std::tuple<Obj *, Func, Args...>;
    auto taskPack{new pack_type{obj, std::forward<Func>(func),
                                std::forward<Args>(args)...}};
    struct ThreadFunc {
      static void *_run(void *arg) {
        auto _taskPack = static_cast<pack_type *>(arg);
        threadCallback(std::get<0>(*_taskPack), _taskPack,
                       std::make_index_sequence<sizeof...(Args)>());
        delete _taskPack;
        return nullptr;
      }
    };
    int ret = pthread_create(&_ptid, nullptr, ThreadFunc::_run, taskPack);
    assert(ret == 0);
    _joinable = true;
  }
  void join();
  void detach();
  bool joinable() const;
  pthread_t getPthreadId();
  static uint32_t getThreadId();

private:
  pthread_t _ptid;
  bool _joinable;
};
} // namespace yf
#endif
