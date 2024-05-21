#include <iostream>
#include <list>
#include <shared_mutex>
#include <thread>
#include <windows.h>

using namespace std;

class ListPer {
public:
    // Конструктор по умолчанию (пустой список)
    ListPer() {}

    // Параметризованный конструктор (инициализируется элементами)
    ListPer(const std::initializer_list<std::string>& elements) {
        for (auto element : elements) {
            add(element);
        }
    }

    ListPer(const ListPer& other) {
        for (const auto& entry : other.data_) {
            data_.push_back({ entry.string });
        }
    }

    // Потокобезопасная реализация пузырьковой сортировки (с использованием data_mutex_)
    void sort() {
        bool swapped;
        do {
            swapped = false;
            auto it = data_.begin();
            auto next = std::next(it);
            for (; next != data_.end(); ++it, ++next) {
                unique_lock lock1(it->mutex);
                unique_lock lock2(next->mutex);
                if (it->string > next->string) {
                    std::swap(it->string, next->string);
                    swapped = true;
                }
            }
        } while (swapped);
    }

    // Потокобезопасный получатель размера (позволяет избежать ненужной блокировки)
    size_t size() {
        shared_lock lock(data_mutex_);
        return data_.size();
    }

    // Потокобезопасный метод печати
    void print() {
        shared_lock lock(data_mutex_);
        for (const auto& element : data_) {
            std::cout << element.string << std::endl;
        }
    }

    // Потокобезопасный метод добавления нового элемента
    void add(std::string element) {
        unique_lock lock(data_mutex_);
        data_.push_back({ element });
    }

    struct SE {
        std::string string;
        std::shared_mutex mutex;

        SE(const std::string& element) : string(element), mutex(std::shared_mutex{}) {}
        SE(const SE& other) : string(other.string), mutex(std::shared_mutex{}) {}
    };

    std::list<SE> data_;
    std::shared_mutex data_mutex_;
};

void periodicallysorting(ListPer* list) {
    for (int i = 0; i < 5; i++) {
        this_thread::sleep_for(10s);
        cout << "Сортируем список данных...." << endl;
        list->sort();
        list->print();
    }
}

void userinput(ListPer* list) {
    string line;
    for (int i = 0; i < 20; i++) {
        getline(cin, line);
        if (line.empty()) {
            list->print();
            continue;
        }
        else {
            list->add(line);
        }
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    cout << "---------После ввода данных ожидайте от 4 до 5 секунд---------------" << endl;
    cout << "---------После 3 строк накладывается запрет на ввод строк-----------" << endl;
    cout << "---------------------------Введите данные---------------------------" << endl;
    ListPer* list = new ListPer();

    thread listenToUserThread(userinput, list);
    thread sortingThread(periodicallysorting, list);

    listenToUserThread.join();
    sortingThread.join();

    delete list;
}
