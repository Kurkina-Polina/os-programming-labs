#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <csignal>
#include <signal.h>
#include <atomic>
int run = 4;

void USRSIG_handler(int){
    std::cout<<"USRSIG ";
    run -=1;
}

void run_process(){

    std::cout<<"new proc pid: ";
    std::cout<<getpid()<<std::endl;

    struct sigaction action{};
    struct sigaction oldact{};
    action.sa_handler = USRSIG_handler;
    
    sigaction(SIGINT, &action, &oldact);
    
    while(run>0){
        std::cout<<"count of runs:" << run <<std::endl;
        sleep(1);
    }
    std::cout<<" is over"<<std::endl;

    sigaction(SIGINT, &oldact, nullptr);
    
}


int main() {
    // Блокируем SIGINT в родительском процессе
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigprocmask(SIG_BLOCK, &mask, NULL);
    
    pid_t pid1 = fork();
    if (pid1 == 0) {
        // Разблокируем SIGINT в дочернем процессе
        sigprocmask(SIG_UNBLOCK, &mask, NULL);
        run_process();
    }
    
    wait(NULL);
    sigprocmask(SIG_UNBLOCK, &mask, NULL);
    while(1){
        
        sleep(1);
    }

    return 0;
}
