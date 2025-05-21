 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>


struct data_t {
    char buf[128];
    pthread_spinlock_t lock ;
};

 
int main(){
    const int size = 4 * 1024;
    const char* name = "task51";
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
    
    pthread_spin_init(&ptr->lock, true);

    if(fork() == 0){
        int offset = 0;
        for(int j=0;j<10;j++) {
            
            pthread_spin_lock(&ptr->lock);
            offset+= snprintf(ptr->buf+offset, sizeof(ptr->buf)-offset, "%d\n", j);
            pthread_spin_unlock(&ptr->lock);
            
        }

    } else {
        
        for(int j=0;j<1;j++) {
            pthread_spin_lock(&ptr->lock);
            std::cout << ptr->buf<<std::flush;
            pthread_spin_unlock(&ptr->lock);
        }

    }

//     pthread_spin_destroy(&ptr->lock);
//     munmap(ptr, size);
//     close(shm_fd);
//     shm_unlink(name);
}