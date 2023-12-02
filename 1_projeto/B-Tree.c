#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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

    printf("H|RRN|%1d|%2d|", height, node->RRNdoNo);
    for (int i = 0; i < 6 - height; i++)
    {
        printf("\t");
    }
    printf("<%d>", height);

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

    printf("||%2d||%25s ->(%2d) ||%2d||%25s ->(%2d) ||%2d||%25s ->(%2d) ||%2d||\n", node->P1, chave1, node->PR1, node->P2, chave2, node->PR2, node->P3, chave3, node->PR3, node->P4);
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
/* Função implementada por mim, mas não mais usada. strcmp();
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
*/
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

static void shiftPointers(BTreeNode *dadoNode, BTreeNode *newRight, int place)
{
    switch (place)
    {
    case 1:

        dadoNode->P4 = dadoNode->P3;
        dadoNode->P3 = dadoNode->P2;
        dadoNode->P2 = newRight->RRNdoNo;
        break;
    case 2:
        dadoNode->P4 = dadoNode->P3;
        dadoNode->P3 = newRight->RRNdoNo;
        break;
    case 3:
        dadoNode->P4 = newRight->RRNdoNo;
        break;
    default:
        break;
    }
}

static void shiftSplitPointers(BTreeNode *leftNode, BTreeNode *BottomRight, BTreeNode *topRight, int *pointers, int place)
{

    switch (place)
    {
    case 1:

        topRight->P1 = pointers[2];
        topRight->P2 = pointers[3];

        leftNode->P2 = BottomRight->RRNdoNo;
        leftNode->P3 = pointers[1];

        leftNode->P4 = -1;
        break;
    case 2:
        topRight->P1 = pointers[2];
        topRight->P2 = pointers[3];

        leftNode->P3 = BottomRight->RRNdoNo;
        leftNode->P4 = -1;
        break;
    case 3:
        topRight->P1 = BottomRight->RRNdoNo;
        topRight->P2 = pointers[3];

        leftNode->P4 = -1;
        break;
    case 4:
        topRight->P1 = pointers[3];
        topRight->P2 = BottomRight->RRNdoNo;

        leftNode->P4 = -1;
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

// Insere o nó no arquivo na pos    ição em que estiver o arquivo de indice
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

int *promotePointers(BTreeNode *node, int pointers[])
{
    pointers[0] = node->P1;
    pointers[1] = node->P2;
    pointers[2] = node->P3;
    pointers[3] = node->P4;
    return pointers;
}

static BTreeNode *splitNode(FILE *bin_index, BTreeNode *childNode, BTreeNode *newRight, char *aux, int referenceRRN)
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
    updateBinArchive(bin_index, childNode, childNode->RRNdoNo);
    updateBinArchive(bin_index, newRight, newRight->RRNdoNo);

    newRight->promoted_aux = vector;
    newRight->priPromoted = priPromoted;
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
    int rootNodeValue;
    char status;
    fseek(bin_index, 0, SEEK_SET); // Vai para o começo do arquivo
    fread(&status, sizeof(char), 1, bin_index);
    fread(&rootNodeValue, sizeof(int), 1, bin_index); // Lê a raiz
    if (rootNodeValue == -1)
        rootNodeValue = 0;
    root = readIndexRegister(bin_index, root, rootNodeValue);
    return root;
}

/*
BTreeNode *getRootBackup(FILE *bin_index, BTreeNode *root)
{
    int a, b, counter = 0;
    do
    {
        counter++;
        a = 1, b = 1;
        int rootNodeValue;
        char status;
        rewind(bin_index);             // Rebobina o arquivo para o início
        fseek(bin_index, 0, SEEK_SET); // Vai para o começo do arquivo
        a = fread(&status, sizeof(char), 1, bin_index);
        b = fread(&rootNodeValue, sizeof(int), 1, bin_index); // Lê a raiz
        if (rootNodeValue == -1)
            rootNodeValue = 0;
        // if (a == 0 || b == 0)
        // {
        //     // perror("Error reading from file");
        //     // fprintf(stderr, "errno: %d, strerror: %s\n", errno, strerror(errno));
        //     printf("\t\n\n\natualizou o arquivo\t");
        //     fclose(bin_index);
        //     bin_index = fopen(outArchiveName, "rb+");
        // }
        // printf("fseek é:%d   |  status %d e rootNode é: %d\n", testeFSEEK, a, b);
        root = readIndexRegister(bin_index, root, rootNodeValue);

    } while (a != 1 || b != 1);
    return root;
}
*/

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

// Testa e insere o dado dentro do arquivo de index

BTreeNode *intermediateSplit(FILE *bin_index, BTreeNode *root, BTreeNode *promotedNewRight, char **promoted, int *priPromoted, char *aux, int referenceRRN, int *nodeIndexRRN)
{
    BTreeNode *newBottomRight = promotedNewRight; //  Inicializa o novo nó direito superior
    BTreeNode *newTopperRight = initNode();       //  Inicializa o novo nó direito inferior

    newTopperRight->RRNdoNo = *nodeIndexRRN;
    (*nodeIndexRRN)++;

    int pointers[4];
    promotePointers(root, pointers);

    promoted = newBottomRight->promoted_aux;   //  Vetor auxiliar para promoção
    priPromoted = newBottomRight->priPromoted; //  Vetor auxiliar para promoção PRI
    char *bottonPromoted = promoted[2];
    int priBottonPromoted = priPromoted[2];
    promoted = promoteVector(root, promoted, bottonPromoted);
    priPromoted = priVector(root, priPromoted, bottonPromoted, priBottonPromoted);
    // Auxilia na promoção, ponteiros completos
    int where = whereToInsert(root, bottonPromoted);
    newTopperRight = splitNode(bin_index, root, newTopperRight, bottonPromoted, priBottonPromoted); //  Splita o nó da direita

    shiftSplitPointers(root, newBottomRight, newTopperRight, pointers, where); // splita os ponteiros no formato de promoção
    updateBinArchive(bin_index, root, root->RRNdoNo);
    updateBinArchive(bin_index, newTopperRight, newTopperRight->RRNdoNo);

    newTopperRight->promoted_aux = promoted;
    newTopperRight->priPromoted = priPromoted;
    return newTopperRight;
}

void newRootSplit(FILE *bin_index, BTreeNode *root, BTreeNode *promotedNewRight, char **promoted, int *priPromoted, char *aux, int referenceRRN, int *nodeIndexRRN)
{
    BTreeNode *newRoot = initNode();              //  Inicializa a nova raiz
    BTreeNode *newBottomRight = promotedNewRight; //  Inicializa o novo nó direito superior
    BTreeNode *newTopperRight = initNode();       //  Inicializa o novo nó direito inferior

    // char *bottomPromoted = promoted[2]; //  Auxiliar para pegar o nó promovido do segundo abaixo

    newTopperRight->RRNdoNo = *nodeIndexRRN;
    (*nodeIndexRRN)++;
    newRoot->RRNdoNo = *nodeIndexRRN;
    (*nodeIndexRRN)++;

    int pointers[4];
    promotePointers(root, pointers);

    char *bottomPromoted = newBottomRight->promoted_aux[2];
    int priBottomPromoted = newBottomRight->priPromoted[2];
    promoted = promoteVector(root, promoted, aux);                                 //  Vetor auxiliar para promoção
    priPromoted = priVector(root, priPromoted, bottomPromoted, priBottomPromoted); //  Vetor auxiliar para promoção PRI

    // Auxilia na promoção, ponteiros completos
    int where = whereToInsert(root, bottomPromoted);
    newTopperRight = splitNode(bin_index, root, newTopperRight, bottomPromoted, priBottomPromoted); //  Splita o nó da direita

    // shiftPointers(newRoot, newRight, whereToInsert(newRoot, promoted[2])); //  Shifta, dessa vez, os ponteiros no local correto
    shiftSplitPointers(root, newBottomRight, newTopperRight, pointers, where); // splita os ponteiros no formato de promoção
    // Nomeia os filhos e atribui o valor da raíz
    newRoot->P1 = root->RRNdoNo;
    newRoot->P2 = newTopperRight->RRNdoNo;
    strcpy(newRoot->C1, promoted[2]);
    newRoot->PR1 = priPromoted[2];

    updateBinArchive(bin_index, root, root->RRNdoNo);
    updateBinArchive(bin_index, newTopperRight, newTopperRight->RRNdoNo);
    updateBinArchive(bin_index, newRoot, newRoot->RRNdoNo);
    updateHeader(bin_index, '0', newRoot->RRNdoNo, nodeIndexRRN);
}

// Caso o nó não tenha mais filhos na direção desejada, insere ali mesmo ou promove através do split
BTreeNode *insertLocal(FILE *bin_index, BTreeNode *root, char **promoted, char *aux, int referenceRRN, int *nodeIndexRRN, int *highestTree)
{
    if (isAvailable(root))
    {
        shiftRightImplement(root, aux, referenceRRN, whereToInsert(root, aux));
        updateBinArchive(bin_index, root, root->RRNdoNo);
        return NULL;
    }
    else // split case
    {
        if (isRoot(bin_index, root, highestTree)) // split na raiz
        {
            int *priPromoted = (int *)malloc(sizeof(int));
            BTreeNode *newRoot = initNode();               //  Inicializa um novo nó raiz
            BTreeNode *newRight = initNode();              //  Inicializa um novo nó direito
            promoted = promoteVector(root, promoted, aux); //  Vetor auxiliar para promoção
            priPromoted = priVector(root, priPromoted, aux, referenceRRN);

            newRight->RRNdoNo = *nodeIndexRRN;
            (*nodeIndexRRN)++;
            newRoot->RRNdoNo = *nodeIndexRRN;
            (*nodeIndexRRN)++;

            newRight = splitNode(bin_index, root, newRight, aux, referenceRRN); //  Splita o nó da direita
            // shiftPointers(newRoot, newRight, whereToInsert(newRoot, promoted[2])); //  Shifta, dessa vez, os ponteiros no local correto

            // Nomeia os filhos e atribui o valor da raíz
            newRoot->P1 = root->RRNdoNo;
            newRoot->P2 = newRight->RRNdoNo;
            strcpy(newRoot->C1, promoted[2]);
            newRoot->PR1 = priPromoted[2];

            updateBinArchive(bin_index, newRoot, newRoot->RRNdoNo);
            updateHeader(bin_index, '0', newRoot->RRNdoNo, nodeIndexRRN);
            // newRootSplit(bin_index, root, newRight, promoted, priPromoted, aux, referenceRRN, nodeIndexRRN);
            return NULL;
        }
        else
        {
            // Caso não seja nem raíz nem um nó folha disponível, promove um novo nó direito com o dado promovido
            BTreeNode *newRight = initNode();
            newRight->RRNdoNo = *nodeIndexRRN;
            (*nodeIndexRRN)++;
            // newRight->promoted_aux = promoteVector(root, promoted, aux);
            // newRight->priPromoted = priVector(root, priPromoted, aux, referenceRRN);
            newRight = splitNode(bin_index, root, newRight, aux, referenceRRN); //  Splita o o nó direito inferior
            updateBinArchive(bin_index, newRight, newRight->RRNdoNo);
            return newRight; // para caso tenha que dar split
        }
    }
}

// Função principal de inserção da string e do RRN na no arquivo Índice de árvore B
BTreeNode *insertIndexString(FILE *bin_index, int node_inIndex, char *aux, int *highestTree, int *nodeIndexRRN, int referenceRRN)
{
    char **promoted = (char **)malloc(4 * sizeof(char *)); // 4 para promoção das strings em si e 4 para promover as PRis
    int *priPromoted;
    BTreeNode *newRight;
    BTreeNode *root = initNode();
    root = readIndexRegister(bin_index, root, node_inIndex);

    if (root->C1[0] == '$') // primeira inserção
    {
        insertInPlace(root, aux, referenceRRN, 1);
        root->RRNdoNo = *nodeIndexRRN;
        updateBinArchive(bin_index, root, 0);
        updateHeader(bin_index, '0', root->RRNdoNo, nodeIndexRRN);
        (*nodeIndexRRN)++;
        return NULL;
    }

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

    // Inserir na 1ª posição
    else if (strcmp(aux, root->C1) < 0 || root->C1[0] == '$')
    {
        if (root->P1 != -1)
        {
            if ((newRight = insertIndexString(bin_index, root->P1, aux, highestTree, nodeIndexRRN, referenceRRN)) != NULL)
            {
                // se subiu, promoveu o filho
                // acessorSun(); // caso tenha um nó abaixo, repassa o código recursivo pra lá
                promoted = newRight->promoted_aux;
                priPromoted = newRight->priPromoted;
                char *bottomPromoted = promoted[2];
                if (isAvailable(root))
                {
                    BTreeNode *childNode = initNode();
                    childNode = readIndexRegister(bin_index, childNode, root->P1);
                    shiftPointers(root, newRight, whereToInsert(root, promoted[2]));
                    shiftRightImplement(root, bottomPromoted, priPromoted[2], whereToInsert(root, bottomPromoted));

                    updateBinArchive(bin_index, root, root->RRNdoNo);
                    // updateBinArchive(bin_index, childNode, childNode->RRNdoNo);
                    // updateBinArchive(bin_index, newRight, newRight->RRNdoNo);
                    // updateHeader(bin_index, '1', root->RRNdoNo, nodeIndexRRN);
                    return NULL;
                }
                else // split de novo
                {
                    if (isRoot(bin_index, root, highestTree)) // split na raiz
                    {
                        newRootSplit(bin_index, root, newRight, promoted, priPromoted, bottomPromoted, referenceRRN, nodeIndexRRN);
                        return NULL;
                    }
                    else // split intermediário
                    {
                        return intermediateSplit(bin_index, root, newRight, promoted, priPromoted, aux, referenceRRN, nodeIndexRRN);
                    }
                }
            }
        }
        else
        {
            // insere localmente no nó, caso esteja cheio faz o split e promove pelo return um no nó direito
            return insertLocal(bin_index, root, promoted, aux, referenceRRN, nodeIndexRRN, highestTree);
        }
    }

    // Inserir na 2ª posição
    else if ((strcmp(aux, root->C1) > 0 && strcmp(aux, root->C2) < 0) || root->C2[0] == '$')
    {
        if (root->P2 != -1)
        {
            if ((newRight = insertIndexString(bin_index, root->P2, aux, highestTree, nodeIndexRRN, referenceRRN)) != NULL)
            {
                // se subiu, promoveu o filho
                // acessorSun(); // caso tenha um nó abaixo, repassa o código recursivo pra lá
                promoted = newRight->promoted_aux;
                priPromoted = newRight->priPromoted;
                char *bottomPromoted = promoted[2];
                if (isAvailable(root))
                {
                    BTreeNode *childNode = initNode();
                    childNode = readIndexRegister(bin_index, childNode, root->P2);
                    shiftPointers(root, newRight, whereToInsert(root, promoted[2]));
                    shiftRightImplement(root, bottomPromoted, priPromoted[2], whereToInsert(root, bottomPromoted));

                    updateBinArchive(bin_index, root, root->RRNdoNo);
                    // updateBinArchive(bin_index, childNode, childNode->RRNdoNo);
                    // updateBinArchive(bin_index, newRight, newRight->RRNdoNo);
                    // updateHeader(bin_index, '1', root->RRNdoNo, nodeIndexRRN);
                    return NULL;
                }
                else // split de novo
                {
                    if (isRoot(bin_index, root, highestTree)) // split na raiz
                    {
                        newRootSplit(bin_index, root, newRight, promoted, priPromoted, bottomPromoted, referenceRRN, nodeIndexRRN);
                        return NULL;
                    }
                    else // split intermediário
                    {
                        return intermediateSplit(bin_index, root, newRight, promoted, priPromoted, aux, referenceRRN, nodeIndexRRN);
                    }
                }
            }
        }
        else
        {
            // insere localmente no nó, caso esteja cheio faz o split e promove pelo return um no nó direito
            return insertLocal(bin_index, root, promoted, aux, referenceRRN, nodeIndexRRN, highestTree);
        }
    }

    // Inserir na 3ª posição
    else if ((strcmp(aux, root->C2) > 0 && strcmp(aux, root->C3) < 0) || root->C3[0] == '$')
    {
        if (root->P3 != -1)
        {
            if ((newRight = insertIndexString(bin_index, root->P3, aux, highestTree, nodeIndexRRN, referenceRRN)) != NULL)
            {
                // se subiu, promoveu o filho
                promoted = newRight->promoted_aux;
                priPromoted = newRight->priPromoted;
                char *bottomPromoted = promoted[2];
                if (isAvailable(root))
                {
                    BTreeNode *childNode = initNode();
                    childNode = readIndexRegister(bin_index, childNode, root->P3);
                    shiftPointers(root, newRight, whereToInsert(root, promoted[2]));
                    shiftRightImplement(root, bottomPromoted, priPromoted[2], whereToInsert(root, bottomPromoted));

                    updateBinArchive(bin_index, root, root->RRNdoNo);
                    // updateBinArchive(bin_index, childNode, childNode->RRNdoNo);
                    // updateBinArchive(bin_index, newRight, newRight->RRNdoNo);
                    // updateHeader(bin_index, '1', root->RRNdoNo, nodeIndexRRN);
                    return NULL;
                }
                else // split de novo
                {
                    if (isRoot(bin_index, root, highestTree)) // split na raiz
                    {
                        newRootSplit(bin_index, root, newRight, promoted, priPromoted, bottomPromoted, referenceRRN, nodeIndexRRN);
                        return NULL;
                    }
                    else // split intermediário
                    {
                        return intermediateSplit(bin_index, root, newRight, promoted, priPromoted, aux, referenceRRN, nodeIndexRRN);
                    }
                }
            }
        }
        else
        {
            // insere localmente no nó, caso esteja cheio faz o split e promove pelo return um no nó direito
            return insertLocal(bin_index, root, promoted, aux, referenceRRN, nodeIndexRRN, highestTree);
        }
    }

    // Inserir na ultima posição
    else if (strcmp(aux, root->C3) > 0)
    {

        if (root->P4 != -1)
        {
            if ((newRight = insertIndexString(bin_index, root->P4, aux, highestTree, nodeIndexRRN, referenceRRN)) != NULL)
            {
                // se subiu, promoveu o filho
                // acessorSun(); // caso tenha um nó abaixo, repassa o código recursivo pra lá
                promoted = newRight->promoted_aux;
                priPromoted = newRight->priPromoted;
                char *bottomPromoted = promoted[2];
                if (isAvailable(root))
                {
                    BTreeNode *childNode = initNode();
                    childNode = readIndexRegister(bin_index, childNode, root->P4);
                    shiftPointers(root, newRight, whereToInsert(root, promoted[2]));
                    shiftRightImplement(root, bottomPromoted, priPromoted[2], whereToInsert(root, bottomPromoted));

                    updateBinArchive(bin_index, root, root->RRNdoNo);
                    // updateBinArchive(bin_index, childNode, childNode->RRNdoNo);
                    // updateBinArchive(bin_index, newRight, newRight->RRNdoNo);
                    // updateHeader(bin_index, '1', root->RRNdoNo, nodeIndexRRN);
                    return NULL;
                }
                else // split de novo
                {
                    if (isRoot(bin_index, root, highestTree)) // split na raiz
                    {
                        newRootSplit(bin_index, root, newRight, promoted, priPromoted, bottomPromoted, referenceRRN, nodeIndexRRN);
                        return NULL;
                    }
                    else // split intermediário
                    {
                        return intermediateSplit(bin_index, root, newRight, promoted, priPromoted, aux, referenceRRN, nodeIndexRRN);
                    }
                }
            }
        }
        else
        {
            // insere localmente no nó, caso esteja cheio faz o split e promove pelo return um no nó direito
            return insertLocal(bin_index, root, promoted, aux, referenceRRN, nodeIndexRRN, highestTree);
        }
    }
    return NULL;
}