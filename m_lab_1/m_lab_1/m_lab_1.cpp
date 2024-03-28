#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <string>

void sleepAndPrint(const std::string& str) {
    // 1 секунда на символ пропорционально длине строки
    std::this_thread::sleep_for(std::chrono::seconds(str.length()));
    std::cout << str << std::endl;
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

    // поток для каждой строки в векторе
    for (const std::string& str : strings) {
        threads.emplace_back(sleepAndPrint, str);
    }

    for (std::thread& th : threads) {
        th.join();
    }

    return 0;
}
