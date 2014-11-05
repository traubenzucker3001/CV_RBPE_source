#include <iostream>
//#include "PhysicEngine/EarlyOut.h"

using namespace std;

#define SIZE 1024

int main() {

	int *a,*b,*c;

	a = (int *)malloc(SIZE*sizeof(int));
	b = (int *)malloc(SIZE*sizeof(int));
	c = (int *)malloc(SIZE*sizeof(int));

	for(int i=0; i<SIZE; ++i){

		a[i] = i;
		b[i] = i;
		c[i] = 0;
	}

	//funktion wieder entfernt
	//VecAddCPU(a,b,c,SIZE);

	for(int i=0; i<10; ++i){

		printf("c[%d] = %d\n",i,c[i]);
	}

	free(a);
	free(b);
	free(c);

	return 0;
}
