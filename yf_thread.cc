#include "yf_thread.h"

namespace yf {
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

uint64_t thread::getThreadId() { return syscall(SYS_gettid); }
}  // namespace yf
