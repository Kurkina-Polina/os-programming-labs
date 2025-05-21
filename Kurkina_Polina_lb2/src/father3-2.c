#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
//task 3.2 

int main() {
    printf("Родительский процесс: PID = %d, PPID = %d\n", getpid(), getppid());
    signal(SIGCHLD,SIG_IGN);
    
    pid_t pid1 = fork();

    if (pid1 == 0) {
        printf("Дочерний процесс 1: PID = %d, PPID = %d\n", getpid(), getppid());
        execl("./son1.out", "son1.out", NULL);
        printf("Дочерний процесс 1: Завершение работы.\n");
        exit(0);

    } 
    sleep(1);
    

    pid_t pid2 = fork();
    if (pid2 == 0) { // Дочерний процесс
        printf("Дочерний процесс 2: PID = %d, PPID = %d\n", getpid(), getppid());
        execl("./son2.out", "son2.out", NULL);
        printf("Дочерний процесс 2: Завершение работы.\n");
        exit(10);

    }
    

    pid_t pid3 = fork();
    if (pid3 == 0) {
        printf("Дочерний процесс 3: PID = %d\n", getpid());
        execl("./son.out", "son.out", NULL);
        printf("Дочерний процесс 3: Завершение работы.\n");
        exit(20); 
    }


    sleep(3); // даем время процессам запуститься

    printf("Родительский процесс: Таблица процессов до отправки сигнала\n");
    system("ps -l | tee  situation3-1-1.txt");

    kill(pid1, SIGTERM);
     
    
    sleep(15);

    printf("Родительский процесс: Таблица процессов после отправки сигнала\n");
    system("ps -l | tee situation3-1-1.txt");

    int status;
    pid_t terminated_pid;
    
    terminated_pid = waitpid(pid2, &status, 0);
    if (terminated_pid == pid2) {
        if (WIFEXITED(status)) {
            printf("Родительский процесс: Дочерний процесс 2 завершился с кодом %d\n", WEXITSTATUS(status));
        }
    }

 
    terminated_pid = waitpid(pid3, &status, 0);
    if (terminated_pid == pid3) {
        if (WIFEXITED(status)) {
            printf("Родительский процесс: Дочерний процесс 3 завершился с кодом %d\n", WEXITSTATUS(status));
        }
    }

   
    printf("Родительский процесс: Таблица процессов после отправки сигнала\n");
    system("ps -l | tee situation3-1-1.txt");
    printf("Родительский процесс: Завершение работы.\n");
    return 0;
}