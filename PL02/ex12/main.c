#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

struct produto{
	float preco;
	char nome[50];
};

struct produto_request{
    char nome[50];
    int id;
};

int main(void)
{
    int i, fd[5][2];
    int fdUnique[2];
	struct produto buffer;
    struct produto_request buffer1;
    const int LEITURA = 0;
    const int ESCRITA = 1;
    const int NUMBER_OF_CHILDREN = 5;

    if(pipe(fdUnique) == -1){
			perror("pipe falhou");
			exit(EXIT_FAILURE);
	}

    struct produto_request produtos_r[5];
	produtos_r[0].id = 0;
    strcpy(produtos_r[0].nome,"batata");
    produtos_r[1].id = 1;
    strcpy(produtos_r[1].nome,"pera");
    produtos_r[2].id = 2;
    strcpy(produtos_r[2].nome,"laranja");
    produtos_r[3].id = 3;
    strcpy(produtos_r[3].nome,"banana");
    produtos_r[4].id = 4;
    strcpy(produtos_r[4].nome,"tangerina");
    
    for(i=0;i<5;i++){
		
		if(pipe(fd[i]) == -1){
			perror("pipe falhou");
			exit(EXIT_FAILURE);
		}	
		pid_t pid = fork();
		if(pid == -1){
			perror("fork falhou");
			exit(EXIT_FAILURE);
		}
		if(pid == 0){
			close(fd[i][ESCRITA]);
            write(fdUnique[ESCRITA],&produtos_r[i],sizeof(struct produto_request));
			if(read(fd[i][LEITURA],&buffer,sizeof(struct produto)) > 0){
				printf("Codigo de barras para produto %d : nome: %s preco: %.2f \n",i+1,buffer.nome, buffer.preco);
			}
			close(fd[i][LEITURA]);
			exit(EXIT_SUCCESS);
		}
		
    }  
    close(fdUnique[ESCRITA]);
    struct produto produtos[5];
    produtos[0].preco = 5.0;
    strcpy(produtos[0].nome,"batata");
    produtos[1].preco = 4.98;
    strcpy(produtos[1].nome,"pera");
    produtos[2].preco = 2.33;
    strcpy(produtos[2].nome,"laranja");
    produtos[3].preco = 6.23;
    strcpy(produtos[3].nome,"banana");
    produtos[4].preco = 5.34;
    strcpy(produtos[4].nome,"tangerina");
    
    for(i=0; i<NUMBER_OF_CHILDREN; i++){
        close(fd[i][LEITURA]);
        if(read(fdUnique[LEITURA],&buffer1,sizeof(struct produto_request)) > 0){
             write(fd[buffer1.id][ESCRITA],&produtos[i],sizeof(struct produto));
        }
        close(fd[i][ESCRITA]);
    }

    close(fdUnique[ESCRITA]);
    return 0;
}
