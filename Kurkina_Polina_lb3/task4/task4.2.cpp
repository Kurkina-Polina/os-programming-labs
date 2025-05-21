
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
    std::cout<<"signal received "<< sig << std::endl;
    std::cout<<"signal info "<< info->si_value .sival_int << std::endl;
}


static  auto const min = 0;
static  auto const max = 10;

int main() {
    struct sigaction action{};
    action.sa_sigaction = my_handler;
    action.sa_flags = SA_SIGINFO; 
    int signal[] = {1, 2, 3, 4, 5, 6, 8, 11, 13, 14, 15};
    for (auto sig = min; sig<=max; sig++){
        sigaction(signal[sig], &action, nullptr);
    }

    pid_t pid = fork();
    
    if (pid !=0){
        for (auto sig = max; sig>=min; sig--){
            const union sigval value = { .sival_int = sig };
            std::cout << "Senging signal " << signal[sig] << " ret = " << sigqueue(pid, signal[sig], value) << std::endl;
            
        }
        int res;
        waitpid(pid, &res, 0);
    }
    
    else{
        sleep(10);
    }
    
    return 0;
}
