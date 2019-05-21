#include "mergeExterno.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

int random_numero(int min_num, int max_num){ // Funcao para preencher o arquivo com numeros randomicos com minimo e maximo.
    int result = 0;

    result = (rand() % (max_num - min_num)) + min_num;

    return result;
}

int criarArqMain(char *nome, int tam, int op){ // Funcao para criar o primeiro arquivo não ordenado.
    int i, j=tam, k;

    FILE *arq = fopen(nome, "w");

    switch(op){ // Case para escolher o modo que será preenchido o arquivo.
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
            printf("\n\nOperacao invalidade, tente novamente.\n\n"); // Caso o usuario digite uma opcao
                                                                    //invalida, o sistema volta ao inicio.
            system("pause");
            system("cls");
            fclose(arq);
            inicializa();
        }
    fclose(arq);
    return 0;
}

int shellSort(int *vet, int tam){ //Funcao usada para ordenar internamente os arquivos temporarios.
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

int mergeSortInterno(int *vet, int esq, int dir){ // Está funcao de merge interno sera usada somente
                                                 //quando o tamanha da memoria for maior que o tamanho do vetor
    if(esq < dir){
        int meio = floor((dir+esq) / 2);
        mergeSortInterno(vet,esq,meio);
        mergeSortInterno(vet,meio+1,dir);
        intercalaInterno(vet,esq,meio,dir);
    }
    return 0;
}

int intercalaInterno(int *v, int inicio, int meio, int fim){ // Continuacao do metodo de merge interno citado acima.
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

int criarArqTemp(char *nome, int *vet, int n){ // Esta funcao criara os arquivos temporarios que terao partes do arquivo original.
    int i, k;
    FILE *arq = fopen(nome, "w"); // Abre arquivo com "w",se existir algum arquivo com o mesmo nome o mesmo sera substituido.
    for(i = 0, k=0; i < n; i++)
        fprintf(arq, "%d ", vet[i]);

    fclose(arq);

    return 0;
}

int arquivoFinal(char *nome, int *vet, int n){ // Funcao que cria o arquivo final.
                                            // A diferenca entre ela e a "criarArqTemp" é o modo de abrir o arquivo.
    int i;
    FILE *arq = fopen(nome, "a"); // Abre arquivo em "a" para continuar de onde parou.
    for(i=0; i<n; i++){
        fprintf(arq, "%d \n", vet[i]);
    }
    fclose(arq);
    return 0;
}


int preencher(struct arquivo *arq, int K){ // Esta funcao preenchera os buffers da struct arquivo.
    int i;

    if(arq->f == NULL){ // Se o arquivo ja estiver fechado, nao precisa fazer mais nada.
        return 0;
    }

    //MAX e pos voltam para o zero, para zerar o buffer.
    arq->MAX = 0;
    arq->pos = 0;

    for(i=0; i<K; i++){ // Um laco de repeticao de i ate k (tamanho do meu buffer).
        if(!feof(arq->f)){ // Se meu arq->f nao estiver no final.
            fscanf(arq->f, "%d ", &arq->buffer[arq->MAX]); // Armazeno o dado no meu vetor da struct (buffer) na posicao MAX;
            arq->MAX++; // Incremento MAX
        }else{
            fclose(arq->f); //Caso o arquivo estiver no final, o mesmo sera fechado.
            arq->f = NULL; // E f recebe NULL.
            break;
        }
    }


}

int procura(struct arquivo* arq, int numArqs, int K, int *menor){ // A funcao procura, ira verificar todos
                                                                 // os arquivos e procurar o menor de sua posicao

    int i = 0, j = -1; // J é uma flag para saber se foi finalizado o processo

    for(i=0; i<numArqs; i++){ // Laco de repeticao de i ate numArqs, pois devera ser verificado todos os arquivos
        if(arq[i].pos < arq[i].MAX){ // A primeira verificacao e se pos é menor que MAX
            if(j == -1){
                j = i;
            }else{
                if(arq[i].buffer[arq[i].pos] < arq[j].buffer[arq[j].pos]){ // Acho o menor valor de todos os arquivos
                    j = i;
                }
            }
        }
    }
    if(j != -1){
        *menor = arq[j].buffer[arq[j].pos]; // Menor recebe o valor menor de todos os arquivos
                                            // e retorna esse valor para a funcao intercalaExterno
        arq[j].pos++; // Incremento pos
        if(arq[j].pos == arq[j].MAX){ // Se pos for igual a MAX, quer dizer que o buffer ja foi lido por inteiro
            preencher(&arq[j], K); // Entao ele sera preenchido novamente
        }
        return 1; // etorno 1 para continuar o while na funcao intercalaExterno
    }else{
        return 0; // Ou retorno 0 para finalizar o laco, pois todos os valores de todos os arquivos ja foram lidos.
    }
}

int intercalaExterno(int numArqs, int n, int K){ // Esta funca de intercalaExterno chamara outras funcoes
                                                // para fazer a intercalacao
    int i, vet[K], j, p, menor, cont = 0;
    char nomeArq[30], *nomeArqFinal = {"arquivo_final/arqFinal.txt"};

    struct arquivo* arq;
    arq = (struct arquivo*)malloc(numArqs * sizeof(struct arquivo)); // alocar a strut arquivo "numArqs" vezes.

    for(i=0; i<numArqs; i++){ // Este laco de repeticao passa os parametros necessarios para cada variavel
        sprintf(nomeArq,"arquivos_temp/ArqTemp%d.txt",i+1);
        arq[i].f = fopen(nomeArq, "r"); // o arquivo para leitura
        arq[i].MAX = 0;
        arq[i].pos = 0;
        arq[i].buffer = (int*)malloc(K*sizeof(int)); // alocar a memoria do tamanho do buffer
        preencher(&arq[i], K); // Preenchendo o buffer com arquivo temporario
    }

    while(procura(arq, numArqs, K, &menor) == 1){ // Este laco recebera o menor valor
                                                 // dos arquivos atraves da funcao "procura" e atribuira para a variavel "vet".
        vet[cont] = menor;
        cont++;
        if(cont == K){ //Se o vetor chegar na sua ultima posicao, insere no arquivo final
                      // e retorna o indice para 0 para continuar o sistema ate o final.
            arquivoFinal(nomeArqFinal, &vet, K); // Atribui os dados do vet ja cheio para o arquivo final.
            cont=0;
        }
    }

    if(cont!=0){ // Caso seu vetor nao seja multiplo do seu limite de memoria sobrara dados no "vet"
                // entao, insere o restante no arquivo final
        arquivoFinal(nomeArqFinal, &vet, cont);
    }

    for(i=0; i<numArqs; i++){
        free(arq[i].buffer); //Desaloca todos os vetores dos arquivos da struct.
    }

    free(arq); //Encerra a struct.
    free(nomeArqFinal); //Encerra o arquivo final.
    return 0;
}


int criarArqsOrdenados(char *nome, int n){ // Esta funcao criara os arquivos temporarios ja ordenados
    int vet[n], numArqs=0, i=0;
    char novoArq[30];
    FILE *arq = fopen(nome, "r");

    while(fscanf(arq, "%d", &vet[i]) != EOF){ // Enquanto nao chegar ao final do arquivo o
                                             // "vet" recebera os dados do arquivo principal.
        i++;
        if(i == n){ // i == n indica que seu "vet" esta na sua ultima posicao.
            numArqs++; // Incrementa o numero de arquivos.
            sprintf(novoArq,"arquivos_temp/ArqTemp%d.txt", numArqs); // Nome dos arquivos temporarios.
            shellSort(&vet, n); // Ordena "vet" antes para criar o arquivo ja ordenado.
            criarArqTemp(novoArq, &vet, i); // Criar arquivo ordenado.
            i = 0;
        }
    }
    if(i > 0){ // Caso o limite de memoria nao seja multiplo do tamanho do vetor, sobrara dados do arquivo temporario no "vet"
              // Entao, faca o mesmo processo acima
        numArqs++;
        sprintf(novoArq,"arquivos_temp/ArqTemp%d.txt", numArqs);
        shellSort(&vet, n);
        criarArqTemp(novoArq, &vet, i);
    }

    fclose(arq); // Fechar arquivo.
    return numArqs;
}


int mergeExterno(char *nome, int n, int tam){  // Funcao que ira chamar as outras funcoes para a continuidade do sistema.
    int numArqs, i, k;

    numArqs = criarArqsOrdenados(nome, n); // A variavel numArqs recebera a quantidade de arquivo temporario criado.
    k = n / (numArqs+1); // k sera o tamanho do buffer

    if(k == 0) // se meu k for igual a 0, ele recebera o valor 1, pois o buffer nao pode ser de tamanho 0.
        k = 1;

    intercalaExterno(numArqs,n,k); // Chamar a funcao "intercalaExterno".

    return 0;
}

int inicializa(){ // Funcao que inicializa o sistema.
    int n, tam, op, i;
    char *nome = {"arq_principal.txt"}; // Nome do arquivo principal

    printf("- Digite o limite da memoria: "); // Limita o espaco de memoria interna.
    scanf("%d", &n);

    printf("\n- Digite o tamanho do arquivo: "); // Definir o tamanho do arquivo.
    scanf("%d", &tam);

    printf("\n- Digite a operacao desejada para preenchimento do vetor:\n"); // Definir modo de preenchimento do arquivo principal.
    printf("1- Crescente \n2- Descrescente \n3- Random\n");
    scanf("%d", &op);


    criarArqMain(nome, tam, op);

    if(n < tam){ // Se o tamanho da memoria for menor que o tamanho do arquivo, fazer o merge externo
        char novoArq[30];

        printf("\nArquivo criar. clique qualquer tecla para ordenar.\n");
        system("pause");

        mergeExterno(nome, n, tam);

        printf("\nTecle qualquer tecla para excluir os arquivos temporarios.\n");
        system("pause");

        for(i=0; i<tam/n; i++){ // Deletar arquivos temporarios
            sprintf(novoArq,"arquivos_temp/ArqTemp%d.txt", i+1);
            remove(novoArq);
        }
    }else{ // Senao, fazer o merge interno.
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
