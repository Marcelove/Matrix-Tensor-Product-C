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

void tensorproduct (void * sim)
{

        struct matrizes * smatrizes = (struct matrizes*)sim;

        FILE *out = fopen("tensor_mlaj.out", "w");

        int linhaTensorial = smatrizes->linhas1 * smatrizes->linhas2;
        int colunaTensorial = smatrizes->cols1 * smatrizes->cols2;

        int tensorMatriz[linhaTensorial][colunaTensorial];

        int a, b, c, d;

         
        for (int a = 0; a < smatrizes->linhas1; a++) { 
  
  
                for (int c = 0; c < smatrizes->linhas2; c++) { 
  
            
                        for (int b = 0; b < smatrizes->cols1; b++) { 
  
                
                                for (int d = 0; d < smatrizes->cols2; d++) { 
  

                                        tensorMatriz[a + d + 1][b + c + 1] = smatrizes->matriz1[a][b] * smatrizes->matriz2[c][d]; 
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

void * lermatriz1(void * sim)
{
        struct matrizes * matrizX = (struct matrizes*)sim;

        matrizX->linhas1 = 0;
        matrizX->cols1 = 0;

        //(2) se algum dos arquivos de entrada não existir ou não puder ser aberto,
        if(fp == NULL)
        {
                printf("\nErro ao abrir o arquivo 1.\n");
                return NULL;
        }

        int **matriz = NULL, **tmp;

        char line[1024];

        while(fgets(line, sizeof line, fp))
        {
                if(matrizX->cols1 == 0)
                {
                        // Determinar o numero de colunas através da primeira linha
                        char *scan = line;
                        int valor;
                        int offset = 0;
                        while(sscanf(scan, "%d%n", &valor, &offset) == 1)
                        {
                                scan += offset;
                                (matrizX->cols1)++;
                        }
                }

                tmp = realloc(matriz, (matrizX->linhas1 + 1) * sizeof *matriz);

                if(tmp == NULL)
                {
                        fclose(fp);
                        matrizX->matriz1 = matriz;
                }

                matriz = tmp;

                matriz[matrizX->linhas1] = calloc(matrizX->cols1, sizeof *matriz[matrizX->linhas1]);

                if(matriz[matrizX->linhas1] == NULL)
                {
                        
                        fclose(fp);
                        if(matrizX->linhas1 == 0) 
                        {
                                fprintf(stderr, "\nFalha na leitura do arquivo.\n");
                
                                fclose(fp);
                                free(matriz);
                                return NULL;
                        }

                        matrizX->matriz1 = matriz;
                }

                int offset = 0;
                char *scan = line;
                for(size_t j = 0; j < matrizX->cols1; ++j)
                {
                        if(sscanf(scan, "%d%n", matriz[matrizX->linhas1] + j, &offset) == 1)
                                scan += offset;
                        else
                                matriz[matrizX->linhas1][j] = 0; 
                }

                        (matrizX->linhas1)++;
        }

        fclose(fp);

        matrizX->matriz1 = matriz;
        
}


void * lermatriz2(void * sim)
{

        struct matrizes * matrizX = (struct matrizes*)sim;

        matrizX->linhas2 = 0;
        matrizX->cols2 = 0;

        //(2) se algum dos arquivos de entrada não existir ou não puder ser aberto,
        if(fp2 == NULL)
        {
                printf("\nErro ao abrir o arquivo 2.\n");
                return NULL;
        }

        int **matriz = NULL, **tmp;

        char line[1024];

        while(fgets(line, sizeof line, fp2))
        {
                if(matrizX->cols2 == 0)
                {
                        // Determinar o numero de colunas através da primeira linha
                        char *scan = line;
                        int valor;
                        int offset = 0;
                        while(sscanf(scan, "%d%n", &valor, &offset) == 1)
                        {
                                scan += offset;
                                (matrizX->cols2)++;
                        }
                }

                tmp = realloc(matriz, (matrizX->linhas2 + 1) * sizeof *matriz);

                if(tmp == NULL)
                {
                        fclose(fp2);
                        return matriz; 
                }

                matriz = tmp;

                matriz[matrizX->linhas2] = calloc(matrizX->cols2, sizeof *matriz[matrizX->linhas2]);

                if(matriz[matrizX->linhas2] == NULL)
                {
                        
                        fclose(fp2);
                        if(matrizX->linhas2 == 0) 
                        {
                                fprintf(stderr, "\nFalha na leitura do arquivo.\n");
                
                                fclose(fp2);
                                free(matriz);
                                return NULL;
                        }

                        matrizX->matriz2 = matriz;
                }

                int offset = 0;
                char *scan = line;
                for(size_t j = 0; j < matrizX->cols2; ++j)
                {
                        if(sscanf(scan, "%d%n", matriz[matrizX->linhas2] + j, &offset) == 1)
                                scan += offset;
                        else
                                matriz[matrizX->linhas2][j] = 0; 
                }

                        
                        (matrizX->linhas2)++;
        }

        fclose(fp2);

        matrizX->matriz2 = matriz;

}