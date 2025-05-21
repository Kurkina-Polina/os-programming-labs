
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <csignal>
#include <signal.h>

int run = 3 ;

void USRSIG_handler(int){
    std::cout<<"USRSIG "<<std::endl;
    for(int i=0; i<5; i++){
        sleep(1);
        std::cout<<"USRSIG "<<std::endl;
    }
}


void SIGINT_handler(int){
    std::cout<<"SIG INT " <<std::endl;
    run -=1 ;
}

void SIGTERM_handler(int){
    std::cout<<"SIG TERM " << std::endl;
}


int main() {
    struct sigaction action_SIGINT{};
    struct sigaction oldact_SIGINT{};
    action_SIGINT.sa_handler = SIGINT_handler;
    sigaction(SIGINT, &action_SIGINT, &oldact_SIGINT);

    struct sigaction action{};
    action.sa_handler = USRSIG_handler;
    sigaddset(&action.sa_mask, SIGINT); // маска для игнорирования сигнала sigint
    sigaction(SIGUSR1, &action, nullptr);

    struct sigaction action_TERM{};
    action_TERM.sa_handler = SIGTERM_handler;
    action_TERM.sa_flags = SA_RESETHAND; // флаг для восстановления обработчика после первого вызова
    sigaction(SIGTERM, &action_TERM, nullptr);

    while(run>0){
        sleep(1);
    }
    sigaction(SIGINT, &oldact_SIGINT, nullptr);
    
    while(1){
        sleep(1);
    }
   



    return 0;
}
