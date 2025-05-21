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
#include <string.h>
//task 6.1 with onle fork 

int main() {
    printf("Родительский процесс: PID = %d, PPID = %d\n", getpid(), getppid());
    int fd = open("output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if(fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    /* Дублируем fd на номер 3 */
    if(dup2(fd, 3) == -1) {
        perror("dup2");
        exit(EXIT_FAILURE);
    }

    /* Родитель пишет до exec() */
    const char *parentStr = "Parent writes before exec\n";
    write(3, parentStr, strlen(parentStr));


    pid_t pid1 = fork();

    if (pid1 == 0) {
        printf("Дочерний процесс : PID = %d, PPID = %d\n", getpid(), getppid());
        execl("./son.out", "son.out", NULL);
        printf("Дочерний процесс : Завершение работы.\n");
        exit(0);

    } 
    
    
    
    wait(NULL);
    printf("Родительский процесс: Завершение работы.\n");
    close(fd);
    return 0;
}