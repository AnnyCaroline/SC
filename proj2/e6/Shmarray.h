#ifndef SHMARRAY_H
#define SHMARRAY_H

#include <sys/shm.h>
#include <stdio.h>  //perror
#include <stdlib.h> //exit

class Shmarray {
    private:
        int shmid;
		int* array;
		int size;

	public:
        int  create(key_t key, int sizeF);
        void init(int nexec);
        int  get(int i);
        void set(int i, int num);
        void sum(int i, int num);
        void del();
};

#endif
