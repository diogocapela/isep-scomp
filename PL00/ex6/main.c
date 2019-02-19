#include <stdio.h>
#include <string.h>

void concatenate(char * str1, char * str2){
	int len = strlen(str1);
	int len2 = strlen(str2);

	int i;
	for(i=0; i<len; i++){
		if(i>=len2-1){
			break;
		}
		*(str2+i)=*(str1+i);
	}
}

int main(){
	char * str1 = "é gay";
	char * str2 = "o capela ";
	concatenate(str1, str2);
	printf("%s", str2);

	return 0;
}