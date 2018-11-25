/* Implementacao dos semaforos de Dijkstra usando IPC
*
* sem_create(): criacao de um semaforo de Dijkstra
* P()         : realizacao da operacao P sobre o semaforo
* V()         : realizacao da operacao V sobre o semaforo
* sem_delete() : destruicao do semaforo
*/ 

#include <sys/sem.h>
#include <stdio.h>  //perror
#include <stdlib.h> //exit

/* criacao de um semaforoa associado a */
/* chave key, com valor initial initval */
int sem_create(key_t key, int initval){
    int semid;

    union semun {
        int val;
        struct semid_ds *buf;
        ushort array[1];
    } arg_ctl ;
    
    semid = semget(key, 1, IPC_CREAT|IPC_EXCL|0666);
    if (semid == -1) {
        semid = semget(key,1,0666); 
        if (semid == -1) {
            perror("Erro semget()");
            exit(1);
        }
    }

    arg_ctl.val = initval;
    if (semctl(semid,0,SETVAL,arg_ctl) == -1) {
        perror("Erro inicializacao semaforo");
        exit(1);
    }
    return(semid);
}

void P(int semid){
    struct sembuf sempar[1];
    sempar[0].sem_num = 0;
    sempar[0].sem_op = -1;
    sempar[0].sem_flg = SEM_UNDO;

    if (semop(semid, sempar, 1) == -1)
        perror("Erro operacao P");
}


void V(int semid){
    struct sembuf sempar[1];
    sempar[0].sem_num = 0;
    sempar[0].sem_op =  1;
    sempar[0].sem_flg = SEM_UNDO;

    if (semop(semid, sempar, 1) == -1)
        perror("Erro operacao V") ;
}


void sem_delete(int semid){
    if (semctl(semid,0,IPC_RMID,0) == -1)
        perror("Erro na destruicao do semaforo");
}