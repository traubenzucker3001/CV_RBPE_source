//test test test
#include "EarlyOut.h"

void VecAddCPU(int *a, int *b int *c, int n){

	int i;
	for(i=0; i<n; ++i){
	
		c[i] = a[i] + b[i];
	}
}

void VecAddGPU(int *a, int *b int *c, int n){

	int i;
	for(i=0; i<n; ++i){
	
		c[i] = a[i] + b[i];
	}
}