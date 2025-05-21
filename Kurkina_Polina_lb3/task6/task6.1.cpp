#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    int fd[2];  
    pipe(fd);   

    if (fork() == 0) {  
        close(fd[1]);   
        char buf[100];
        read(fd[0], buf, sizeof(buf));  
        printf("Child received: %s\n", buf);
        close(fd[0]);
    } else {  
        close(fd[0]);  
        const char* msg = "Parent";
        write(fd[1], msg, strlen(msg) + 1);  
        close(fd[1]);
        wait(NULL);  
    }
    return 0;
}