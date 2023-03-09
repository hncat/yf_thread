#include <unistd.h>

#include <atomic>
#include <iostream>
#include <vector>

#include "yf_cond_var.h"
#include "yf_lock.h"
#include "yf_mutex.h"
#include "yf_thread.h"

class Task {
   public:
    Task() = default;
    Task(int id) : taskId(id) {}
    void doTask() {
        std::cout << "Task threadId: " << yf::thread::getThreadId() << '\n';
        std::cout << "doTask taskId: " << taskId << '\n';
    }

   private:
    int taskId;
};

std::vector<Task> tasks;
static int count = 0;
yf::mutex tmutex;
yf::conditional_variable cond;

void addTask(int num) {
    while (num > 0) {
        {
            yf::lock_guard<yf::mutex> lock(tmutex);
            tasks.emplace_back(++count);
            --num;
        }
        cond.notfiy_one();
    }
}

void consumeTask() {
    while (true) {
        Task task;
        {
            yf::lock_guard<yf::mutex> lock(tmutex);
            cond.wait_for(lock, []() { return tasks.empty(); });
            task = tasks.back();
            tasks.pop_back();
        }
        task.doTask();
        sleep(1);
    }
}

int main(int argc, char *argv[]) {
    yf::thread t(addTask, 20);
    yf::thread t1(consumeTask);
    yf::thread t2(consumeTask);
    t.join();
    t1.join();
    t2.join();
    return 0;
}
