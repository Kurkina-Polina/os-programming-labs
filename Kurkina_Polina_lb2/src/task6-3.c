#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
//task 6.3 with exelc and fork
void handler(int sig) {
    printf("Process %d caught signal %d\n", getpid(), sig);
}


int main() {
    printf("Родительский процесс: PID = %d, PPID = %d\n", getpid(), getppid());
    // Устанавливаем обработчик SIGUSR1
    if (signal(SIGUSR1, handler) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }

    pid_t pid1 = fork();

    if (pid1 == 0) {
        printf("Дочерний процесс : PID = %d, PPID = %d\n", getpid(), getppid());
        execl("./son.out", "son.out", NULL);
       
        exit(0);

    } 
    pid_t pid2 = fork();
    if (pid2 == 0) {
        printf("Дочерний процесс : PID = %d, PPID = %d\n", getpid(), getppid());
        sleep(1);
        raise(SIGUSR1);
        
        
        exit(0);

    } 
    
    raise(SIGUSR1);
    kill(pid1, SIGUSR1);
    
    wait(NULL);
    printf("Родительский процесс: Завершение работы.\n");
    
    return 0;
}