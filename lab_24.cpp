#include <iostream>
#include <thread>
#include <semaphore>
#include <chrono>
#include <windows.h>

std::counting_semaphore<1> semA(0);
std::counting_semaphore<1> semB(0);
std::counting_semaphore<1> semC(0);

void produceA() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));  // Имитация времени на изготовление детали А
        std::cout << "Деталь A произведена.\n";
        semA.release();  // Отпустить семафор для детали А
    }
}

void produceB() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(2));  // Имитация времени на изготовление детали B
        std::cout << "Деталь B произведена.\n";
        semB.release();  // Отпустить семафор для детали B
    }
}

void produceC() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(3));  // Имитация времени на изготовление детали C
        std::cout << "Деталь C произведена.\n";
        semC.release();  // Отпустить семафор для детали C
    }
}

void assembleWidget() {
    while (true) {
        semA.acquire();  // Ожидание детали А
        semB.acquire();  // Ожидание детали B
        semC.acquire();  // Ожидание детали C
        std::cout << "Виджет собран из деталей A, B и C.\n";
        // Здесь может быть код для отправки виджета на следующий этап или упаковки
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::thread threadA(produceA);
    std::thread threadB(produceB);
    std::thread threadC(produceC);
    std::thread assemblyThread(assembleWidget);

    threadA.join();
    threadB.join();
    threadC.join();
    assemblyThread.join();

    return 0;
}