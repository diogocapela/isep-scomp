#include <stdio.h>

void getInverse(int *vector, int n, float *vector2){
	int i;
	int temp;
	for(i=0; i<n; i++){
		temp = *(vector+i);
		*(vector2+i) = 1.0/(float)temp;	
	}
}

int main(){

	int data[] = {1,2,3,4,5};
	float dataInverse[5];
	getInverse(data, 5, dataInverse);
	int i;
	for(i=0; i<5; i++){
		printf("%f\n", dataInverse[i]);
	}

	return 0;
}