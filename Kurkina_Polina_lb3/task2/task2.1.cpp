#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <csignal>
#include <signal.h>

int run = 1;

void USRSIG_handler(int){
    std::cout<<"USRSIG ";
    run -=1;
}

void run_process(){

    std::cout<<"proc pid: ";
    std::cout<<getpid()<<std::endl;

    struct sigaction action{};
    struct sigaction oldact{};
    action.sa_handler = USRSIG_handler;
    sigaction(SIGINT, &action, &oldact);
    
    while(run){
        std::cout<<"working"<<std::endl;
        sleep(1);
    }
    std::cout<<"proc is over"<<std::endl;
    
}


int main() {

    pid_t pid1 = fork();
    if (pid1 == 0) {
        run_process();
        exit(0);
    }
    
    
    wait(NULL);


    return 0;
}
