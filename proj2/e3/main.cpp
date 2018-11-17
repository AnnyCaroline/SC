#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>

#include <wait.h>
#include <math.h>
#include <stdio.h>

#include "Timer.h"
#include "dijkstra.h"
#include "shmarray.h"

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

	int mutex[2]; //e3

	// E2 - INICIALIZAÇÃO DA MEMÓRIA COMPARTILHADA
	shmarray_create(IPC_PRIVATE, 2);
	shmarray_init();

	// E3 - INICIALIZAÇÃO DO SEMÁFORO
	mutex[0] = sem_create(IPC_PRIVATE, 1);
	mutex[1] = sem_create(IPC_PRIVATE, 1);

    for(int i=0; i < N_CHILD; i++) {

        child[i] = fork();

        if (child[i] < 0){
		    perror ("Erro ao criar um processo filho\n");
		    exit(1);
	    }
        else if (child[i] == 0){
            Timer tchild;
		    int pid = getpid();

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

				// E3 - Semaforo
				P(mutex[0]);				

				// E2 - rotina
					int primeira = shmarray_get(0);
					primeira++;
					usleep(sleepTime*1000);
					shmarray_set(0, primeira);

					// E3 - Semaforo
					V(mutex[0]);
 
					usleep(sleepTime*1000); //eu
					
					// E3 - Semaforo
					P(mutex[1]);
					shmarray_sum(1,-1);
					V(mutex[1]);

					usleep(sleepTime*1000);
				// E2 - fim rotina		

				tchild.clock();

				// E1 - ADD NA MEDIA
			    media += tchild.interval_s()/N_LOOP;
			    intervals[j] = tchild.interval_s();

				// E2 - Ao exibir as informações, acrescenta o valor das variáveis compartilhadas
				printf ("%s PID: %d, Número: %d, Paço: %3d, sleep time: %3dms, tempo de execucao: %.5fs var1: %3d, var2: %3d\n", color[getChildIndex(pid)], pid, getChildIndex(pid), j, sleepTime, tchild.interval_s(), shmarray_get(0), shmarray_get(1));	
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
        printf("%s", color[i]);
        printf("%d, pid %d", getChildIndex(pid), pid);
        printf(C_RESET);
        printf(" terminou.\n");
		printf("-------------------------------------\n\n");
    }

	// E2 - COPIA PARA VARIÁVEL LOCAL
	int var1 = shmarray_get(0);
	int var2 = shmarray_get(1);

	// E2 - LIBRAR SHM
	shmarray_delete();

	// E3 - LIBERAR SEMÁFORO
	sem_delete(mutex[0]);
	sem_delete(mutex[1]);

	t.clock();
	
	printf("Todos os  filhos terminaram. Processamento teve a duração de %.5f s\n", t.interval_s());
	printf(" - var1: %d\n", var1);
	printf(" - var2: %d\n", var2);

	return 0;
}
