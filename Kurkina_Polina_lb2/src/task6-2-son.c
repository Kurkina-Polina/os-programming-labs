#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sched.h>

int main() {
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
    return 0;
}