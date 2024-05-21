#include <windows.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

    int buffer = 0; // Буфер для данных
    bool done = false; // Флаг завершения работы
    std::mutex mtx; // Мьютекс для синхронизации
    std::condition_variable cond_var; // Условная переменная для управления потоками

    void producer() {
        for (int i = 1; i <= 25; ++i) {
            std::unique_lock<std::mutex> lock(mtx);
            buffer = i;
            std::cout << "Произведено: " << buffer << std::endl;
            // Оповестить потребителя о наличии нового значения в буфере
            cond_var.notify_one();
            // Ожидать, пока потребитель не обработает данные
            cond_var.wait(lock, [] {return buffer == 0; });
        }
        done = true;
        cond_var.notify_one(); // Оповестить потребителя о завершении работы
    }

    void consumer() {
        std::unique_lock<std::mutex> lock(mtx);
        while (!done) {
            // Ожидать новых данных в буфере или завершения работы
            cond_var.wait(lock, [] {return buffer != 0 || done; });
            if (!done) {
                std::cout << "Потреблено: " << buffer << std::endl;
                buffer = 0; // Пометить данные как обработанные
                // Оповестить производителя о готовности к получению новых данных
                cond_var.notify_one();
            }
        }
    }

    int main() {
        SetConsoleCP(1251);
        SetConsoleOutputCP(1251);

        std::thread producerThread(producer);
        std::thread consumerThread(consumer);

        producerThread.join();
        consumerThread.join();

        return 0;
    }