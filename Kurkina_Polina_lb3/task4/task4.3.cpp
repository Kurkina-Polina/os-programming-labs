
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
    std::format_to(str, "signal {} received \n", sig);
    write(STDOUT_FILENO, std::data(str), std::size(str));
}


static  auto const min = 0;
static  auto const max = 10;

int main() {
    struct sigaction action{};
    action.sa_sigaction = my_handler;
    action.sa_flags = SA_SIGINFO; 
    int signal[] = {SIGINT, SIGUSR1, SIGUSR2};
    for (auto sig : signal){
        sigaction(sig, &action, nullptr);
    }

    for (auto sig = SIGRTMIN; sig<=SIGRTMAX; sig++){
        sigaction(sig, &action, nullptr);
    }

    pid_t pid = fork();
    
    if (pid !=0){
        for (auto sig = SIGRTMAX; sig>=SIGRTMIN; sig--){
            const union sigval value = { .sival_int = sig };
            sigqueue(pid, sig, value);
            
        }
        for (auto sig  : signal){
            const union sigval value = { .sival_int = sig };
            sigqueue(pid, sig, value);
            
        }
        int res;
        waitpid(pid, &res, 0);
    }
    
    else{
        sleep(10);
    }
    
    return 0;
}
