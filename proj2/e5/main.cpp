#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <wait.h>
#include <math.h>
#include <stdio.h>

#include <sys/resource.h>

#include "Timer.h"
#include "Shmarray.h"
#include "Msg.h"
#include "dijkstra.h"

#define C_RED   "\x1B[31m"
#define C_YEL   "\x1B[33m"
#define C_CYN   "\x1B[36m"
#define C_RESET "\x1b[0m"

#define N_CHILD 3
#define N_LOOP  100
#define N_MUTEX 2

int child[N_CHILD];
const char* color[N_CHILD] = {C_RED, C_YEL, C_CYN};

int getChildIndex(int pid){
	for(int i=0; i < N_CHILD; i++){
		if (child[i] == pid)
			return i;
	}
	return -1;
}

int main(){
	Timer t;
	t.click();

	Shmarray array;
	array.create(IPC_PRIVATE, 2); 	//e3 - criacao
	array.init(); //e3 - inicializacao

	#if N_MUTEX==1
		int mutex = sem_create(IPC_PRIVATE, 1);
	#else
		int mutex[2];
		mutex[0] = sem_create(IPC_PRIVATE, 1);
		mutex[1] = sem_create(IPC_PRIVATE, 1);
	#endif

	Msg msg;
	msg.create(IPC_PRIVATE);

	int prioridadeDoPai = getpriority(PRIO_PROCESS, getpid());

	printf("\n-------------------------------------\n");
	printf("Prioridade do processo pai: %d \n", prioridadeDoPai);
	printf("-------------------------------------\n\n");

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

			// E5 - muda a prioridade do processo
			if (getChildIndex(pid) != 0)
				setpriority(PRIO_PROCESS, pid, 19);

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

				#if N_MUTEX==1
					P(mutex);
				#else
					P(mutex[0]);
				#endif

				// E2 - rotina
				int primeira = array.get(0);
				primeira++;
				usleep(sleepTime*1000);
				array.set(0, primeira);

				#if N_MUTEX==1
				#else
					int var1 = array.get(0);
					V(mutex[0]);
				#endif					

				usleep(sleepTime*1000); //eu
				
				#if N_MUTEX==1
				#else
					P(mutex[1]);
				#endif	

				array.sum(1, -1);
				
				#if N_MUTEX==1
				#else
					int var2 = array.get(1);
					V(mutex[1]);
				#endif		

				usleep(sleepTime*1000);
				// E2 - fim rotina	

				tchild.clock();

				// E1 - ADD NA MEDIA
			    media += tchild.interval_s()/N_LOOP;
			    intervals[j] = tchild.interval_s();

				// E2 - Ao exibir as informações, acrescenta o valor das variáveis compartilhadas
				#if N_MUTEX==1
					printf ("%s PID: %d, Número: %d, Paço: %3d, sleep time: %3dms, tempo de execucao: %.5fs var1: %3d, var2: %3d\n", color[getChildIndex(pid)], pid, getChildIndex(pid), j, sleepTime, tchild.interval_s(), array.get(0), array.get(1), getpriority(PRIO_PROCESS, pid));	
				#else
					printf ("%s PID: %d, Número: %d, Paço: %3d, sleep time: %3dms, tempo de execucao: %.5fs var1: %3d, var2: %3d, prioridade: %2d\n", color[getChildIndex(pid)], pid, getChildIndex(pid), j, sleepTime, tchild.interval_s(), var1, var2, getpriority(PRIO_PROCESS, pid)  );	
				#endif	
				printf(C_RESET);

				#if N_MUTEX==1
					V(mutex);	
				#endif
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

			// E4 - ENVIA MENSAGEM
			msg.send(pid);

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
        int pid = msg.receive();
		printf(C_RESET);
		printf("\n-------------------------------------\n");
        printf("Filho ");
        printf("%s", color[getChildIndex(pid)]);
        printf("%d, pid %d", getChildIndex(pid), pid);
        printf(C_RESET);
        printf(" terminou.\n");
		printf("-------------------------------------\n\n");
    }

	// E2 - COPIA PARA VARIÁVEL LOCAL
	int var1 = array.get(0);
	int var2 = array.get(1);

	// E2 - LIBERAR MEMÓRIA COMPARTILHADA
	array.del();

	// E3 - LIBERAR SEMÁFORO
	#if N_MUTEX==1
		sem_delete(mutex);
	#else
		sem_delete(mutex[0]);
		sem_delete(mutex[1]);
	#endif	

	//E4 - LIBERAR FILA DE MENSAGENS
	msg.del();

	t.clock();
	
	printf("Todos os  filhos terminaram. Processamento teve a duração de %.5f s\n", t.interval_s());
	printf(" - var1: %d\n", var1);
	printf(" - var2: %d\n", var2);

	return 0;
}
