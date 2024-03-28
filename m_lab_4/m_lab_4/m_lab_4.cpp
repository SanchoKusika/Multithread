#include <iostream>
#include <thread>
#include <vector>
#include <cmath>
#include <csignal>
#include <atomic>
#include <windows.h>

#ifndef NUM_ITERATIONS
#define NUM_ITERATIONS 1000000000 // Значение по умолчанию
#endif

std::atomic<bool> running(true); // Атомарная переменная для безопасного обращения из нескольких потоков

// Функция для вычисления части ряда Лейбница
void calculatePiSegment(long long start, long long end, double& sum) {
    double localSum = 0.0;
    for (long long i = start; i < end && running.load(); i++) {
        localSum += std::pow(-1, i) / (2 * i + 1);
        if (i % 100000 == 0) { // Периодически проверяем флаг
            std::this_thread::yield(); // Уступаем время другим потокам
        }
    }
    sum = localSum;
}

// Обработчик сигнала прерывания
void signalHandler(int signal) {
    if (signal == SIGINT) {
        running.store(false);
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    // Устанавка обработчика сигнала
    std::signal(SIGINT, signalHandler);

    int numThreads;
    std::cout << "Введите количество потоков: ";
    std::cin >> numThreads;

    std::vector<std::thread> threads(numThreads);
    std::vector<double> partialSums(numThreads, 0.0);
    long long iterationsPerThread = NUM_ITERATIONS / numThreads;

    // Запускаем потоки
    for (int i = 0; i < numThreads; ++i) {
        long long start = i * iterationsPerThread;
        long long end = (i + 1 == numThreads) ? NUM_ITERATIONS : (i + 1) * iterationsPerThread;
        threads[i] = std::thread(calculatePiSegment, start, end, std::ref(partialSums[i]));
    }

    // Ждем завершения всех потоков
    for (std::thread& t : threads) {
        t.join();
    }

    // Сумма результатов из тех потоков, что успели завершиться.
    double pi = 0.0;
    for (double sum : partialSums) {
        pi += sum;
    }
    pi *= 4;

    std::cout.precision(15);
    std::cout << "Приближенное значение Пи (могут быть неточности из-за прерывания): " << pi << std::endl;

    return 0;
}
