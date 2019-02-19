#include <stdio.h>

int getPairSum(int *vector, int n){
	int sum=0;
	int i=0;
	int temp;
	for(i=0; i<n; i++){
		temp=(*(vector+i));
		if((temp%2)==0){
			sum += 	temp;
		}
	}
	return sum;
}

int main(){

	int data[] = {1,2,3,4,5};
	printf("%d\n", getPairSum(data, 5));


	return 0;
}