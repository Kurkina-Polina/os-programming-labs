#define _GNU_SOURCE
#include <err.h>
#include <sched.h>
#include <signal.h>
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
#include <string.h>
#include <errno.h>

pthread_t t1, t2, t3;
FILE *file;

void* mythread(void * arg){
    char const* name = arg;
    printf("%s is started\n", name);
    fprintf(file, "%s is started TID %ld PID %d\n", name, syscall(SYS_gettid), getpid());
    fflush(file);
    
    for (int i = 0; i < 5; i++) {
        fprintf(file, "%s: Message %d\n", name, i);
        fflush(file);  // Очищаем буфер, чтобы сообщения сразу записывались в файл, по умолчанию оно кешируется для оптимизации
        sleep(1);  
    }

    int policy;
    struct sched_param param;
    // Получаем параметры планирования для текущей нити
    pthread_getschedparam(pthread_self(), &policy, &param);

    // Выводим политику и приоритет
    fprintf(file, "%s\n", name);
    fprintf(file, "  Политика планирования: %s\n",
           (policy == SCHED_FIFO)  ? "SCHED_FIFO" :
           (policy == SCHED_RR)    ? "SCHED_RR" :
           (policy == SCHED_OTHER) ? "SCHED_OTHER" : "Неизвестно");
    fprintf(file, "  Приоритет: %d\n", param.sched_priority);
    fflush(file);
        

    return NULL;
    
}

int mythread_int(void * arg){
    char const* name = arg;
    printf("%s is started\n", name);
    fprintf(file, "%s is started TID %ld PID %d\n", name, syscall(SYS_gettid), getpid());
    fflush(file);
    

    for (int i = 0; i < 5; i++) {
        fprintf(file, "%s: Message %d\n", name, i);
        fflush(file);  // Очищаем буфер, чтобы сообщения сразу записывались в файл, по умолчанию оно кешируется для оптимизации
        sleep(1);
        
    }

    int policy;
    struct sched_param param;
    // Получаем параметры планирования для текущей нити
    pthread_getschedparam(pthread_self(), &policy, &param);

    // Выводим политику и приоритет
    
    fprintf(file, "  Политика планирования: %s\n",
           (policy == SCHED_FIFO)  ? "SCHED_FIFO" :
           (policy == SCHED_RR)    ? "SCHED_RR" :
           (policy == SCHED_OTHER) ? "SCHED_OTHER" : "Неизвестно");
    fprintf(file, "  Приоритет: %d\n", param.sched_priority);
    
        

    return 0;
    
}

#define STACK_SIZE (1024 * 1024) 

pid_t child_pid;
pid_t parent_pid;

int main(){
    file = fopen("output.txt", "w");
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }
    /* Allocate memory to be used for the stack of the child. */
    char            *stack;         /* Start of stack buffer */
    char            *stackTop;      /* End of stack buffer */
    stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
    stackTop = stack + STACK_SIZE;  /* Assume stack grows downward */
    if (stack == NULL) {
        perror("Failed to open file");
        return 1;
    }

    char            *stack2;         /* Start of stack buffer */
    char            *stackTop2;      /* End of stack buffer */
    stack2 = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
    stackTop2 = stack2 + STACK_SIZE;  /* Assume stack grows downward */
    if (stack2 == NULL) {
        perror("Failed to open file");
        return 1;
    }
    
    

    //system("ps -o uid,gid,ruid,pid,ppid,pgid,tid,vsz,stat,command -T");
    pthread_create(&t1, NULL, &mythread, "thread1");
    pthread_create(&t2, NULL, &mythread, "thread2");
    pthread_create(&t3, NULL, &mythread, "thread3");
    
    
    // для каждого потока свой стек
    // флаги  CLONE_VM |  CLONE_FS | CLONE_FILES  нужны чтобы у потоков был доступ к одним и тем же файловым дескрипторам, к фс и прочее
    // флаги CLONE_THREAD  | CLONE_PARENT_SETTID |CLONE_CHILD_CLEARTID необходимы чтобы создавался именно поток с таким же pid ppid и прочее, без этих флагов будет создаваться новый процесс а не поток

    pid_t t4 = clone(mythread_int, stackTop,
        CLONE_VM |  CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_SYSVSEM | CLONE_THREAD  | CLONE_PARENT_SETTID |
        CLONE_CHILD_CLEARTID,
        "thread4");

    pid_t t5 = clone(mythread_int, stackTop2,
        CLONE_VM |  CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_SYSVSEM | CLONE_THREAD  | CLONE_PARENT_SETTID |
                 CLONE_CHILD_CLEARTID, "thread5");

    
    system("ps -o uid,gid,ruid,pid,ppid,pgid,tid,vsz,stat,command -T");
    //end of work
    printf("killing \n");
    

    pthread_join(t1, NULL); // переводит в состояние ожидания текущий поток пока не завершится другой
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    int status;
    //pthread_join не применим в потокам созданным с clone тк принимают значение в формате pthread_t поэтому используем waitpid
    // если не ожидать совсем исполнение других потоков, то главный поток завершится и все остальные потоки соответсвенно тоже
    waitpid(t4, &status, 0);
    waitpid(t5, &status, 0);

    
    fclose(file);
    system("ps -o uid,gid,ruid,pid,ppid,pgid,tid,vsz,stat,command -T");
    return 0;
}

/* Выполнение программы:
polina@polina-UX303UA:~/OS/Kurkina_Polina_lb2/scr$ gcc task4.c && ./a.out
thread1 is started
thread2 is started
thread3 is started
thread4 is started
thread5 is started
  UID   GID  RUID     PID    PPID    PGID     TID    VSZ STAT COMMAND
 1000  1000  1000   40568   40560   40568   40568  20208 Ss   bash
 1000  1000  1000  111343    2069  111342  111343   2548 S    son.out
 1000  1000  1000  124793   40568  124793  124793  94852 Sl+  ./a.out
 1000  1000  1000  124793   40568  124793  124794  94852 Sl+  ./a.out
 1000  1000  1000  124793   40568  124793  124795  94852 Sl+  ./a.out
 1000  1000  1000  124793   40568  124793  124796  94852 Sl+  ./a.out
 1000  1000  1000  124793   40568  124793  124797  94852 Sl+  ./a.out
 1000  1000  1000  124793   40568  124793  124798  94852 Sl+  ./a.out
 1000  1000  1000  124799  124793  124793  124799   2800 S+   sh -c -- ps -o uid,gid,ruid,pid,ppid,pgid,tid,vsz,stat,command -T
 1000  1000  1000  124800  124799  124793  124800  13120 R+   ps -o uid,gid,ruid,pid,ppid,pgid,tid,vsz,stat,command -T
killing 
  UID   GID  RUID     PID    PPID    PGID     TID    VSZ STAT COMMAND
 1000  1000  1000   40568   40560   40568   40568  20208 Ss   bash
 1000  1000  1000  111343    2069  111342  111343   2548 S    son.out
 1000  1000  1000  124793   40568  124793  124793  94852 S+   ./a.out
 1000  1000  1000  124802  124793  124793  124802   2800 S+   sh -c -- ps -o uid,gid,ruid,pid,ppid,pgid,tid,vsz,stat,command -T
 1000  1000  1000  124803  124802  124793  124803  12988 R+   ps -o uid,gid,ruid,pid,ppid,pgid,tid,vsz,stat,command -T
polina@polina-UX303UA:~/OS/Kurkina_Polina_lb2/scr$ cat output.txt 
thread1 is started TID 124794 PID 124793
thread3 is started TID 124796 PID 124793
thread4 is started TID 124797 PID 124793
thread4: Message 0
thread2 is started TID 124795 PID 124793
thread2: Message 0
thread5 is started TID 124798 PID 124793
thread1: Message 0
thread5: Message 0
thread3: Message 0
thread4: Message 1
thread4: Message 1
thread5: Message 1
thread2: Message 1
thread1: Message 1
thread3: Message 1
thread4: Message 2
thread4: Message 2
thread5: Message 2
thread2: Message 2
thread1: Message 2
thread3: Message 2
thread4: Message 3
thread2: Message 3
thread3: Message 3
thread1: Message 3
thread4: Message 4
thread3: Message 4
thread2: Message 4
thread5: Message 4
thread1: Message 4
thread2
  Политика планирования: SCHED_OTHER
  Приоритет: 0
  Политика планирования: SCHED_OTHER
  Приоритет: 0
thread3
  Политика планирования: SCHED_OTHER
  Приоритет: 0
  Политика планирования: SCHED_OTHER
  Приоритет: 0
thread1
  Политика планирования: SCHED_OTHER
  Приоритет: 0
*/