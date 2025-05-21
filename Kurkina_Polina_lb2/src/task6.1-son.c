#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    const char *childStr = "Child exec writes\n";
    write(3, childStr, strlen(childStr));
    return 0;
}