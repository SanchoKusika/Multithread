#include <iostream>
#include <queue>
#include <mutex>
#include <semaphore>
#include <string>
#include <windows.h>

class MessageQueue {
private:
    std::queue<std::string> queue;
    std::counting_semaphore<> available_slots;
    std::counting_semaphore<> available_items;
    std::mutex mutex;

public:
    MessageQueue(int capacity) : available_slots(capacity), available_items(0) {}

    void init() {
        std::lock_guard<std::mutex> lock(mutex);
        while (!queue.empty()) queue.pop();
    }

    void put(const std::string& msg) {
        available_slots.acquire();
        {
            std::lock_guard<std::mutex> lock(mutex);
            queue.push(msg);
        }
        available_items.release();
    }

    std::string get() {
        available_items.acquire();
        std::lock_guard<std::mutex> lock(mutex);
        std::string msg = queue.front();
        queue.pop();
        available_slots.release();
        return msg;
    }

    void destroy() {
        std::lock_guard<std::mutex> lock(mutex);
        while (!queue.empty()) queue.pop();
    }
};

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    MessageQueue queue(10); // Очередь с максимальной вместимостью 10 сообщений
    queue.init();
    queue.put("Привет");
    queue.put("Мир");
    std::cout << "Сообщение 1: " << queue.get() << std::endl;
    std::cout << "Сообщение 2: " << queue.get() << std::endl;
    queue.destroy();
    return 0;
}
