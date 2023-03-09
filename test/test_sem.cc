#include <unistd.h>

#include <atomic>
#include <iostream>
#include <vector>

#include "yf_lock.h"
#include "yf_mutex.h"
#include "yf_sem.h"
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
yf::atomic_mutex tamutex;
yf::sem tsem{0};

void addTask(int num) {
    while (num > 0) {
        yf::lock_guard<yf::atomic_mutex> lock(tamutex);
        tasks.emplace_back(++count);
        tsem.post();
        --num;
    }
}

void consumeTask() {
    while (true) {
        if (!tsem.wait()) {
            continue;
        }
        Task task;
        {
            yf::lock_guard<yf::atomic_mutex> lock(tamutex);
            if (tasks.empty()) {
                continue;
            }
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
