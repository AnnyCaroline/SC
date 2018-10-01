#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_CHILD 3

int child[N_CHILD];

int getChildIndex(int pid){
	int i;
	for(i=0; i < N_CHILD; i++) {
		if (child[i] == pid)
			return i;
	}

	return -1;
}

int main(){

	int i;
	for(i=0; i < N_CHILD; i++) {

	  child[i] = fork();

	  if (child[i] < 0){
		perror ("Erro ao criar um processo filho\n");
		exit(1);
	  }
	  else if (child[i] == 0){
		printf ("My pid is (%d)\n", getpid ());
		sleep(5);
		exit(0);
	  }
	  else if (child[i] > 0){
		printf ("Eu sou o pai, e criei um filho com pid = %d\n", child[i]);
	  }

	}

	for(i=0; i < N_CHILD; i++) {
		int pid = wait(NULL);
        printf("Filho %d, pid %d, terminou.\n", getChildIndex(pid), pid);
	}
	printf("Todos os  filhos terminaram. Processamento teve a duração de Z ms\n");


	return 0;
}

