#include <iostream>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");

    // Создаем именованные семафоры
    sem_t* semA = sem_open("/semaphoreA", O_CREAT, 0666, 1);
    sem_t* semB = sem_open("/semaphoreB", O_CREAT, 0666, 0);

    // Создаем новый процесс
    pid_t pid = fork();

    if (pid == 0) {
        // Дочерний процесс (процесс B)
        for (int i = 0; i < 10; ++i) {
            sem_wait(semB); // Ждем, пока разрешится семафор B
            cout << i + 1 << " cтрока текста дочернего процесса" << endl;
            sem_post(semA); // Устанавливаем семафор A для родительского процесса
        }
    }
    else if (pid > 0) {
        // Родительский процесс (процесс A)
        for (int i = 0; i < 10; ++i) {
            sem_wait(semA); // Ждем, пока разрешится семафор A
            cout << i + 1 << " cтрока текста родительского процесса" << endl;
            sem_post(semB); // Устанавливаем семафор B для дочернего процесса
        }
    }
    else {
        cerr << "Ошибка при создании дочернего процесса." << endl;
        return 1;
    }

    // Закрываем семафоры
    sem_close(semA);
    sem_close(semB);

    // Удаляем именованные семафоры
    sem_unlink("/semaphoreA");
    sem_unlink("/semaphoreB");

    return 0;
}
