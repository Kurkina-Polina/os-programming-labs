#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <errno.h>
//task 2.3

int main(int argc, char* argv[]) {

    printf("first arg is %s\n", argv[0]);
   
    pid_t ppid = getpid(); // Получаем PID родительского процесса
    printf("Родительский процесс: PID = %d, PPID = %d\n", getpid(), getppid());
    //pid_t pid = fork();

    //надо указывать путь
    execl("./son.out", "son.out", NULL);
    // int returnn = execl("yes", "yes", NULL);
    // if (returnn =-1) {
    //     printf("%s\n", strerror(errno));
    //     return EXIT_FAILURE;
    // }
    execlp("son.out", "son.out", NULL);
     //   сам ищет исполняемый файл
    // int rc = execlp("yes", "yes", NULL);
    // if (rc =-1) {
    //     printf("%s\n", strerror(errno));
    //     return EXIT_FAILURE;
    // }

    //можно задать окружение
    char const *env[] = {"MY_VAR=Hello", "PATH=/usr/bin", NULL};
    execle("/bin/ls", "ls", "-l", NULL, env);


    //используется массив аргументов
    char  *args[] = {"ls", "-l", NULL};
    execv("/bin/ls", args);
    return EXIT_SUCCESS;
  }
  