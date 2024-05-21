#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <atomic>
#include <condition_variable>

class DiningPhilosophers {
public:
    explicit DiningPhilosophers(int num_philosophers)
        : forks(num_philosophers), meals_eaten(0) {}

    void dine() {
        for (int i = 0; i < forks.size(); ++i) {
            philosophers.emplace_back(&DiningPhilosophers::philosopher, this, i);
        }

        for (auto& ph : philosophers) {
            ph.join();
        }
    }

private:
    std::vector<std::thread> philosophers;
    std::vector<std::mutex> forks;
    std::vector<std::condition_variable> conditions;
    std::atomic<int> meals_eaten;
    std::atomic<bool> running{ true };

    void philosopher(int id) {
        while (meals_eaten.load() < philosophers.size()) {
            think(id);
            eat(id);
        }
        running = false;
    }

    void think(int id) {
        std::cout << "Philosopher " << id + 1 << " is thinking." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    void eat(int id) {
        std::unique_lock<std::mutex> lock(forks[id], std::defer_lock);
        std::unique_lock<std::mutex> lock2(forks[(id + 1) % forks.size()], std::defer_lock);

        std::lock(lock, lock2); // Захватываем обе вилки одновременно

        if (running) { // Проверяем, что мы все еще в процессе выполнения
            std::cout << "Philosopher " << id + 1 << " is eating." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            meals_eaten++;
        }
    }
};

int main() {
    int num_philosophers = 5;
    DiningPhilosophers table(num_philosophers);
    table.dine();

    std::cout << "All Philosophers just eat once" << std::endl;
    return 0;
}
