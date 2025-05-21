
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <csignal>
#include <signal.h>
#include <unistd.h>
#include <format>
#include <pthread.h>
#include <atomic>


void my_handler(int sig, siginfo_t *info, void *) {
    std::cout<<"signal reseived "<< sig << std::endl;
    std::cout<<"signal info "<< info->si_value .sival_int << std::endl;
}



int main() {
    struct sigaction action{};
    action.sa_sigaction = my_handler;
    action.sa_flags = SA_SIGINFO; 
    for (auto sig = SIGRTMIN; sig<=SIGRTMAX; sig++){
        sigaction(sig, &action, nullptr);
    }

    pid_t pid = fork();
    
    if (pid !=0){
        for (auto sig = SIGRTMAX; sig>=SIGRTMIN; sig--){
            const union sigval value = { .sival_int = sig };
            sigqueue(pid,sig, value);
        }
        
    }
    
    else{
        sleep(10);
    }
    
    return 0;
}
