#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>

int main() {
    int min_priority = sched_get_priority_min(SCHED_FIFO);
    int max_priority = sched_get_priority_max(SCHED_FIFO);

    printf("Минимальный приоритет для SCHED_FIFO: %d\n", min_priority);
    printf("Максимальный приоритет для SCHED_FIFO: %d\n", max_priority);

    return 0;
}