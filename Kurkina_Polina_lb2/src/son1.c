
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main() {
    printf("son1: PID = %d\n", getpid());

    while (1) {
        sleep(1);
    }

    printf("son1: Завершение работы \n");
    return 0;
}