
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <csignal>
#include <signal.h>
#include <unistd.h>
#include <format>
#include <pthread.h>
#include <atomic>


std::atomic<int> a = 0;
 

void SIGINT_handler(int) {
    a++;
    sleep(2);   
}



int main() {
    struct sigaction action_SIGINT{};
    struct sigaction oldact_SIGINT{};
    action_SIGINT.sa_handler = SIGINT_handler;
    action_SIGINT.sa_flags = SA_NODEFER; 
    sigaction(SIGINT, &action_SIGINT, &oldact_SIGINT);
    
    
    while(1){
        while(a) {
            printf("SIGNAL SIGINT called!\n");
            a--;
        }
    }

    return 0;
}
