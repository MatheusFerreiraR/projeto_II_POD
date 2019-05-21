#include "mergeExterno.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

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
                fprintf(arq,"%d \n",i+1);
            }
            break;
        case 2:
            for(i=0, k=0; i<tam; i++, k++, j--){
                fprintf(arq,"%d \n",j);

            }
            break;
        case 3:
            for(i=0, k=0; i<tam; i++, k++, j--){
                fprintf(arq,"%d \n",random_numero(1, tam));

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

int shellSort(int *vet, int tam) {
    int i=0 , j , value;
    int x;

    int gap = 1;
    while(gap < tam) {
        gap = 3*gap+1;
    }
    while ( gap > 1) {
        gap /= 3;
        for(i = gap; i < tam; i++) {
            value = vet[i];
            j = i;
            while (j >= gap && value < vet[j - gap]) {
                vet[j] = vet [j - gap];
                j = j - gap;
            }
            vet [j] = value;
        }
    }
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
    int i, k;
    FILE *arq = fopen(nome, "w"); //alterar para ab
    for(i = 0, k=0; i < n; i++)
        fprintf(arq, "%d ", vet[i]);

    fclose(arq);

    return 0;
}

int arquivoFinal(char *nome, int *vet, int n){
    int i, j;
    FILE *arq = fopen(nome, "a");
    for(i=0, j=1; i<n; i++, j++){
        fprintf(arq, "%d \n", vet[i]);
        if(j == 30){
            fprintf(arq, "\n");
            j=0;
        }
    }
    fclose(arq);
    return 0;
}

struct arquivo{
    FILE *f;
    int pos, MAX, *buffer;
};

int preencher(struct arquivo* arq, int K){
    int i;
    if(arq->f == NULL){
        return 0;
    }

    arq->MAX = 0;
    arq->pos = 0;

    for(i=0; i<K; i++){
        if(!feof(arq->f)){
            fscanf(arq->f, "%d ", &arq->buffer[arq->MAX]);
            arq->MAX++;
        }else{
            fclose(arq->f);
            arq->f = NULL;
            break;
        }
    }

}

int procura(struct arquivo* arq, int numArqs, int K, int *menor){
    int i = 0, j = -1;

    for(i=0; i<numArqs; i++){
        if(arq[i].pos < arq[i].MAX){
            if(j == -1){
                j = i;
            }else{
                if(arq[i].buffer[arq[i].pos] < arq[j].buffer[arq[j].pos]){
                    j = i;
                }
            }
        }
    }
    if(j != -1){
        *menor = arq[j].buffer[arq[j].pos];
        arq[j].pos++;
        if(arq[j].pos == arq[j].MAX){
            preencher(&arq[j], K);
        }
        return 1;
    }else{
        return 0;
    }
}

int intercalaExterno(int numArqs, int n, int K){
    int i, vet[K], j, p, menor, cont = 0;
    char nomeArq[30], *nomeArqFinal = {"arquivo_final/arqFinal.txt"};

    struct arquivo* arq;
    arq = (struct arquivo*)malloc(numArqs * sizeof(struct arquivo));

    for(i=0; i<numArqs; i++){
        sprintf(nomeArq,"arquivos_temp/ArqTemp%d.txt",i+1);
        arq[i].f = fopen(nomeArq, "r"); // Passo o arquivo para leitura
        arq[i].MAX = 0;
        arq[i].pos = 0;
        arq[i].buffer = (int*)malloc(K*sizeof(int)); // Aloco a memoria do tamanho do buffer
        preencher(&arq[i], K); // Passando cada arquivo temporario para um vetor dentro da struct
    }

    //printf("%d", arq[0].pos);
    //printf("%s", nomeArqFinal);

    while(procura(arq, numArqs, K, &menor) == 1){ //
        vet[cont] = menor;
        cont++;
        if(cont == K){ //Se o vetor encher, insere no arquivo final
            arquivoFinal(nomeArqFinal, &vet, K);
            cont=0;
        }
    }

    if(cont!=0){ //Se sobrar algo no vetor, insere o restante no arquivo final
        arquivoFinal(nomeArqFinal, &vet, cont);
    }

    for(i=0; i<numArqs; i++){
        free(arq[i].buffer); //Desaloca todos os vetores dos arquivos da struct.
    }

    free(arq); //Encerra a struct.
    free(nomeArqFinal); //Encerra o arquivo final.
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
            sprintf(novoArq,"arquivos_temp/ArqTemp%d.txt", numArqs);
            shellSort(&vet, n);
            criarArqTemp(novoArq, &vet, i);
            i = 0;
        }
    }
    if(i > 0){
        numArqs++;
        sprintf(novoArq,"arquivos_temp/ArqTemp%d.txt", numArqs);
        shellSort(&vet, n);
        criarArqTemp(novoArq, &vet, i);
    }
    fclose(arq);
    return numArqs;
}


int mergeExterno(char *nome, int n, int tam){ //n = limite da memmoria - tam = tamanho vetor
    char novoArq[30];
    int numArqs, i, k;

    numArqs = criarArqsOrdenados(nome, n);
    k = n / (numArqs+1);
    if(k == 0)
        k = 1;
    intercalaExterno(numArqs,n,k);

    return 0;
}

int inicializa(){
    int n, tam, op, i;
    char *nome = {"arq_principal.txt"};

    printf("- Digite o limite da memoria: ");
    scanf("%d", &n);

    printf("\n- Digite o tamanho do vetor: ");
    scanf("%d", &tam);

    printf("\n- Digite a operacao desejada para preenchimento do vetor:\n");
    printf("1- Crescente \n2- Descrescente \n3- Random\n");
    scanf("%d", &op);


    criarArqMain(nome, tam, op);

    if(n < tam){
        char novoArq[30];
        printf("\nArquivo criar. clique qualquer tecla para ordenar.\n");
        system("pause");
        mergeExterno(nome, n, tam);
        printf("\nTecle qualquer tecla para excluir os arquivos temporarios.\n");
        system("pause");

        for(i=0; i<tam/n; i++){
            sprintf(novoArq,"arquivos_temp/ArqTemp%d.txt", i+1);
            remove(novoArq);
        }
    }else{
        int vet[tam];
        FILE *arq = fopen(nome, "r");

        for(i = 0; i<tam; i++)
            fscanf(arq, "%d ", &vet[i]);

        mergeSortInterno(&vet, 0, tam-1);
        for(i=0; i<tam; i++)
            printf("%d ", vet[i]);
    }
    return 0;
}
