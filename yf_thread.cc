#include "yf_thread.h"

#include <sys/syscall.h>
#include <unistd.h>

namespace yf {
static thread_local uint32_t _tid = 0;

void thread::join() {
    int ret = pthread_join(_ptid, nullptr);
    assert(ret == 0);
    _ptid = 0;
}

void thread::detach() {
    int ret = pthread_detach(_ptid);
    assert(ret == 0);
    _joinable = false;
    _ptid = 0;
}

bool thread::joinable() const { return _joinable; }

pthread_t thread::getPthreadId() { return _ptid; }

uint32_t thread::getThreadId() {
    if (_tid == 0) {
        _tid = ::syscall(SYS_gettid);
    }
    return _tid;
}
}  // namespace yf
