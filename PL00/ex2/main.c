#include <stdio.h>

void subtractValueFromArray(int *vector, int n, int num){
	if(n-1<0){
		return;
	}
	*(vector+n-1)=*(vector+n-1)-num;
	subtractValueFromArray(vector, n-1, num);
}

int main(){

	int data[] = {1,2,3,4,5};
	subtractValueFromArray(data, 5, 1);
	int i;
	for(i=0; i<5; i++){
		printf("%d\n", data[i]);
	}

	return 0;
}