#include "yf_thread.h"
#include <iostream>
#include <unistd.h>

void func(int a) {
    std::cout << "threadId: " << yf::thread::getThreadId() << " a: " << a << '\n';
    while (true) {
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
    void funcA(int a) {
        std::cout << a << "funcA()\n";
    }
    static void func(int a) {
        std::cout << a << "A::func()\n";
    }
};

void test02() {
    // yf::thread t1(A::func, 1);
    // t1.join();
    A a;
    yf::thread t2(&A::funcA, &a, 1);
    t2.join();
}

void test03() {
    yf::thread t1([](int a){ std::cout << "a: " << a << " lambda thread\n"; }, 1);
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
