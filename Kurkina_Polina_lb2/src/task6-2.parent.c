#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
//task 6.2 with exelc and fork


void print_policy(){
    int policy = sched_getscheduler(0);
    if (policy == -1) {
        perror("sched_getscheduler");
        
    }

    // Преобразуем политику в читаемый формат
    const char *policy_name;
    switch (policy) {
        case SCHED_OTHER:
            policy_name = "SCHED_OTHER";
            break;
        
        case SCHED_FIFO:
            policy_name = "SCHED_FIFO";
            break;
        case SCHED_RR:
            policy_name = "SCHED_RR";
            break;
    }

    printf("Политика планирования текущего процесса (PID = %d): %s\n", getpid(), policy_name);
}

int main() {
    printf("Родительский процесс: PID = %d, PPID = %d\n", getpid(), getppid());
    print_policy();


    pid_t pid1 = fork();

    if (pid1 == 0) {
        printf("Дочерний процесс : PID = %d, PPID = %d\n", getpid(), getppid());
        execl("./son.out", "son.out", NULL);
       
        exit(0);

    } 
    pid_t pid2 = fork();
    if (pid2 == 0) {
        printf("Дочерний процесс : PID = %d, PPID = %d\n", getpid(), getppid());
        print_policy();
        
        exit(0);

    } 
    
    
    
    wait(NULL);
    wait(NULL);
    printf("Родительский процесс: Завершение работы.\n");
    
    return 0;
}