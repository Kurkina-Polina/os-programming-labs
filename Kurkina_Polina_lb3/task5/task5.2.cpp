 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>
#include <semaphore.h>

struct data_t {
    char buf[128];
    sem_t sem;  ;
};

 
int main(){
    const int size = 4 * 1024;
    const char* name = "task52";
    int shm_fd;

    
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Failed open shm file");
        return EXIT_FAILURE;
    }
    if (ftruncate(shm_fd, size) == -1) {
        perror("Failed to truncate file");
        return EXIT_FAILURE;
    }

    data_t *ptr = (data_t*)mmap(0, size, PROT_WRITE|PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("Failed to mmap file");
        return EXIT_FAILURE;
    }
    
    
    sem_init(&ptr->sem, 1, 0);

    if(fork() == 0){
        for(int j=0;j<10;j++) {
            sem_wait(&ptr->sem);
            std::cout << ptr->buf;
            
        }

    } else {
        for(int j=0;j<10;j++) {
            snprintf(ptr->buf, sizeof(ptr->buf), "%d\n", j);
            sem_post(&ptr->sem);
            
        }
        

    }
    sem_destroy(&ptr->sem);    
//     pthread_spin_destroy(&ptr->lock);
//     munmap(ptr, size);
//     close(shm_fd);
//     shm_unlink(name);
}