#include <unistd.h>

#include <iostream>

#include "yf_lock.h"
#include "yf_mutex.h"
#include "yf_thread.h"

yf::mutex g_mutex;
yf::atomic_mutex g_amutex;

void func(int a) {
    while (true) {
        sleep(1);
        // yf::lock_gurd<yf::mutex> lock{g_mutex};
        yf::lock_gurd<yf::atomic_mutex> alock{g_amutex};
        std::cout << "threadId: " << yf::thread::getThreadId() << " a: " << a
                  << "begin\n";
        sleep(2);
        std::cout << "threadId: " << yf::thread::getThreadId() << " a: " << a
                  << "end\n";
    }
}

void test01() {
    yf::thread t1{func, 1};
    yf::thread t2{func, 2};
    t1.join();
    t2.join();
}

struct A {
   public:
    void funcA(int a) { std::cout << a << "funcA()\n"; }
    static void func(int a) { std::cout << a << "A::func()\n"; }
};

void test02() {
    A a;
    yf::thread t2(&A::funcA, &a, 1);
    t2.join();
}

void test03() {
    yf::thread t1([](int a) { std::cout << "a: " << a << " lambda thread\n"; },
                  1);
    t1.join();
}

int main() {
    std::cout << "main threadId: " << yf::thread::getThreadId() << '\n';
    // while (true) {
    test01();
    // test02();
    // test03();
    // }
    return 0;
}
