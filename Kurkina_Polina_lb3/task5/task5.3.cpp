 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <vector>
 
std::vector<int> memory;
pthread_mutex_t lock ;

void* thread1(void *arg){
    for (int i=0; i<10; i++){
        pthread_mutex_lock( &lock );
        memory.push_back(i);
        pthread_mutex_unlock( &lock );
    }
    return nullptr;
     
}

void* thread2(void *arg){
    for (int i=0; i<10; i++){
        pthread_mutex_lock( &lock );
        std::cout<< memory.back()<<std::endl;
        pthread_mutex_unlock( &lock );
    }
    return nullptr;
}

int main(){
    pthread_t t1, t2;
    pthread_mutex_init(&lock, NULL);
    pthread_create(&t1, NULL, &thread1, NULL);
    pthread_create(&t2, NULL, &thread2, NULL);


    pthread_join(t1, NULL); 
    pthread_join(t2, NULL);
}