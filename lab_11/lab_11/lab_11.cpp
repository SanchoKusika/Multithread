#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <Windows.h>
#include <semaphore>

std::mutex mtx;  // Один мьютекс для защиты вывода
std::condition_variable cv;  // Переменная условия для сигнализации
bool ready_to_switch = true;  // Условная переменная для смены потоков
int current_thread = 1;  // Текущий активный поток

void printThread(int id, int max) {
    for (int i = 1; i <= max; i++) {
        std::unique_lock<std::mutex> lk(mtx);
        cv.wait(lk, [&]() { return current_thread == id; });  // Ожидание своей очереди
        std::cout << "Поток " << id << ": Строка " << i << std::endl;
        current_thread = id == 1 ? 2 : 1;  // Переключение на другой поток
        lk.unlock();
        cv.notify_all();  // Оповещение другого потока
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    const int max_lines = 10;

    std::thread t1(printThread, 1, max_lines);
    std::thread t2(printThread, 2, max_lines);

    t1.join();
    t2.join();

    return 0;
}

// 12:
// Deadlock
// Race condition
// Неэффективное использование ресурсов
// 
// 
// решение задания подходит к 13 лаб