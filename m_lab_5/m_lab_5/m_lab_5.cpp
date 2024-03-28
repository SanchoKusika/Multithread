#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <atomic>
#include <Windows.h>

class DiningPhilosophers {
public:
    explicit DiningPhilosophers(int num_philosophers)
        : forks(num_philosophers), meals_eaten(0) {}

    void dine() {
        for (int i = 0; i < forks.size(); ++i) {
            philosophers.emplace_back(&DiningPhilosophers::philosopher, this, i);
        }

        // Ждем пока каждый философ не поест по одному разу
        while (meals_eaten.load() < philosophers.size()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        running = false; // Сообщаем всем философам прекратить действия

        for (auto& ph : philosophers) {
            ph.join();
        }
    }

private:
    std::vector<std::thread> philosophers;
    std::vector<std::mutex> forks;
    std::atomic<int> meals_eaten;
    std::atomic<bool> running{ true };

    void philosopher(int id) {
        while (running) {
            think(id);
            eat(id);
        }
    }

    void think(int id) {
        std::cout << "Philosopher " << id + 1 << " is thinking." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    void eat(int id) {
        std::lock_guard<std::mutex> left_fork(forks[id]);
        std::lock_guard<std::mutex> right_fork(forks[(id + 1) % forks.size()]);

        if (running) { // Проверяем, что мы все еще в процессе выполнения
            std::cout << "Philosopher " << id + 1 << " is eating." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            meals_eaten++;
        }
    }
};

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int num_philosophers = 5;
    DiningPhilosophers table(num_philosophers);
    table.dine();

    std::cout << "Все философы поели один раз." << std::endl;
    return 0;
}
