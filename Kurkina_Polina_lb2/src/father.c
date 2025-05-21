#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
//task 2.2



int main() {

    printf("Родительский процесс: PID = %d, PPID = %d\n", getpid(), getppid());


    pid_t pid = fork();
    

    if (pid < 0) {
        fprintf(stderr, "Ошибка при создании дочернего процесса\n");
        exit(1);
    } else if (pid == 0) { // Дочерний процесс
        // Запускаем программу son.out
        execl("./son.out", "son.out", NULL);
        // Если execl вернул управление, значит произошла ошибка
        perror("execl");
        exit(1);
    } else { // Родительский процесс
        // Фиксируем состояние таблицы процессов после создания дочернего процесса
        system("ps -l > father_after_fork.txt");
        long result_father = 0;
        printf("Родительский процесс: Дочерний процесс создан с PID = %d\n", pid);
        for (int i = 0; i < 10; i++){
            result_father = result_father + i;
            printf("Родительский процесс: итерация цикла:  = %d\n", i);
            sleep(3); //задержка 3 секунды.
          }

        // Ожидаем завершения дочернего процесса
        wait(NULL);
        printf("Родительский процесс: Дочерний процесс завершился.\n");

        
    }
    printf("Родительский процесс: Завершение работы.\n");
    return 0;
  }
  