#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <wait.h>
#include <stdio.h>

#define C_RED   "\x1B[31m"
#define C_YEL   "\x1B[33m"
#define C_CYN   "\x1B[36m"
#define C_RESET "\x1b[0m"

#define N_CHILD 3
#define N_LOOP  100

int child[N_CHILD];
const char* color[N_CHILD] = {C_RED, C_YEL, C_CYN};

int getChildIndex(int pid){
	for(int i=0; i < N_CHILD; i++) {
		if (child[i] == pid)
			return i;
	}

	return -1;
}

int main(){

	//Cria área de memória compartilhada
	int shmid;
	int* array;

	//ToDo Why 666?
	shmid = shmget(9876, 2*sizeof(int), IPC_CREAT | 0666);

	if (shmid < 0){
		perror("shmget");
		exit(1);
	}

	array = (int*) shmat(shmid, NULL, 0);

	if (array == (int *) -1){
		perror("shmat");
		//ToDo destruir shm
		exit(1);
	}

	array[0]=0;
	array[1]=300;

    for(int i=0; i < N_CHILD; i++) {

        child[i] = fork();

        if (child[i] < 0){
		    perror ("Erro ao criar um processo filho\n");
		    exit(1);
	    }
        else if (child[i] == 0){
			int* array = (int*) shmat(shmid, NULL, 0);
		    int pid = getpid();
		    child[i] = pid;	

			
			// Ao exibir as informações, acrescenta o valor das variáveis compartilhadas
		    printf ("%s PID: %d, Número: %d, Variável 1: %d, Variável 2: %d\n", color[getChildIndex(pid)], pid, getChildIndex(pid), array[0], array[1]);
	        printf(C_RESET);

			// Le a primeira variável compartilhada e
			// Copia para uma variável local
			int primeira = array[0];
			// Incrementa o valor da variável
			primeira++;
			// Dorme um tempo
		    sleep(1);
			// Copia de volta para a primeira variável compartilhada
			array[0] = primeira;
			// EU: coloquei pra dormir mais um pouco 
			sleep(1);
			// Decremente o valor da variável compartilhada
			printf("\n vai decrementar %d\n",array[0]);
			array[0]--;
			// Dorme mais um tempo
		    sleep(1);

            //TERMINA O FILHO
            exit(0);
        }
        else if (child[i] > 0){
	        printf ("Eu sou o pai, e criei um filho com pid = ");
	        printf("%s", color[i]);
	        printf("%d\n", child[i]);
	        printf(C_RESET);
        }
    }

    //BARREIRA
    for(int i=0; i < N_CHILD; i++) {
        int pid = wait(NULL);
        printf("Filho ");
        printf("%s", color[i]);
        printf("%d, pid %d", getChildIndex(pid), pid);
        printf(C_RESET);
        printf(" terminou.\n");
    }

	printf("Todos os filhos terminaram.\n");
	printf(" - var1: %d\n", array[0]);
	printf(" - var2: %d\n", array[1]);

	return 0;
}

