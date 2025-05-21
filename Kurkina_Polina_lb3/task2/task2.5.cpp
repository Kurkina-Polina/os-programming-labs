
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <csignal>
#include <signal.h>

int run = 4;

void USRSIG_handler(int){
    std::cout<<"USRSIG ";
    run -=1;
}

void* func_thread22(void* arg){

    std::cout<<"thread24 tid: ";
    std::cout<<gettid()<<std::endl;

    struct sigaction action{};
    struct sigaction oldact{};
    action.sa_handler = USRSIG_handler;
    
    sigaction(SIGTSTP, &action, &oldact);
    
    while(run){
        std::cout<<"thread24"<<std::endl;
        sleep(1);
    }
    std::cout<<"thread24 is over"<<std::endl;

    sigaction(SIGTSTP, &oldact, nullptr);
    return nullptr;
}


int main() {

   
    pthread_t thread1;
    int res1 = pthread_create(&thread1, nullptr, &func_thread22, nullptr);

    if(res1 !=0){
        std::cout<<"error in create thread";
    }

    while(1){
        sleep(1);
    }
    void* res;
    pthread_join(thread1, &res);



    return 0;
}
