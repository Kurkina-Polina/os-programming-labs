
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <csignal>
#include <signal.h>
#include <unistd.h>
#include <format>
#include <pthread.h>
#include <atomic>
#include <sys/wait.h>



void my_handler(int sig, siginfo_t *info, void *) {
    char str[128]{};
    std::format_to(str, "number {} received \n", info->si_value .sival_int );
    //std::string str = std::format("signal {} received \n", sig);
    write(STDOUT_FILENO, std::data(str), std::size(str));
}



int main() {
    struct sigaction action{};
    action.sa_sigaction = my_handler;
    action.sa_flags = SA_SIGINFO; 
    sigaction(SIGRTMIN, &action, nullptr);
    

    pid_t pid = fork();
    
    if (pid !=0){
        for (auto i = 0; i < 10; i++){
            const union sigval value = { .sival_int =  i};
            sigqueue(pid, SIGRTMIN, value);
            //std::cout << "Sending signal " << sig << " ret = " << sigqueue(pid, sig, value) << std::endl;
            
        }
        
        int res;
        waitpid(pid, &res, 0);
    }
    
    else{
        sleep(10);
    }
    
    return 0;
}
