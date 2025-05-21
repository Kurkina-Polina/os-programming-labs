#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main() {
    printf("son2: PID = %d\n", getpid());

    // Игнорирование сигнала SIGTERM
    signal(SIGTERM, SIG_IGN);
    sleep(5);
    
    printf("son2: Завершение работы \n");
    return 0;
}