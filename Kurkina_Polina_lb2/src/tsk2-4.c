#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
//task 2.4

int main() {
    printf("Родительский процесс: PID = %d, PPID = %d\n", getpid(), getppid());

    // Ситуация 1: Нормальное завершение
    printf("\nСитуация 1: Нормальное завершение\n");
    pid_t pid1 = fork();

    if (pid1 < 0) {
        fprintf(stderr, "Ошибка при создании дочернего процесса\n");
        exit(1);

    } else if (pid1 == 0) { // Дочерний процесс
        printf("Дочерний процесс 1: PID = %d, PPID = %d\n", getpid(), getppid());
        sleep(5); // Имитация работы
        printf("Дочерний процесс 1: Завершение работы.\n");
        exit(0);

    } else { // Родительский процесс
        printf("Родительский процесс: Дочерний процесс 1 создан с PID = %d\n", pid1);
        system("ps -l | tee  situation1.txt");
        wait(NULL); // Ожидание завершения дочернего процесса
        printf("Родительский процесс: Дочерний процесс 1 завершился.\n");
    }


    // Ситуация 3: Состояние зомби
    printf("\nСитуация 3: Состояние зомби\n");
    pid_t pid3 = fork();

    if (pid3 < 0) {
        fprintf(stderr, "Ошибка при создании дочернего процесса\n");
        exit(1);

    } else if (pid3 == 0) { // Дочерний процесс
        printf("Дочерний процесс 3: PID = %d, PPID = %d\n", getpid(), getppid());
        sleep(5); // Имитация работы
        printf("Дочерний процесс 3: Завершение работы.\n");
        exit(0);

    } else { // Родительский процесс
        printf("Родительский процесс: Дочерний процесс 3 создан с PID = %d\n", pid3);
        system("ps -l | tee  situation3_before_zombie.txt");
        sleep(10); // Родительский процесс не ожидает завершения дочернего
        system("ps -l | tee situation3_after_zombie.txt");
        printf("Родительский процесс: Завершение работы.\n");
    }


    // Ситуация 2: Смена родителя
    printf("\nСитуация 2: Смена родителя\n");
    pid_t pid2 = fork();

    if (pid2 < 0) {
        fprintf(stderr, "Ошибка при создании дочернего процесса\n");
        return EXIT_FAILURE;
    } else if (pid2 == 0) { // Дочерний процесс
        printf("Дочерний процесс 2: PID = %d, PPID = %d\n", getpid(), getppid());
        sleep(10); // Имитация работы
        printf("Дочерний процесс 2: Новый PPID = %d\n", getppid()); // Новый родитель
        printf("Дочерний процесс 2: Завершение работы.\n");
    } else { // Родительский процесс
        printf("Родительский процесс PID = %d, PPID = %d: Дочерний процесс 2 создан с PID = %d\n",  getpid(), getppid(), pid2);
        system("ps -l | tee  situation2_before_exit.txt");
        sleep(2); // Родительский процесс завершается раньше дочернего
        printf("Родительский процесс: Завершение работы.\n");
    }

    return 0;
}