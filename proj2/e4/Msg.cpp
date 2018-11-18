#include "Msg.h"

void Msg::create(key_t key){
    msqid = msgget(key, IPC_CREAT | 0600);

    if (msqid == -1){
        perror("Erro de msgget");
        exit(1);
    }
}

void Msg::send(int pid_f){
    struct Msg::msgtext msg;
    msg.pid = pid_f;
    int retorno = msgsnd(msqid, &msg, sizeof(int), 0);
    if(retorno == -1){
        perror("Envio de mensagem impossivel") ;
        exit(-1);
    }
}

int Msg::receive(){
    struct Msg::msgtext msg;
    int retorno = msgrcv(msqid, &msg, sizeof(int), 0, 0);

    if (retorno==-1){
        perror("msgrcv");
        exit(1);
    }else{
        return msg.pid;
    }
}

void Msg::del(){
	if (msgctl(msqid, IPC_RMID, NULL) < 0){
		perror("Erro ao liberar o fila de mensagens\n");
	}
}
