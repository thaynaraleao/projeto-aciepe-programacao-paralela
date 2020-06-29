#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

#define DIM 2048000     // ajuste a dimensão aqui...

// Array estático

double dados [DIM];

int PESOS_CPF_PRIMEIRO_DIGITO[9] = {10, 9, 8, 7, 6, 5, 4, 3, 2};
int PESOS_CPF_SEGUNDO_DIGITO[10] = {11, 10, 9, 8, 7, 6, 5, 4, 3, 2};

int
main(int argc, char *argv[])
{
    int i, j, k, soma_primeiro_digito, cpf, soma_segundo_digito, primeiro_digito, segundo_digito;
    
    /* Atribuição de valores iniciais */
    srand(time(NULL));
    
    for(i=0; i < DIM; i++){
        dados[i]= (int)rand() / (int)(RAND_MAX/ 999999991);
    }
    
    #pragma omp parallel num_threads(4) private(soma_primeiro_digito, soma_segundo_digito, primeiro_digito, segundo_digito, cpf, j, k)
   {
        #pragma omp for
            for (i=0; i < DIM; i++){
                soma_primeiro_digito = 0;
                soma_segundo_digito = 0;
                cpf = dados[i];
                j = 8;
                
                // Cálculo do primeiro dígito verificador
                
                while (cpf > 0){
                    soma_primeiro_digito = soma_primeiro_digito + ((cpf % 10) * PESOS_CPF_PRIMEIRO_DIGITO[j]);
                    cpf = cpf/10;
                    j = j - 1;
                }
                cpf = dados[i];
                if (soma_primeiro_digito % 11 < 2)
                    {primeiro_digito = 0;}
                else
                    {primeiro_digito = 11 - (soma_primeiro_digito % 11);}
                dados[i] = (dados[i]*10) + primeiro_digito;
               
                
                // Cálculo do segundo dígito verificador
                
                k = 8;
                while (cpf > 0){
                    soma_segundo_digito = soma_segundo_digito + ((cpf % 10) * PESOS_CPF_SEGUNDO_DIGITO[k]);
                    cpf = cpf/10.0;
                    k = k - 1;
                }
                
               soma_segundo_digito = soma_segundo_digito + (primeiro_digito * PESOS_CPF_SEGUNDO_DIGITO[9]);
               if (soma_primeiro_digito % 11 < 2)
                    {segundo_digito = 0;}
               else
                    {segundo_digito = 11 - (soma_segundo_digito % 11);}
                    
               dados[i] = (dados[i]*10) + segundo_digito;
            }  
    }    

    return 0; 
}
