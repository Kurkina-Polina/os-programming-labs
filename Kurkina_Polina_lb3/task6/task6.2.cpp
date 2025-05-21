#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>
const char *fifo_path = "/tmp/myfifo";

void create_fifo(){
    if (mkfifo(fifo_path, 0666) == -1) {
        perror("mkfifo failed");
        exit(EXIT_FAILURE);
    }

    printf("FIFO created at %s\n", fifo_path);
    
}


void* write_fifo(void* arg){
    int fd = open(fifo_path, O_WRONLY);  

    if (fd == -1) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    const char *msg = "writer";
    write(fd, msg, strlen(msg) + 1);  
    close(fd);
    return nullptr;
    
    
}

void* read_fifo(void* arg){
    int fd = open(fifo_path, O_RDONLY);
    if (fd == -1) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }
    char buf[100];
    read(fd, buf, sizeof(buf));  
    printf("Received: %s\n", buf);
    close(fd);
    return nullptr;
    
}

int main() {
    
    create_fifo();
    pthread_t t1, t2;
    pthread_create(&t1, NULL, &write_fifo, NULL);
    pthread_create(&t2, NULL, &read_fifo, NULL);
   

    pthread_join(t1, NULL); 
    pthread_join(t2, NULL);
    
}