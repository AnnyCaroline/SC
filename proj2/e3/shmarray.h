/* Shared Memory int Array */

#include <sys/shm.h>
#include <stdio.h>  //perror
#include <stdlib.h> //exit

int shmid;
int* array;
int size;

int shmarray_create(key_t key, int sizeF){
    size = sizeF;

	shmid = shmget(key, size*sizeof(int), IPC_CREAT | 0600);
	/*
	 from: http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/shm/shmget.html

	 If a client wants to use a shared memory created with IPC_PRIVATE, it must be a child process of the server, created after the parent has obtained the shared memory, so that the private key value can be passed to the child when it is created. For a client, changing IPC_CREAT | 0666 to 0666 works fine. A warning to novice C programmers: don't change 0666 to 666. The leading 0 of an integer indicates that the integer is an octal number. Thus, 0666 is 110110110 in binary. If the leading zero is removed, the integer becomes six hundred sixty six with a binary representation 1111011010.
	 
	 Server and clients can have a parent/client relationship or run as separate and unrelated processes. In the former case, if a shared memory is requested and attached prior to forking the child client process, then the server may want to use IPC_PRIVATE since the child receives an identical copy of the server's address space which includes the attached shared memory. However, if the server and clients are separate processes, using IPC_PRIVATE is unwise since the clients will not be able to request the same shared memory segment with a unique and unknown key. 
	*/

	// E2 - ERRO AO CRIAR MEMÓRIA COMPARTILHADA
	if (shmid < 0){
		perror("shmget");
		exit(1);
	}

	// E2 - ATTACH
	array = (int*) shmat(shmid, NULL, 0);

	// E2 - ERRO NO ATTACH
	if (array == (int *) -1){
		perror("shmat");
		//E2 - LIBERAR A MEMÓRIA COMPARTILHADA - IPC_RMID indica uma operação de remoção
		if (!shmctl(shmid, IPC_RMID, NULL)){
			perror("Erro ao liberar o segmento de memória compartilhada\n");
		}
		exit(1);
	}
}

void shmarray_init(){
	array[0]=0;
	array[1]=300;
}

void shmarray_delete(){
	// E2 - DETACH (DESACOPLAMENTO) NA MEMÓRIA COMPARTILHADA
	if (shmdt(array) < 0){
		perror("Erro no detach\n");
	};

	// E2 - LIBERAR A MEMÓRIA COMPARTILHADA - IPC_RMID indica uma operação de remoção
	if (shmctl(shmid, IPC_RMID, NULL) < 0){
		perror("Erro ao liberar o segmento de memória compartilhada\n");
	}
}

int shmarray_get(int i){
    return array[i];
}

void shmarray_set(int i, int num){
    array[i] = num;
}

void shmarray_sum(int i, int num){
    if (i>=0 && i<=size){
        array[i] = array[i] + num;
    }
}


