
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <csignal>
#include <signal.h>
#include <stdlib.h>
int run_thread2 = 1;

void* func_thread1(void* arg){
    pthread_t* thread2 = (pthread_t*)arg;
    std::cout<<"thread1 tid: "<<std::endl;
    std::cout<<gettid()<<std::endl;

    std::cout<<"started"<<std::endl;
    system("ps -o uid,gid,ruid,pid,ppid,pgid,tid,vsz,stat,command -T");
    
    pthread_kill(*thread2, SIGUSR1);

    std::cout<<"SIGUSR"<<std::endl;
    system("ps -o uid,gid,ruid,pid,ppid,pgid,tid,vsz,stat,command -T");
    
    
    return nullptr;
}


void USRSIG_handler(int signo){
    std::cout<<"USRSIG is working\n ttid: "<<std::endl;
    std::cout<<gettid()<<std::endl;
    run_thread2 = 0;
}


void* func_thread2(void* arg){
    struct sigaction action{};
    struct sigaction oldact{};
    action.sa_handler = USRSIG_handler;

    sigaction(SIGUSR1, &action, &oldact);

    std::cout<<"thread2 tid: "<<std::endl;
    std::cout<<gettid()<<std::endl;
    while(run_thread2){
        sleep(1);
    }
    std::cout<<"thread2 is over"<<std::endl;
    return nullptr;
}

int main() {

    
    
    pthread_t thread1, thread2;
    int res2 = pthread_create(&thread2, nullptr, &func_thread2, nullptr);
    int res1 = pthread_create(&thread1, nullptr, &func_thread1, &thread2);

    if(res1 !=0 || res2!=0){
        std::cout<<"error in create thread";
    }
    
    void* res;
    pthread_join(thread1, &res);

    std::cout<<"the end"<<std::endl;
    system("ps -o uid,gid,ruid,pid,ppid,pgid,tid,vsz,stat,command -T");
    


    return 0;
}
