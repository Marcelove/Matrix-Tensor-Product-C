#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/time.h>

struct timeval start, end;

////gcc -pthread -o main c.c

void tensorproduct(int **matriz1, int **matriz2, int linhas1, int linhas2, 
        int cols1, int cols2)
{

        FILE *out = fopen("tensor_mlaj.out", "w");

        int linhaTensorial = linhas1 * linhas2;
        int colunaTensorial = cols1 * cols2;

        int tensorMatriz[linhaTensorial][colunaTensorial];

        int a, b, c, d;

        
        for (int a = 0; a < linhas1; a++) { 
  
        
                for (int c = 0; c < linhas2; c++) { 
  
            
                        for (int b = 0; b < cols1; b++) { 
  
                
                                for (int d = 0; d < cols2; d++) { 
  
                                        tensorMatriz[a + d + 1][b + c + 1] = matriz1[a][b] * matriz2[c][d]; 
                                        //printf("%d\t", tensorMatriz[a + d + 1][b + c + 1]); 
                                        fprintf (out, "%d\t", tensorMatriz[a + d + 1][b + c + 1]);
                                } 
                        } 
                        fprintf(out, "\n"); 
                } 
        } 

        gettimeofday(&end, NULL);

        double time_taken = end.tv_sec + end.tv_usec / 1e6 - start.tv_sec - start.tv_usec / 1e6; // in seconds

        fprintf(out, "\nTempo gasto: %f .", time_taken);

        fclose(out);
}



int **lermatriz(size_t *linhas, size_t *colunas, const char *nomearquivo)
{
        *linhas = 0;
        *colunas = 0;

        FILE *fp = fopen(nomearquivo, "r");

        //(2) se algum dos arquivos de entrada não existir ou não puder ser aberto,
        if(fp == NULL)
        {
                fprintf(stderr, "Erro ao abrir arquivo %s: %s\n", nomearquivo, strerror(errno));
                return NULL;
        }

        int **matriz = NULL, **tmp;

        char line[1024];

        while(fgets(line, sizeof line, fp))
        {
                if(*colunas == 0)
                {
                        // Determinar o numero de colunas através da primeira linha
                        char *scan = line;
                        int valor;
                        int offset = 0;
                        while(sscanf(scan, "%d%n", &valor, &offset) == 1)
                        {
                                scan += offset;
                                (*colunas)++;
                        }
                }

                tmp = realloc(matriz, (*linhas + 1) * sizeof *matriz);

                if(tmp == NULL)
                {
                        fclose(fp);
                        return matriz; 
                }

                matriz = tmp;

                matriz[*linhas] = calloc(*colunas, sizeof *matriz[*linhas]);

                if(matriz[*linhas] == NULL)
                {
                        
                        fclose(fp);
                        if(*linhas == 0) 
                        {
                                fprintf(stderr, "\nFalha na leitura do arquivo.\n");
                
                                fclose(fp);
                                free(matriz);
                                return NULL;
                        }

                        return matriz; 
                }

                int offset = 0;
                char *scan = line;
                for(size_t j = 0; j < *colunas; ++j)
                {
                        if(sscanf(scan, "%d%n", matriz[*linhas] + j, &offset) == 1)
                                scan += offset;
                        else
                                matriz[*linhas][j] = 0; 
                        }

                        
                        (*linhas)++;
                }

                fclose(fp);

                return matriz;
}



int main(int argc, char * argv[])
{

        gettimeofday(&start, NULL);

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

        size_t cols1, linhas1, cols2, linhas2;
        int **matriz1 = lermatriz(&linhas1, &cols1, argv[1]);
        int **matriz2 = lermatriz(&linhas2, &cols2, argv[2]);


        tensorproduct(matriz1, matriz2, linhas1, linhas2, cols1, cols2);
        


        // Liberando a matriz 1
        for(size_t i = 0; i < linhas1; ++i){
                free(matriz1[i]);
        }

        // Liberando a matriz 2
        for(size_t i = 0; i < linhas2; ++i){
                free(matriz2[i]);
        }


        free(matriz1);
        free(matriz2);

        return 0;
}
