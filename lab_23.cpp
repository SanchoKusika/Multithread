#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <string>
#include <list>
#include <mutex>
#include <algorithm>

std::list<std::string> resultList;
std::mutex listMutex;

void sleepAndStore(const std::string& str) {
    // 1 секунда на символ пропорционально длине строки
    std::this_thread::sleep_for(std::chrono::seconds(str.length()));
    {
        std::lock_guard<std::mutex> guard(listMutex);
        resultList.push_back(str);
    }
}

int main() {
    std::vector<std::string> strings = {
        "banana",
        "apple",
        "cherry",
        "date",
        "elderberry",
        "fig",
        "grape"
    };

    std::vector<std::thread> threads;

    // Создание потока для каждой строки в векторе
    for (const std::string& str : strings) {
        threads.emplace_back(sleepAndStore, str);
    }

    // Ожидание завершения всех потоков
    for (std::thread& th : threads) {
        th.join();
    }

    // Сортировка и вывод результатов
    resultList.sort();
    for (const std::string& str : resultList) {
        std::cout << str << std::endl;
    }

    return 0;
}
