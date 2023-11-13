#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "structsBTree.h"
#include "B-Tree.h"

BTreeNode *createRoot(BTreeNode *raiz)
{
    BTreeNode *newNode = (BTreeNode *)malloc(sizeof(BTreeNode));
    printf("teste");
    newNode->P1 = NULL;
    newNode->P2 = NULL;
    newNode->P3 = NULL;
    newNode->P4 = NULL;
    newNode->PR1 = -1;
    newNode->PR2 = -1;
    newNode->PR3 = -1;

    return newNode;
}


void insertNonFull(BTreeNode *node, char *key)
{
}
void insert(BTreeNode *node, Dados *dado)
{
    int stringConcatMaxSize = strlen(dado->nomeTecnologiaDestino.string) + strlen(dado->nomeTecnologiaOrigem.string) + 1; //  Para se concatenar, achas-se o tamanho total da string concatenada
    char aux[stringConcatMaxSize];
    strcpy(aux, dado->nomeTecnologiaOrigem.string);  // Copia origem na aux
    strcat(aux, dado->nomeTecnologiaDestino.string); // Concatena com destino
    printf("\n\n%s",aux);
    // if ()
    // {
    //     insertNonFull(node, );
    // }
    // else
    // {
    //     // insertFull
    // }
}