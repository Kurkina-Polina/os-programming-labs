#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
//task 2.1

int main() {
  pid_t pid;
  pid_t ppid = getpid(); 
  pid = fork();

  if (pid < 0) {
    fprintf(stderr, "Fork failed\n");
    exit(1);
  } else if (pid == 0) { // Это дочерний процесс
    printf("Дочерний процесс: PID = %d, PPID = %d\n", getpid(), getppid());
    int result_son = 20;
    //Долгое вычисление
    for (long i = 0; i < 10; i++){
      result_son = result_son + i;
      printf("Дочерний процесс: Результат вычислений = %d\n", result_son);
      
      sleep(3); //задержка 3 секунды.
    }
    sleep(3); 
    printf("Дочерний процесс: Завершение работы.\n");
    exit(0);
  } else { // Это родительский процесс
    printf("Родительский процесс: PID = %d, PPID = %d\n", getpid(), getppid());
    
    int result_parent = 1;
    
    //Долгое вычисление
    for (long i = 0; i < 10; i++){
      result_parent = result_parent + i;
      printf("Родительский процесс: Результат вычислений = %d\n", result_parent);	
      sleep(3); 
    }	    
    sleep(2); 
    printf("Родительский процесс: Ожидание завершения дочернего процесса...\n");

    wait(NULL); // Ожидаем завершения дочернего процесса
    printf("Родительский процесс: Дочерний процесс завершился.\n");
  }

  printf("Программа завершена.\n");
  exit(0);
}
