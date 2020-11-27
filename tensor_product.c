#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#include "tensor.h"

struct matrizes {

    size_t cols1, linhas1, cols2, linhas2;
    int **matriz1;
    int **matriz2;
};

FILE *fp;
FILE *fp2;

struct timeval start, end;

int main(int argc, char * argv[])
{
        gettimeofday(&start, NULL);

        struct matrizes * smatrizes = malloc(sizeof(struct matrizes)); 

        /*
        O programa não deve falhar se encontrar um erro, precisando checar todos os parâmetros antes
        de aceita-los. Assim, seu programa deve apresentar um mensagem de erro coerente e encerrar
        a execução quando:
        */
        //(1) um número incorreto de argumento para os comando tensor for passado,
        if((argv[3] != NULL) || (argv[2] == NULL) || (argv[1] == NULL)){


                fprintf(stderr, "Erro: número incorreto de argumentos passados.\n");
                return 0;
        }

        fp = fopen(argv[1], "r");
        fp2 = fopen(argv[1], "r");


        pthread_t novaThread1, novaThread2;

        pthread_create(&novaThread1, NULL, lermatriz1, (void*) smatrizes);
        pthread_create(&novaThread2, NULL, lermatriz2, (void*) smatrizes);


        //tensorproduct(matriz1, matriz2, linhas1, linhas2, cols1, cols2);
        tensorproduct( (void*) smatrizes);


        // Liberando a matriz 1
        for(size_t i = 0; i < smatrizes->linhas1; ++i){
                free(smatrizes->matriz1[i]);
        }

        // Liberando a matriz 2
        for(size_t i = 0; i < smatrizes->linhas2; ++i){
                free(smatrizes->matriz2[i]);
        }


        free(smatrizes->matriz1);
        free(smatrizes->matriz2);
        free(smatrizes);

        return 0;
}
