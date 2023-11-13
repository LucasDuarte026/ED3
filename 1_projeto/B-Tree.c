#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "structsBTree.h"
#include "B-Tree.h"

BTreeNode *createRoot(BTreeNode *raiz)
{
    BTreeNode *newNode = (BTreeNode *)malloc(sizeof(BTreeNode));
    newNode->C1 = NULL;
    newNode->C2 = NULL;
    newNode->C3 = NULL;

    newNode->P1 = NULL;
    newNode->P2 = NULL;
    newNode->P3 = NULL;
    newNode->P4 = NULL;

    newNode->PR1 = -1;
    newNode->PR2 = -1;
    newNode->PR3 = -1;

    return newNode;
}

int heightTree(BTreeNode *raiz)
{

    int height = 1;
    if (raiz == NULL)
    {
        return 1;
    }
    else
    {
        if (raiz->P1 != NULL)
        {
            height += heightTree(raiz->P1);
        }
        if (raiz->P2 != NULL)
        {
            height += heightTree(raiz->P2);
        }
        if (raiz->P3 != NULL)
        {
            height += heightTree(raiz->P3);
        }
        if (raiz->P4 != NULL)
        {
            height += heightTree(raiz->P4);
        }
    }
    return height;
}
void insertInRoot(BTreeNode *root, char *aux)
{
    if (root->C1 == NULL)
    {
        root->C1 = strdup(aux);
    }
    else if (root->C2 == NULL)
    {
        root->C2 = strdup(aux);
    }
    else if (root->C3 == NULL)
    {
        root->C3 = strdup(aux);
    }
}

void insertIndex(BTreeNode *root, Dados *dados) // Testa e insere o dado dentro do arquivo de index
{
    int stringConcatMaxSize = strlen(dados->nomeTecnologiaDestino.string) + strlen(dados->nomeTecnologiaOrigem.string) + 1; //  Para se concatenar, achas-se o tamanho total da string concatenada
    char aux[stringConcatMaxSize];                                                                                          //  Cria um auxiliar para guardar tal string concatenada

    strcpy(aux, dados->nomeTecnologiaOrigem.string);  // Copia origem na aux
    strcat(aux, dados->nomeTecnologiaDestino.string); // Concatena com destino

    // printf("concatenado: %s\n", aux);
    printf("valor é %d\n", heightTree(root));
    if (heightTree(root) == 1)
    {
        insertInRoot(root, aux);
        //  é nó raiz, inserir já nele
    }
    /*
    else
    {
        switch (isAvailable(raiz))
        {
        case 1:

            break;
        case 2:
            break;
        case 3:
            break;
        case 0:
            // insertIndex(raiz,&dados)/
            break;
        default:
            break;
        }
    }
    */
}