#define _GNU_SOURCE
#include <err.h>
#include <sched.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <errno.h>

pthread_t t1, t2, t3;
FILE *file;

void* mythread(void * arg){
    char const* name = arg;
    printf("%s is started TID %ld PID %d\n", name, syscall(SYS_gettid), getpid());
    
    struct sched_param param;
    int policy = SCHED_RR;
    if(*name == '1'){
        
        param.sched_priority = 1;
    }
    else if(*name == '2'){
        
        param.sched_priority = 99;
    }
    else if(*name == '3'){
        param.sched_priority = 1;
    }
    
    if (pthread_setschedparam(pthread_self(), policy, &param) != 0) {
        perror("pthread_setschedparam");
    }

   
    
    // Получаем параметры планирования для текущей нити
    pthread_getschedparam(pthread_self(), &policy, &param);

    // Выводим политику и приоритет
    printf(" %c: Политика планирования: %s\n", *name,
           (policy == SCHED_FIFO)  ? "SCHED_FIFO" :
           (policy == SCHED_RR)    ? "SCHED_RR" :
           (policy == SCHED_OTHER) ? "SCHED_OTHER" : "Неизвестно");
    printf("  Приоритет: %d\n", param.sched_priority);
    



    
    
    for (int i = 0; i < 50; i++) {
        fputc(*name, file);
        fflush(file);  // Очищаем буфер, чтобы сообщения сразу записывались в файл, по умолчанию оно кешируется для оптимизации
        sleep(1);  
    }

    
        

    return NULL;
    
}

int main(){
    file = fopen("output.txt", "w");
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }
    
    

    
    pthread_create(&t1, NULL, &mythread, "1");
    pthread_create(&t2, NULL, &mythread, "2");
    pthread_create(&t3, NULL, &mythread, "3");

    //end of work
    printf("killing \n");
    
    pthread_join(t1, NULL); // переводит в состояние ожидания текущий поток пока не завершится другой
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    //sleep(20);
    fclose(file);
    return 0;
}