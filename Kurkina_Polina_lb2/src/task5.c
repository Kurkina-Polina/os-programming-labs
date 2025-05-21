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
#include <pthread.h>
#include <sys/syscall.h>
#include <string.h>
#include <errno.h>



int main() {
    printf("Родительский процесс: PID = %d, PPID = %d\n", getpid(), getppid());
    signal(SIGCHLD,SIG_IGN);
    
    pid_t pid1 = fork();

    if (pid1 == 0) {
        sleep(3);
        for (int i=0; i<100; i++){
            printf("1\n");
        }
        
        exit(0);

    } 
    sleep(1);
    

    pid_t pid2 = fork();
    if (pid2 == 0) { // Дочерний процесс
        sleep(2);
        for (int i=0; i<100; i++){
            printf("2\n");
        }
        exit(10);

    }
    

    pid_t pid3 = fork();
    if (pid3 == 0) {
        sleep(2);
        for (int i=0; i<100; i++){
            printf("3\n");
        }
        exit(20); 
    }

    system("ps -o pid,comm,policy,pri,ni ");
    sleep(15);


    int status;
    pid_t terminated_pid;
    
    terminated_pid = waitpid(pid1, &status, 0);
    terminated_pid = waitpid(pid2, &status, 0);
    terminated_pid = waitpid(pid3, &status, 0);

    return 0;
}