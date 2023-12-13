#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structsBTree.h"
#include "B-Tree.h"

/*  Inicializa a raiz   */

void cleanKeyVector(char vector[])
{
    for (int i = 0; i < 55; i++)
    {
        vector[i] = '$';
    }
}

BTreeNode *initNode()
{
    // Os ponteiros são inicializados apontando para -1,
            
    BTreeNode *newNode = (BTreeNode *)malloc(sizeof(BTreeNode));
    newNode->alturaNo = -1;
    newNode->nroChavesNo = -1;
    newNode->RRNdoNo = -1;

    cleanKeyVector(newNode->C1); // são iniciados com '$'
    cleanKeyVector(newNode->C2);
    cleanKeyVector(newNode->C3);

    newNode->P1 = -1;
    newNode->P2 = -1;
    newNode->P3 = -1;
    newNode->P4 = -1;

    newNode->PR1 = -1;
    newNode->PR2 = -1;
    newNode->PR3 = -1;

    return newNode;
}

int heightTree(FILE *bin_index, BTreeNode *node)
{
    int height = 0;
    BTreeNode *sonNode = initNode();

    if (node->P1 != -1)
    {
        sonNode = readIndexRegister(bin_index, sonNode, node->P1);
        int childHeight = heightTree(bin_index, sonNode);
        if (childHeight > height)
        {
            height = childHeight;
        }
    }
    if (node->P2 != -1)
    {
        sonNode = readIndexRegister(bin_index, sonNode, node->P2);

        int childHeight = heightTree(bin_index, sonNode);
        if (childHeight > height)
        {
            height = childHeight;
        }
    }
    if (node->P3 != -1)
    {
        sonNode = readIndexRegister(bin_index, sonNode, node->P2);
        int childHeight = heightTree(bin_index, sonNode);
        if (childHeight > height)
        {
            height = childHeight;
        }
    }
    if (node->P4 != -1)
    {
        sonNode = readIndexRegister(bin_index, sonNode, node->P4);
        int childHeight = heightTree(bin_index, sonNode);
        if (childHeight > height)
        {
            height = childHeight;
        }
    }
    return height + 1;
}

void treePrint(FILE *bin_index, int RRN)
{
    BTreeNode *node = initNode();
    node = readIndexRegister(bin_index, node, RRN);
    int height = heightTree(bin_index, node);

    if (node->P1 != -1)
    {
        treePrint(bin_index, node->P1);
    }
    if (node->P2 != -1)
    {
        treePrint(bin_index, node->P2);
    }
    if (node->P3 != -1)
    {
        treePrint(bin_index, node->P3);
    }
    if (node->P4 != -1)
    {
        treePrint(bin_index, node->P4);
    }
    printf("\t--> Height: %d\t| %d", height, node->RRNdoNo);
    for (int i = 0; i < 3 - height; i++)
    {
        printf("\t");
    }
    char *chave1, *chave2, *chave3;

    if (node->C1[0] != '$')
        chave1 = node->C1;
    else
        chave1 = " ";
    if (node->C2[0] != '$')
        chave2 = node->C2;
    else
        chave2 = " ";
    if (node->C3[0] != '$')
        chave3 = node->C3;
    else
        chave3 = " ";

    printf("||%2d||%15s ->(%2d) ||%2d||%15s ->(%2d) ||%2d||%15s ->(%2d) ||%2d|\n", node->P1, chave1, node->PR1, node->P2, chave2, node->PR2, node->P3, chave3, node->PR3, node->P4);
}

static int isAvailable(BTreeNode *node)
{
    if (node->C3[0] != '$')
    {
        return 0; // Texto  existe, portanto NÃO está disponível
    }
    return 1; // Texto não existe, é vazio, portanto ESTÁ disponível
}

/* Insere o valor de fato no local pedido */
static void insertInPlace(BTreeNode *node, char *aux, int referenceRRN, int place)
{
    switch (place)
    {
    case 1:
        strcpy(node->C1, aux);
        node->PR1 = referenceRRN;
        break;
    case 2:
        strcpy(node->C2, aux);
        node->PR2 = referenceRRN;
        break;
    case 3:
        strcpy(node->C3, aux);
        node->PR3 = referenceRRN;
        break;
    default:
        break;
    }
}

/*  Testa se vem antes, depois ou são igual
 * -> antes 1
 * -> depois 2
 * -> iguais 0
 */
static int stringHigherThen(char *a, char *b, int place)
{
    // A CORRIGIR CASO:
    // "DANIEL" E "DANIELO"
    if (a == NULL)
    {
        return 1;
        printf("Aux é nulo\n");
    }
    if (b == NULL)
    {
        return 1;
    }
    else if (a[place] == '\0')
    {
        if (b[place] == '\0')
            return 0;
        else
            return 1;
    }
    else if (b[place] == '\0')
    {
        return 2;
    }
    if (a[place] < b[place])
    {
        return 1; // caso venha antes
    }
    else if (a[place] > b[place])
    {
        return 2; // caso venha depois
    }
    else if (a[place] == b[place])
    {
        if (place > 55)
            return 0; // Caso sejam iguais
        else
            return stringHigherThen(a, b, place + 1); // caso a string tenha o elemento char igual
    }
}

static void shiftRightImplement(BTreeNode *node, char *aux, int referenceRRN, int place)
{
    switch (place)
    {
    case 1:
        // shiftar os aux
        strcpy(node->C3, node->C2);
        strcpy(node->C2, node->C1);
        strcpy(node->C1, aux);

        // shift dos PRi
        node->PR3 = node->PR2;
        node->PR2 = node->PR1;
        node->PR1 = referenceRRN;

        break;
    case 2:
        // shiftar os aux
        strcpy(node->C3, node->C2);
        strcpy(node->C2, aux);

        // shift dos PRi
        node->PR3 = node->PR2;
        node->PR2 = referenceRRN;

        break;
    case 3:
        // shiftar os aux
        node->PR3 = referenceRRN;

        // shift dos PRi
        strcpy(node->C3, aux);
        break;
    case 4:
        printf("\n\n\ttestar se faz sentido ter estrado aqui");
        strcpy(node->C1, aux);
        break;
    default:
        break;
    }
}

static void shiftPointers(BTreeNode *dadNode, BTreeNode *childNode, BTreeNode *newRight, int place)
{
    switch (place)
    {
    case 1:

        dadNode->P4 = dadNode->P3;
        dadNode->P3 = dadNode->P2;
        dadNode->P2 = newRight->RRNdoNo;
        break;
    case 2:
        dadNode->P4 = dadNode->P3;
        dadNode->P3 = newRight->RRNdoNo;
        break;
    case 3:
        dadNode->P4 = newRight->RRNdoNo;
        break;
    case 4:
        // dadNode->P4;
        // newRight->P1 = childNode->P3;
        // newRight->P2 = ;
        // newRight->P3 = ;
        // childNode->P3 = NULL;
        // childNode->P4 = NULL;
        break;
    default:
        break;
    }
}
int whereToInsert(BTreeNode *node, char *aux)
{
    if (strcmp(aux, node->C1) < 0 || node->C1[0] == '$')
        return 1;
    else if ((strcmp(aux, node->C1) > 0 && strcmp(aux, node->C2) < 0) || node->C2[0] == '$')
        return 2;
    else if ((strcmp(aux, node->C2) > 0 && strcmp(aux, node->C3) < 0) || node->C3[0] == '$')
        return 3;
    else if (strcmp(aux, node->C3) > 0)
        return 4;
    else
        return -1;
}

char **promoteVector(BTreeNode *node, char **vector, char *aux)
{
    int where = whereToInsert(node, aux);
    switch (where)
    {
    case 1:
        vector[0] = strdup(aux);
        vector[1] = strdup(node->C1);
        vector[2] = strdup(node->C2);
        vector[3] = strdup(node->C3);

        break;
    case 2:
        vector[0] = strdup(node->C1);
        vector[1] = strdup(aux);
        vector[2] = strdup(node->C2);
        vector[3] = strdup(node->C3);

        break;
    case 3:
        vector[0] = strdup(node->C1);
        vector[1] = strdup(node->C2);
        vector[2] = strdup(aux);
        vector[3] = strdup(node->C3);

        break;
    case 4:
        vector[0] = strdup(node->C1);
        vector[1] = strdup(node->C2);
        vector[2] = strdup(node->C3);
        vector[3] = strdup(aux);

        break;

    default:
        printf("erro 75\n");
        break;
    }
    return vector;
}

int *priVector(BTreeNode *childNode, int *priPromoted, char *aux, int referenceRRN)
{
    int where = whereToInsert(childNode, aux);
    switch (where)
    {
    case 1:
        priPromoted[0] = referenceRRN;
        priPromoted[1] = childNode->PR1;
        priPromoted[2] = childNode->PR2;
        priPromoted[3] = childNode->PR3;

        break;
    case 2:

        priPromoted[0] = childNode->PR1;
        priPromoted[1] = referenceRRN;
        priPromoted[2] = childNode->PR2;
        priPromoted[3] = childNode->PR3;

        break;
    case 3:
        priPromoted[0] = childNode->PR1;
        priPromoted[1] = childNode->PR2;
        priPromoted[2] = referenceRRN;
        priPromoted[3] = childNode->PR3;

        break;
    case 4:
        priPromoted[0] = childNode->PR1;
        priPromoted[1] = childNode->PR2;
        priPromoted[2] = childNode->PR3;
        priPromoted[3] = referenceRRN;
        break;

    default:
        printf("erro 75\n");
        break;
    }
    return priPromoted;
}

/*
BTreeNode **promotePointers(BTreeNode *node, BTreeNode **pointers)
{
    pointers[0] = node->P1;
    pointers[1] = node->P2;
    pointers[2] = node->P3;
    pointers[3] = node->P4;
    return pointers;
}
*/
static BTreeNode *splitNode(BTreeNode *childNode, BTreeNode *newRight, char *aux, int referenceRRN)
{
    char **vector = (char **)malloc(4 * sizeof(char *));
    int *priPromoted = (int *)malloc(4 * sizeof(int));
    vector = promoteVector(childNode, vector, aux);
    priPromoted = priVector(childNode, priPromoted, aux, referenceRRN);

    strcpy(childNode->C1, vector[0]);
    strcpy(childNode->C2, vector[1]);
    childNode->PR1 = priPromoted[0];
    childNode->PR2 = priPromoted[1];

    strcpy(newRight->C1, vector[3]);
    newRight->PR1 = priPromoted[3];

    cleanKeyVector(childNode->C3);
    childNode->PR3 = -1;
    return newRight;
}

// Testa se é raiz por meio da variavel local de "altura" e testa com a quantidade de filhos de um nó genérico
int isRoot(FILE *bin_index, BTreeNode *root, int *highestTree)
{
    int local_height = heightTree(bin_index, root);
    if (local_height == 1 && local_height < *highestTree)
    {
        return 0; //  É nó folha
    }
    else if (*highestTree == local_height)
    {
        return 1; //  É nó Raiz
    }

    else
    {
        return 0; //  É nó intermediario
    }
}

// Conta a quantidade de chaves armazenadas no nó
int keysQuant(BTreeNode *node)
{

    if (node->C3[0] != '$')
    {
        return 3;
    }
    else if (node->C2[0] != '$')
    {
        return 2;
    }
    else if (node->C1[0] != '$')
    {
        return 1;
    }
    else
    {
        printf("nó apenas inicializado\n");
        return 0;
    }
}

BTreeNode *readIndexRegister(FILE *bin_index, BTreeNode *prt_root, int RRN)
{

    fseek(bin_index, (1 + RRN) * RECORD_SIZE, SEEK_SET);
    fread(&prt_root->nroChavesNo, sizeof(int), 1, bin_index);
    fread(&prt_root->alturaNo, sizeof(int), 1, bin_index);
    fread(&prt_root->RRNdoNo, sizeof(int), 1, bin_index);

    fread(&prt_root->P1, sizeof(int), 1, bin_index);

    fread(&prt_root->C1, sizeof(char), 55, bin_index);
    if (prt_root->C1[0] != '$')
    {
        for (int i = 0; i < 55; i++)
        {
            if (prt_root->C1[i] == '$')
            {
                prt_root->C1[i] = '\0';
                break;
            }
        }
    }
    else
        cleanKeyVector(prt_root->C1);

    fread(&prt_root->PR1, sizeof(int), 1, bin_index);
    fread(&prt_root->P2, sizeof(int), 1, bin_index);

    fread(&prt_root->C2, sizeof(char), 55, bin_index);
    if (prt_root->C2[0] != '$')
    {
        for (int i = 0; i < 55; i++)
        {
            if (prt_root->C2[i] == '$')
            {
                prt_root->C2[i] = '\0';
                break;
            }
        }
    }
    else
        cleanKeyVector(prt_root->C2);
    fread(&prt_root->PR2, sizeof(int), 1, bin_index);
    fread(&prt_root->P3, sizeof(int), 1, bin_index);

    fread(&prt_root->C3, sizeof(char), 55, bin_index);
    if (prt_root->C3[0] != '$')
    {
        for (int i = 0; i < 55; i++)
        {
            if (prt_root->C3[i] == '$')
            {
                prt_root->C3[i] = '\0';
                break;
            }
        }
    }
    else
        cleanKeyVector(prt_root->C3);

    fread(&prt_root->PR3, sizeof(int), 1, bin_index);
    fread(&prt_root->P4, sizeof(int), 1, bin_index);
    return prt_root;
}

BTreeNode *getRoot(FILE *bin_index, BTreeNode *root)
{

    fseek(bin_index, 0, SEEK_SET); // Vai pro começo do arquivo
    char status;
    fread(&status, sizeof(char), 1, bin_index);
    int rootNodeValue;
    int analysis = fread(&rootNodeValue, sizeof(int), 1, bin_index); // lê a raiz
    printf("agora a é %d\n", analysis);
    root = readIndexRegister(bin_index, root, rootNodeValue);
    return root;
}
// Insere o nó no arquivo na posição em que estiver o arquivo de indice
// PLACE == -1  para armazenar no final

void updateBinArchive(FILE *bin_index, BTreeNode *node, int placeRRN)
{

    node->nroChavesNo = keysQuant(node);
    node->alturaNo = heightTree(bin_index, node);
    char aux = '$';
    if (placeRRN != -1)
        fseek(bin_index, (placeRRN + 1) * RECORD_SIZE, SEEK_SET);
    else
        fseek(bin_index, 0, SEEK_END);

    fwrite(&node->nroChavesNo, sizeof(int), 1, bin_index);
    fwrite(&node->alturaNo, sizeof(int), 1, bin_index);
    fwrite(&node->RRNdoNo, sizeof(int), 1, bin_index);

    fwrite(&node->P1, sizeof(int), 1, bin_index);
    int teste = 1;
    for (int i = 0; i < 55; i++)
    {
        if (!teste)
        {
            teste = 0;
            fwrite(&aux, sizeof(char), 1, bin_index);
        }
        if (teste)
        {
            if (node->C1[i] == '\0')
            {
                teste = 0;
                fwrite(&aux, sizeof(char), 1, bin_index);
            }
        }
        if (teste)
        {
            fwrite(&node->C1[i], sizeof(char), 1, bin_index);
        }
    }
    fwrite(&node->PR1, sizeof(int), 1, bin_index);
    fwrite(&node->P2, sizeof(int), 1, bin_index);

    teste = 1;
    for (int i = 0; i < 55; i++)
    {
        if (!teste)
        {
            fwrite(&aux, sizeof(char), 1, bin_index);
        }
        if (teste)
        {
            if (node->C2[i] == '\0')
            {
                teste = 0;
                fwrite(&aux, sizeof(char), 1, bin_index);
            }
        }
        if (teste)
        {
            fwrite(&node->C2[i], sizeof(char), 1, bin_index);
        }
    }
    fwrite(&node->PR2, sizeof(int), 1, bin_index);
    fwrite(&node->P3, sizeof(int), 1, bin_index);

    teste = 1;
    for (int i = 0; i < 55; i++)
    {
        if (!teste)
        {
            teste = 0;
            fwrite(&aux, sizeof(char), 1, bin_index);
        }
        if (teste)
        {
            if (node->C3[i] == '\0')
            {
                teste = 0;
                fwrite(&aux, sizeof(char), 1, bin_index);
            }
        }
        if (teste)
        {
            fwrite(&node->C3[i], sizeof(char), 1, bin_index);
        }
    }
    fwrite(&node->PR3, sizeof(int), 1, bin_index);
    fwrite(&node->P4, sizeof(int), 1, bin_index);
    fflush(bin_index);
}

// Atualiza o header do arquivo  indice com as informações abaixo
void updateHeader(FILE *bin_index, char status, int rootNodeRRN, int *nodeIndexRRN)
{
    fseek(bin_index, 0, SEEK_SET);

    fwrite(&status, sizeof(char), 1, bin_index);
    if (rootNodeRRN != -1)
    {
        fwrite(&rootNodeRRN, sizeof(int), 1, bin_index);
    }
    else
    {
        fseek(bin_index, 4, SEEK_CUR);
    }
    fwrite(nodeIndexRRN, sizeof(int), 1, bin_index);
    fflush(bin_index);
}
