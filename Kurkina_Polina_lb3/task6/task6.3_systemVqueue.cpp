#include <iostream>
#include <cstring>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

struct Message {
    long mtype;         
    char mtext[256];     
};

int main() {
    key_t key = ftok("mailbox", 65); 
    int msgid = msgget(key, 0666 | IPC_CREAT);  

   
    if (fork() == 0) {
        Message msg;
        msg.mtype = 1;  
        strcpy(msg.mtext, "Сообщение для почтового ящика");
        
        msgsnd(msgid, &msg, sizeof(msg.mtext), 0);  
        std::cout << "Сообщение отправлено: " << msg.mtext << std::endl;
        exit(0);
    }


    else {
        sleep(2);  
        Message msg;
        msgrcv(msgid, &msg, sizeof(msg.mtext), 1, 0);  
        std::cout << "Получено сообщение: " << msg.mtext << std::endl;
        
        msgctl(msgid, IPC_RMID, NULL);  
    }

    return 0;
}