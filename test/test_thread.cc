#include <unistd.h>
#include <shared_mutex>

#include <iostream>

#include "yf_lock.h"
#include "yf_mutex.h"
#include "yf_thread.h"
#include "yf_shared_mutex.h"

yf::mutex g_mutex;
yf::mutex g_mutex1;
yf::atomic_mutex g_amutex;
yf::shared_mutex g_smutex;

void func(int a) {
  while (true) {
    sleep(1);
    // yf::lock_guard<yf::mutex> lock{g_mutex};
    // yf::lock_guard<yf::atomic_mutex> alock{g_amutex};
    // yf::unique_lock<yf::atomic_mutex> alock{g_amutex};
    // yf::unique_lock<yf::mutex> lock{g_mutex};
    // yf::unique_lock<yf::mutex> lock1{g_mutex1};
    // yf::unique_lock<yf::mutex> lock2{g_mutex};
    // lock1 = std::move(lock2);
    // lock1.swap(lock2);
    if (!g_mutex.try_lock()) {
      yf::shared_lock<yf::shared_mutex> slock{g_smutex};
      std::cout << "trylock false: " << yf::thread::getThreadId() << " a: " << a
                << "\n";
    } else {
      yf::shared_lock<yf::shared_mutex> slock{g_smutex};
      std::cout << "threadId: " << yf::thread::getThreadId() << " a: " << a
                << "begin\n";
      sleep(2);
      std::cout << "threadId: " << yf::thread::getThreadId() << " a: " << a
                << "end\n";
      g_mutex.unlock();
    }
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
  int b = 10;
  yf::thread t1(A::func, 1);
  t1.join();
  A a;
  yf::thread t2(&A::funcA, &a, b);
  std::cout << "b:" << b << '\n';
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
