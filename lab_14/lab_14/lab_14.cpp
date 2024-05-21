#include <iostream>
#include <thread>
#include <semaphore>
#include <windows.h>

std::counting_semaphore<1> sem1(1);
std::counting_semaphore<1> sem2(0);

void printThread(int id, int max) {
    for (int i = 1; i <= max; i++) {
        if (id == 1) {
            sem1.acquire();  // Ждем сигнала от семафора 1
            std::cout << "Поток 1: Строка " << i << std::endl;
            sem2.release();  // Отправляем сигнал семафору 2
        }
        else {
            sem2.acquire();  // Ждем сигнала от семафора 2
            std::cout << "Поток 2: Строка " << i << std::endl;
            sem1.release();  // Отправляем сигнал семафору 1
        }
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


//15
// семафоры контролируют порядок доступа потоков к ресурсам