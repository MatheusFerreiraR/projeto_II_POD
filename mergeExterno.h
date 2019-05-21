#ifndef MERGEEXTERNO_H_INCLUDED
#define MERGEEXTERNO_H_INCLUDED
#include<stdio.h>
struct arquivo{ //Struct que usaremos para ordenar os dados dos arquivos temporarios.
    FILE *f; // f abrira o arquivo.
    int pos, MAX, *buffer; // pos indicara a posisao, MAX indicara se a pos chegou ao maximo
                          // e buffer sera um vetor com os dados para ordenacao.
};

int random_numero(int min_num, int max_num);
int criarArqMain(char *nome, int tam, int op);
int shellSort(int *vet, int tam);
int mergeSortInterno(int *vet, int esq, int dir);
int intercalaInterno(int *v, int inicio, int meio, int fim);
int criarArqTemp(char *nome, int *vet, int n);
int arquivoFinal(char *nome, int *vet, int n);
int preencher(struct arquivo *arq, int K);
int procura(struct arquivo* arq, int numArqs, int K, int *menor);
int intercalaExterno(int numArqs, int n, int K);
int criarArqsOrdenados(char *nome, int n);
int mergeExterno(char *nome, int n, int tam);
int inicializa();

#endif // MERGEEXTERNO_H_INCLUDED
