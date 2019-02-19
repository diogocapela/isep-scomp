#include <stdio.h>

int getPairSum(int *vector, int n, int sum){
	if((n-1)<0){
		return sum;
	}	
	int temp = *(vector+(n-1));
	if(temp%2==0){
		return getPairSum(vector, n-1, sum+temp);
	}
	return getPairSum(vector, n-1, sum);
}

int main(){

	int data[] = {1,2,3,4,5};
	printf("%d\n", getPairSum(data, 5, 0));


	return 0;
}