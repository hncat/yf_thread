#include "yf_thread.h"
#include <iostream>
#include <unistd.h>

void func(int a) {
    std::cout << "a is a: " << a << " func thread\n";
    // sleep(1);
}

void test01() {
    yf::thread t1{func, 1};
    // std::cout << "pthread id:" << t1.getPthreadId() << '\n';
    t1.join();
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
    yf::thread t1([](int a){ std::cout << "a:" << a << " lambda thread\n"; }, 1);
    t1.join();
}

int main() {
    int i = 10;
    while (i > 0) {
        test01();
        test02();
        test03();
        --i;
    }
    return 0;
}
