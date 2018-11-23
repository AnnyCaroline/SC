#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <wait.h>
#include <math.h>
#include <stdio.h>

#include "time.h"
#include "Timer.h"

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

	Timer t;
	t.click();

	// E2 - CRIA ÁREA DE MEMÓRIA COMPARTILHADA
	int shmid;
	int* array;

	// E2 - SHMGET
	shmid = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT | 0600);
	/*
	 from: http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/shm/shmget.html

	 If a client wants to use a shared memory created with IPC_PRIVATE, it must be a child process of the server, created after the parent has obtained the shared memory, so that the private key value can be passed to the child when it is created. For a client, changing IPC_CREAT | 0666 to 0666 works fine. A warning to novice C programmers: don't change 0666 to 666. The leading 0 of an integer indicates that the integer is an octal number. Thus, 0666 is 110110110 in binary. If the leading zero is removed, the integer becomes six hundred sixty six with a binary representation 1111011010.
	 
	 Server and clients can have a parent/client relationship or run as separate and unrelated processes. In the former case, if a shared memory is requested and attached prior to forking the child client process, then the server may want to use IPC_PRIVATE since the child receives an identical copy of the server's address space which includes the attached shared memory. However, if the server and clients are separate processes, using IPC_PRIVATE is unwise since the clients will not be able to request the same shared memory segment with a unique and unknown key. 
	*/

	//E2 - ERRO AO CRIAR MEMÓRIA COMPARTILHADA
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

	// E2 - INICIALIZAÇÃO DAS VARIÁVEIS COMPARTILHADAS
	array[0]=0;
	array[1]=300;

	if (array == (int *) -1){
		perror("shmat");
		//ToDo destruir shm
		exit(1);
	}	

    for(int i=0; i < N_CHILD; i++) {

        child[i] = fork();

        if (child[i] < 0){
		    perror ("Erro ao criar um processo filho\n");
		    exit(1);
	    }
        else if (child[i] == 0){
            Timer tchild;
		    int pid = getpid();
		    child[i] = pid;

		    srand(pid * (time(NULL)/100));

		    int sleepTime;
		    double intervals[N_LOOP];

		    double media = 0;
		    double desvioPadrao = 0;

		    printf("\n");
		    for (int j=0; j<N_LOOP; j++){
			    tchild.click();

			    // TEMPO ALEATÓRIO ( entre  5  e  100  ms)
			    sleepTime = random()%96 + 5; //[0~95]+5

				/* *************************** */
				/* PROCEDIMENTO DO EXERCÍCIO 2 */
				/* *************************** */

				// E2 - Le a primeira variável compartilhada e
				// E2 - Copia para uma variável local
				int primeira = array[0];
				// E2 - Incrementa o valor da variável
				primeira++;
				// E2 - Dorme um tempo
				usleep(sleepTime*1000);
				// E2 - Copia de volta para a primeira variável compartilhada
				array[0] = primeira;
				// EU: coloquei pra dormir mais um pouco 
				usleep(sleepTime*1000);
				// E2 - Decremente o valor da variável compartilhada
				array[1]--;
				// E2 - Dorme mais um tempo
				usleep(sleepTime*1000);

				/* ********************************** */
				/* FIM DO PROCEDIMENTO DO EXERCÍCIO 2 */
				/* ********************************** */				
				tchild.clock();

				// E1 - ADD NA MEDIA
			    media += tchild.interval_s()/N_LOOP;
			    intervals[j] = tchild.interval_s();

				// E2 - Ao exibir as informações, acrescenta o valor das variáveis compartilhadas
				printf ("%s PID: %d, Número: %d, Paço: %3d, sleep time: %3dms, tempo de execucao: %.5fs var1: %3d, var2: %3d\n", color[getChildIndex(pid)], pid, getChildIndex(pid), j, sleepTime, tchild.interval_s(), array[0], array[1]);	
				printf(C_RESET);		
		    }

            // E1 - CALCULA O DESVIO PADRÃO
            for (int j=0; j<N_LOOP; j++)
	            desvioPadrao += pow(intervals[j] - media,2);
            desvioPadrao = desvioPadrao/N_LOOP;
            desvioPadrao = sqrt(desvioPadrao);

            // E1 - EXIBE O TEMPO MÉDIO E O DESVIO PADRÃO
            printf("%s", color[getChildIndex(pid)]);
            printf(" >> PID: %d, Tempo gasto médio: %.5f ms\n", pid, media);
            printf(" >> PID: %d, Desvio padrao: %.5f\n", pid, desvioPadrao);	
            printf(C_RESET);	

            // E1 - TERMINA O FILHO
            exit(0);
        }
        else if (child[i] > 0){
	        printf ("Eu sou o pai, e criei um filho com pid = ");
	        printf("%s", color[i]);
	        printf("%d\n", child[i]);
	        printf(C_RESET);
        }
    }

    // E1 - BARREIRA
    for(int i=0; i < N_CHILD; i++) {
        int pid = wait(NULL);
		printf("\n-------------------------------------\n");
        printf("Filho ");
        printf("%s", color[getChildIndex(pid)]);
        printf("%d, pid %d", getChildIndex(pid), pid);
        printf(C_RESET);
        printf(" terminou.\n");
		printf("-------------------------------------\n\n");
    }

	// E2 - COPIA PARA VARIÁVEL LOCAL
	int var1 = array[0];
	int var2 = array[1];

	// E2 - DETACH (DESACOPLAMENTO) NA MEMÓRIA COMPARTILHADA
	if (shmdt(array) == -1){
		perror("Erro no detach\n");
	};

	// E2 - LIBERAR A MEMÓRIA COMPARTILHADA - IPC_RMID indica uma operação de remoção
	if (shmctl(shmid, IPC_RMID, NULL) == -1){
		perror("Erro ao liberar o segmento de memória compartilhada\n");
	}

	t.clock();
	
	printf("Todos os  filhos terminaram. Processamento teve a duração de %.5f s\n", t.interval_s());
	printf(" - var1: %d\n", var1);
	printf(" - var2: %d\n", var2);

	return 0;
}
