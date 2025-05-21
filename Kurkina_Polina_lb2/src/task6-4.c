#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
//task 6.4

#define STACK_SIZE (1024 * 1024)


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


int child_func(void *arg) {
    printf("Дочерний процесс: PID = %d, PPID = %d\n", getpid(), getppid());
    int fd = *(int *)arg;
    const char *childStr = "Child clone \n";
    write(fd, childStr, strlen(childStr));
    print_policy();
    return 0;
}

int main() {
    printf("Родительский процесс: PID = %d, PPID = %d\n", getpid(), getppid());
    print_policy();
    int fd = open("output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if(fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Запись родителя
    const char *parentStr = "Parent writes before clone\n";
    write(fd, parentStr, strlen(parentStr));

    // Выделяем стек для дочернего процесса
    char *stack = malloc(STACK_SIZE);
    if (!stack) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    char *stackTop = stack + STACK_SIZE;

    // Используем clone() с флагом CLONE_FILES, чтобы файлы были разделяемыми
    pid_t pid = clone(child_func, stackTop, CLONE_FILES | SIGCHLD, &fd);
    if (pid == -1) {
        perror("clone");
        free(stack);
        exit(EXIT_FAILURE);
    }

    // Дополнительная запись родителя после clone()
    const char *parentStr2 = "Parent writes after clone\n";
    write(fd, parentStr2, strlen(parentStr2));
    wait(NULL);
    close(fd);
    free(stack);
    return 0;
}