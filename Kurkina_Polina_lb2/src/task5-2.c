#define _GNU_SOURCE
#include <err.h>
#include <sched.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/resource.h>



int main() {
    printf("Родительский процесс: PID = %d, PPID = %d\n", getpid(), getppid());
    int nice_value = getpriority(PRIO_PROCESS, 0);
    printf("Nice value of the current process: %d\n", nice_value);
    
    
    pid_t pid1 = fork();

    if (pid1 == 0) {
        
        nice(5); // Увеличиваем nice value на 5 (снижаем приоритет)
        for (int i=0; i<100; i++){
            putchar('1');
            fflush(stdout);
        }
        int nice_value = getpriority(PRIO_PROCESS, 0);
        printf("\n Nice value of the current process: %d\n", nice_value);
        exit(0);

    } 
    

    pid_t pid2 = fork();
    if (pid2 == 0) { // Дочерний процесс
        nice(19); // Увеличиваем nice value на 5 (снижаем приоритет)
        
        for (int i=0; i<100; i++){
            putchar('2');
            fflush(stdout);
        }
        int nice_value = getpriority(PRIO_PROCESS, 0);
        printf("\n Nice value of the current process: %d\n", nice_value);
        exit(10);

    }
    

    pid_t pid3 = fork();
    if (pid3 == 0) {
        
        nice(1); // Увеличиваем nice value на 5 (снижаем приоритет)
        for (int i=0; i<100; i++){
            putchar('3');
            fflush(stdout);
        }
        int nice_value = getpriority(PRIO_PROCESS, 0);
        printf("\n Nice value of the current process: %d\n", nice_value);
        exit(20); 
    }

    system("ps -o pid,comm,policy,pri,ni ");
    


    int status;
    pid_t terminated_pid;
    
    terminated_pid = waitpid(pid1, &status, 0);
    terminated_pid = waitpid(pid2, &status, 0);
    terminated_pid = waitpid(pid3, &status, 0);

    return 0;
}