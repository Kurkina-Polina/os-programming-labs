#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <unistd.h>
#include <cstring>

int main() {
    const char* queue_name = "/test_mailbox";  // Имя очереди
    mqd_t mq;

    // Атрибуты очереди (макс. 10 сообщений по 256 байт)
    struct mq_attr attr = {
        .mq_maxmsg = 10,        
        .mq_msgsize = 256,      
    };

    // Процесс-отправитель (не ждёт получателя)
    if (fork() == 0) {
        mq = mq_open(queue_name, O_WRONLY | O_CREAT, 0666, &attr);
        const char* msg = "Сообщение для почтового ящика";
        mq_send(mq, msg, strlen(msg) + 1, 0);  // Приоритет = 0
        std::cout << "Отправлено: " << msg << std::endl;
        mq_close(mq);
        exit(0);
    }

    // Процесс-получатель (может быть запущен позже)
    else {
        sleep(2);  // Имитация задержки
        mq = mq_open(queue_name, O_RDONLY);
        char buf[256];
        unsigned int prio;
        ssize_t bytes = mq_receive(mq, buf, sizeof(buf), &prio);
        std::cout << "Получено (" << bytes << " байт): " << buf << std::endl;
        

        sleep(60);
        mq_close(mq);
        mq_unlink(queue_name);  
    }

    return 0;
}