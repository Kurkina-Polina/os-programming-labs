#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
   

    FILE *file = fopen(argv[1], "r");

    if (file == NULL) {
       printf("Ошибка: не удалось открыть файл %s.\n", argv[1]);
        return 0;
    }

    printf("Содержимое файла %s:\n", argv[1]);
    int ch;
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }

    fclose(file);
    return 0;
}

