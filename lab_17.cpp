#include <iostream>
#include <list>
#include <string>
#include <mutex>
#include <thread>
#include <algorithm>
#include <chrono>
#include <windows.h>

std::list<std::string> list;
std::mutex listMutex;

void addString(const std::string& input) {
    std::lock_guard<std::mutex> lock(listMutex);
    if (input.empty()) {
        // Печать текущего состояния списка
        for (const auto& str : list) {
            std::cout << str << std::endl;
        }
    }
    else {
        // Добавление строки в список
        list.push_back(input);
    }
}

void sortList() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::lock_guard<std::mutex> lock(listMutex);
        list.sort();
        std::cout << "Список отсортирован." << std::endl;
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::thread sorter(sortList);
    sorter.detach();  // Отсоединяем поток сортировщика, чтобы он работал в фоновом режиме

    std::string input;
    while (true) {
        std::getline(std::cin, input);
        addString(input);
    }

    return 0;
}
