#include "mergeExterno.h"
#include <stdio.h>
#include <time.h>

int random_numero(int min_num, int max_num){
    int result = 0, low_num = 0, hi_num = 0;

    if (min_num < max_num){
        low_num = min_num;
        hi_num = max_num + 1;
    }else{
        low_num = max_num + 1;
        hi_num = min_num;
    }

    result = (rand() % (hi_num - low_num)) + low_num;

    return result;
}

int criarArqMain(char *nome, int tam, int op){
    int i, j=tam, k;

    FILE *arq = fopen(nome, "w");

    switch(op){
        case 1:
            for(i=0,k=1; i<tam; i++, k++){
                fprintf(arq,"%d ",i+1);
                if(k==100){
                    fprintf(arq,"\n");
                    k=0;
                }
            }
            break;
        case 2:
            for(i=0, k=0; i<tam; i++, k++, j--){
                fprintf(arq,"%d ",j);
                if(k==100){
                    fprintf(arq,"\n");
                    k=0;
                }
            }
            break;
        case 3:
            for(i=0, k=0; i<tam; i++, k++, j--){
                fprintf(arq,"%d ",random_numero(1, tam));
                if(k==100){
                    fprintf(arq,"\n");
                    k=0;
                }
            }
            break;
        default:
            printf("\n\nOperacao invalidade, tente novamente.\n\n");
            system("pause");
            system("cls");
            fclose(arq);
            inicializa();
        }
    fclose(arq);
    return 0;
}

int mergeSortInterno(int *vet, int esq, int dir){
    if(esq < dir){
        int meio = floor((dir+esq) / 2);
        mergeSortInterno(vet,esq,meio);
        mergeSortInterno(vet,meio+1,dir);
        intercalaInterno(vet,esq,meio,dir);
    }
    return 0;
}

int intercalaInterno(int *v, int inicio, int meio, int fim){
    int p1 = inicio, p2 = meio+1, i = 0, tam = fim-inicio+1, k, j;
    int fim1 = 0, fim2 = 0;
    int vetAux[tam];

    for(i=0; i < tam; i++){
        if(!fim1 && !fim2){
            if(v[p1] < v[p2])
                vetAux[i] = v[p1++];
            else
                vetAux[i] = v[p2++];

            if(p1>meio) fim1 = 1;
            if(p2>fim) fim2 = 1;
        }else{
            if(!fim1)
                vetAux[i] = v[p1++];
            else
                vetAux[i] = v[p2++];
        }
    }
    for(i = 0, k = inicio; i < tam; i++, k++)
        v[k] = vetAux[i];

    return 0;
}

int criarArqTemp(char *nome, int *vet, int n){
    int i;
    FILE *arq = fopen(nome, "a");
    for(i = 0; i < n; i++)
        fprintf(arq, "%d ", vet[i]);

    fclose(arq);

    return 0;
}

int criarArqsOrdenados(char *nome, int n){
    int vet[n], numArqs=0, i=0;
    char novoArq[30];
    FILE *arq = fopen(nome, "r");

    while(fscanf(arq, "%d", &vet[i]) != EOF){
        i++;
        if(i == n){
            numArqs++;
            sprintf(novoArq,"arquivos_temp/Temp%d.txt", numArqs);
            mergeSortInterno(&vet, 0, n-1);
//            printf("\nordenado:\n");
//            for(i=0; i<n; i++){
//                printf("%d ", vet[i]);
//            }
//            printf("\n");
            criarArqTemp(novoArq, &vet, n);
            i = 0;
        }
    }
    if(i > 0){
        numArqs++;
        sprintf(novoArq,"arquivos_temp/Temp%d.txt", numArqs);
        mergeSortInterno(&vet, 0, n-1);
        criarArqTemp(novoArq, &vet, n);
    }
    fclose(arq);
    return numArqs;
}


int mergeExterno(char *nome, int n, int tam){ //n = limite da memmoria - tam = tamanho vetor
    char novoArq[30];
    int numArqs, i, k;

    numArqs = criarArqsOrdenados(nome, n);

    return 0;
}

int inicializa(){
    int n, tam, op;
    char *nome[] = {"arq_principal.txt"};

    printf("- Digite o limite da memoria: ");
    scanf("%d", &n);

    printf("\n- Digite o tamanho do vetor: ");
    scanf("%d", &tam);

    printf("\n- Digite a operacao desejada para preenchimento do vetor:\n");
    printf("1- Crescente \n2- Descrescente \n3- Random\n");
    scanf("%d", &op);

    criarArqMain(nome, tam, op);
    mergeExterno(nome, n, tam);
    return 0;
}
