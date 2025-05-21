
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <csignal>
#include <signal.h>

int run_thread22 = 1;

void USRSIG_handler(int){
    std::cout<<"USRSIG ";
    run_thread22 -=1;
}

void* func_thread22(void* arg){

    std::cout<<"thread22 tid: ";
    std::cout<<gettid()<<std::endl;

    struct sigaction action{};
    struct sigaction oldact{};
    action.sa_handler = USRSIG_handler;
    sigaction(SIGINT, &action, &oldact);
    
    while(run_thread22){
        std::cout<<"thread22"<<std::endl;
        sleep(1);
    }
    std::cout<<"thread22 is over"<<std::endl;
    return nullptr;
}


int main() {

   
    pthread_t thread1;
    int res1 = pthread_create(&thread1, nullptr, &func_thread22, nullptr);

    if(res1 !=0){
        std::cout<<"error in create thread";
    }
    void* res;
    pthread_join(thread1, &res);



    return 0;
}
