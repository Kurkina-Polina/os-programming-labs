#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
//task 2.2 and also used for 2.3

int main() {
    pid_t pid;
    pid_t ppid = getpid(); // Получаем PID родительского процесса
    printf("Дочерний процесс: PID = %d, PPID = %d\n", getpid(), getppid());
    // Фиксируем состояние таблицы процессов в дочернем процессе
    //system("ps -l > output_son.txt");
    
    //Долгое вычисление
    for (long i = 0; i < 10; i++){
      int result_son = 20 + i;
      //printf("Дочерний процесс: Итерация цикла = %d\n", result_son);
      //system("ps -l >> output_son.txt");
      sleep(3); //задержка 3 секунды.
    }
    printf("Дочерняя Программа завершена.\n");
    exit(0);
  }
  