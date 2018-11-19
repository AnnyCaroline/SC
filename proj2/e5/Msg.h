#ifndef MSG_H
#define MSG_H

#include <sys/types.h>
#include <sys/msg.h>
#include <stdio.h>  //perror
#include <stdlib.h> //exit
#include <string.h> //strlen


class Msg {
    private:
        int msqid;
        struct msgtext {
            long mtype = 1;
            int pid;
        };                 

	public:
        void create(key_t key);
        void send(int pid_f);
        int  receive();
        void del();
};

#endif
