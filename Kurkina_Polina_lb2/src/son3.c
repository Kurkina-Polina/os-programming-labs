#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void signal_handler(int signum) {
    printf("Дочерний процесс 3: Получен сигнал %d\n", signum);
    exit(0); // Завершаем процесс после обработки сигнала
}

int main() {
    printf("son3: PID = %d\n", getpid());

    // Перехват и обработка сигнала SIGTERM
    signal(SIGTERM, signal_handler);

    while (1) {
        sleep(1);
    }

    printf("son3: Завершение работы \n");
    return 0;
}