#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
//task 3.1 

int main() {
    printf("Родительский процесс: PID = %d, PPID = %d\n", getpid(), getppid());

    
    printf("\nСитуация 1:  реакция на сигнал по умолчанию\n");
    pid_t pid1 = fork();

    if (pid1 == 0) {
        printf("Дочерний процесс 1: PID = %d, PPID = %d\n", getpid(), getppid());
        execl("./son1.out", "son1.out", NULL);
        printf("Дочерний процесс 1: Завершение работы.\n");
        exit(0);

    } 
    sleep(1);

    printf("\nСитуация 2: реакция игнорирования\n");
    pid_t pid2 = fork();
    if (pid2 == 0) { // Дочерний процесс
        printf("Дочерний процесс 2: PID = %d, PPID = %d\n", getpid(), getppid());
        execl("./son2.out", "son2.out", NULL);
        printf("Дочерний процесс 2: Завершение работы.\n");
        exit(0);

    }

    sleep(1);

    printf("\nСитуация 3: перехватывание и обработка сигнала\n");
    pid_t pid3 = fork();

    if (pid3 == 0) { // Дочерний процесс
        printf("Дочерний процесс 3: PID = %d, PPID = %d\n", getpid(), getppid());
        execl("./son3.out", "son3.out", NULL);
        printf("Дочерний процесс 3: Завершение работы.\n");
    }

    sleep(3); // даем время процессам запуститься

    printf("Родительский процесс: Таблица процессов до отправки сигнала\n");
    system("ps -l | tee  situation3-1-1.txt");

    kill(pid1, SIGTERM);
    kill(pid2, SIGTERM);
    kill(pid3, SIGTERM);
    
    sleep(15);

    printf("Родительский процесс: Таблица процессов после отправки сигнала\n");
    system("ps -l | tee situation3-1-1.txt");

    wait(NULL);
    wait(NULL);
    wait(NULL);

    printf("Родительский процесс: Завершение работы.\n");

    return 0;
}