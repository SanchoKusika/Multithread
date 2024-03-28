#include <windows.h>
#include <iostream>
#include <thread>
#include <vector>
#include <cmath>

#ifndef NUM_ITERATIONS
#define NUM_ITERATIONS 1000000 // default если не определено во время компиляции
#endif

    // Функция для вычисления части ряда Лейбница
    void calculatePiSegment(long long start, long long end, double& sum) {
        double localSum = 0.0;
        for (long long i = start; i < end; i++) {
            localSum += std::pow(-1, i) / (2 * i + 1);
        }
        sum = localSum;
    }

    int main() {
        SetConsoleCP(1251);
        SetConsoleOutputCP(1251);
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

        // Суммируем результаты
        double pi = 0.0;
        for (double sum : partialSums) {
            pi += sum;
        }
        pi *= 4; // умножаем на 4, так как это часть формулы

        std::cout.precision(15);
        std::cout << "Приближенное значение Пи: " << pi << std::endl;

        return 0;
    }
