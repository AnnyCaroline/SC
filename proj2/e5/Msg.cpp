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
    msg.mtype=1;
    msg.pid = pid_f;
    int retorno = msgsnd(msqid, &msg, sizeof(int), 0);

    /*
    Esta função recebe três parâmetros em sua chamada:
    - o identificador da fila msqid;
    - um ponteiro msgp para a estrutura de tipo msgbuf que contem a mensagem a ser enviada;
    - um inteiro msg_sz indicando o tamanho em bytes da mensagem apontada por msgbuf e;
    - uma flag msgflg que controla o modo de envio da mensagem.
        - Se este parâmetro vale 0, ele provocará o bloqueio do processo chamando msgsnd quando a fila de mensagens estiver cheia;
        - Se ele tem o flag IPC_NOWAIT, a função retorna imediatamente sem enviar a mensagem e com um valor de erro igual a -1 indicando que a fila esta cheia. 
    */

    if(retorno == -1){
        perror("Envio de mensagem impossivel") ;
        exit(-1);
    }
}

int Msg::receive(){
    struct Msg::msgtext msg;
    msg.mtype = 1;
    int retorno = msgrcv(msqid, &msg, sizeof(int), 0, 0);

    /*Ela recebe cinco parâmetros:
    - o identificador da fila msqid;
    - um ponteiro msgp para a estrutura de tipo msgbuf;
    - tamanho máximo da mensgem a ser recebida; 
    - um inteiro longo msgtyp indicando o qual a mensagem a ser recebida e;
        - 0: a primeira mensagem da fila será lida, isto é, a mensagem na cabeça da lista será lida;
    - um flag msgflg controlando o modo de execução da recepção da mensagem.
        - Se ele não contiver o flag IPC_NOWAIT, o processo que chama a função msgrcv ficará bloqueado até que haja uma mensagem do tipo requerido
        - O flag MSG_EXCEPT é utlizado em conjunto com msgtyp > 0 para a leitura da primeira mensagem da fila cujo tipo de mensagem difere de msgtyp
    */

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
