#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <stdlib.h>
#include <errno.h>

void run_process(char name, FILE *file) {
    while (1) {
        fputc(name, file);
        fflush(stdout);
        
        //sleep(1);
    }
}


void print_policy(){
    int policy = sched_getscheduler(0);
    if (policy == -1) {
        perror("sched_getscheduler");
        return;
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
        case SCHED_BATCH:
            policy_name = "SCHED_BATCH";
            break;
        case SCHED_IDLE:
            policy_name = "SCHED_IDLE";
            break;
        case SCHED_DEADLINE:
            policy_name = "SCHED_DEADLINE";
            break;
        default:
            policy_name = "Unknown";
            break;
    }

    printf("Политика планирования текущего процесса (PID = %d): %s\n", getpid(), policy_name);
}


int main() {
    system("echo \"run\" > output.txt");
    system("chmod 777 output.txt");
    FILE *file = fopen("output.txt", "w");
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }
    struct sched_param param;
    param.sched_priority = 50; // Устанавливаем приоритет

    pid_t pid1 = fork();
    if (pid1 == 0) {        
        //param.sched_priority = 1;
        sched_setscheduler(0, SCHED_OTHER, &param);
        print_policy();
        run_process('1', file);
        
        exit(0);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        //param.sched_priority = 50;
        sched_setscheduler(0, SCHED_RR, &param);
        print_policy();
        run_process('2', file);
        exit(0);
    }

    pid_t pid3 = fork();
    if (pid3 == 0) {
        //param.sched_priority = 99;
        sched_setscheduler(0, SCHED_FIFO, &param);
        print_policy();
        run_process('3', file);
        exit(0);
    }

    
    sleep(6);
    
    kill(pid1, SIGTERM);
    kill(pid2, SIGTERM);
    kill(pid3, SIGTERM);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    fclose(file);
    FILE *file_new = fopen("output.txt", "r");

    int count_1 = 0; 
    int count_2 = 0;
    int count_3 = 0;
    char ch;

    // Читаем файл посимвольно
    while ((ch = fgetc(file_new)) != EOF) {
        if (ch == '1') {
            count_1++; 
        }
        else if (ch == '2') {
            count_2++; 
        }
        else if (ch == '3') {
            count_3++; 
        }
    }

    printf("count of 1: %d\ncount of 2^ %d\ncount of 3^ %d\n", count_1, count_2, count_3);
    fclose(file_new);
    return 0;
}