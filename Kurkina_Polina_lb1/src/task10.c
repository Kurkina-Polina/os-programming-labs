#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        puts("Неправильно указаны аргументы");
        return 0;
    }

    const char *filename = argv[1];

    // Открытие файла
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        puts("Ошибка открытия файла");
        return 0;    
    }

    // Чтение и вывод содержимого файла
    char buffer[1024];
    ssize_t bytesRead;
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        fprintf(stdout, "%.*s", (int)bytesRead, buffer);
    }

    if (bytesRead == -1) {
        puts("Ошибка чтения файла");
        close(fd);
        return 0;
    }

    close(fd);
    return 0;
}
