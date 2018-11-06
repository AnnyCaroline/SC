#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <math.h>
#include <stdio.h>

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

			    // AGUARDA UM TEMPO ALEATÓRIO ( entre  5  e  100  ms)
			    sleepTime = random()%96 + 5; //[0~95]+5
			    usleep(sleepTime*1000);

			    tchild.clock();

			    //ADD NA MEDIA
			    media += tchild.interval_s()/N_LOOP;
			    intervals[j] = tchild.interval_s();

			    printf ("%s PID: %d, Número: %d, Paço: %d, sleep time: %dms, tempo de execucao: %.5fs C_RESET\n", color[getChildIndex(pid)], pid, getChildIndex(pid), j, sleepTime,tchild.interval_s());
		    }

            //CALCULA O DESVIO PADRÃO
            for (int j=0; j<N_LOOP; j++)
	            desvioPadrao += pow(intervals[j] - media,2);
            desvioPadrao = desvioPadrao/N_LOOP;
            desvioPadrao = sqrt(desvioPadrao);

            //EXIBE O TEMPO MÉDIO E O DESVIO PADRÃO
            printf("%s", color[getChildIndex(pid)]);
            printf("PID: %d, Tempo gasto médio: %.5f ms\n", pid, media);
            printf("PID: %d, Desvio padrao: %.5f\n", pid, desvioPadrao);	
            printf(C_RESET);	

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

	t.clock();
	printf("Todos os  filhos terminaram. Processamento teve a duração de %.5f s\n", t.interval_s());

	return 0;
}
