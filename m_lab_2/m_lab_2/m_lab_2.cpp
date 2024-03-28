#include <iostream>
#include <filesystem>
#include <thread>
#include <windows.h>

namespace fs = std::filesystem;

void copyContents(const fs::path& source, const fs::path& destination) {
    try {
        // Создаем директории в пункте назначения, если они не существуют.
        if (!fs::exists(destination)) {
            fs::create_directories(destination);
        }
        // Копируем каждый элемент в директории.
        for (const auto& item : fs::directory_iterator(source)) {
            fs::path dest = destination / item.path().filename();
            if (fs::is_directory(item.status())) {
                // Рекурсивно копируем подкаталоги.
                fs::copy(item, dest, fs::copy_options::recursive);
            }
            else if (fs::is_regular_file(item.status())) {
                // Копируем файлы.
                fs::copy(item, dest, fs::copy_options::update_existing);
            }
        }
    }
    catch (std::exception& e) {
        // Обработка исключений, связанных с файловыми операциями.
        std::cerr << e.what() << '\n';
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    fs::path sourcePath = R"(C:\Users\SanchoKusika\Desktop\portfolio)";
    fs::path destinationPath = R"(C:\Users\SanchoKusika\Desktop\clone)";

    // Запускаем процесс копирования в отдельном потоке.
    std::thread copyThread(copyContents, sourcePath, destinationPath);
    // Ждем завершения копирования.
    copyThread.join();

    std::cout << "Копирование прошло успешно." << std::endl;

    return 0;
}
