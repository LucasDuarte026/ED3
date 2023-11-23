#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structsBTree.h"
#include "B-Tree.h"

/*  Inicializa a raiz   */
BTreeNode *initNode()
{
    // Os ponteiros são inicializados apontando para NULL,
    // para melhor controle dentro das funções de inserção, asim como discutido com o monitor

    BTreeNode *newNode = (BTreeNode *)malloc(sizeof(BTreeNode));
    newNode->alturaNo = -1;
    newNode->nroChavesNo = -1;
    newNode->RRNdoNo = -1;

    newNode->C1 = NULL;
    newNode->C2 = NULL;
    newNode->C3 = NULL;

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

static int isAvailable(char *text)
{
    if (text)
    {
        return 0; // Texto  existe, portanto NÃO está disponível
    }
    return 1; // Texto não existe, é null, portanto ESTÁ disponível
}

/* Insere o valor de fato no local pedido */
static void insertInPlace(BTreeNode *node, char *aux,int referenceRRN, int place)
{
    switch (place)
    {
    case 1:
        node->C1 = strdup(aux);
        node->PR1 = referenceRRN;
        break;
    case 2:
        node->C2 = strdup(aux);
        node->PR2 = referenceRRN;
        break;
    case 3:
        node->C3 = strdup(aux);
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

static void shiftRightImplement(BTreeNode *node, char *aux, int place)
{
    switch (place)
    {
    case 1:
        node->C3 = node->C2;
        node->C2 = node->C1;
        node->C1 = strdup(aux);
        break;
    case 2:
        node->C3 = node->C2;
        node->C2 = strdup(aux);
        break;
    case 3:
        node->C3 = strdup(aux);
        break;
    case 4:
        node->C1 = strdup(aux);
        break;
    default:
        break;
    }
}
/*
static void shiftPointers(BTreeNode *dadNode, BTreeNode *childNode, BTreeNode *newRight, int place)
{
    switch (place)
    {
    case 1:

        dadNode->P4 = dadNode->P3;
        dadNode->P3 = dadNode->P2;
        dadNode->P2 = newRight;
        break;
    case 2:
        dadNode->P4 = dadNode->P3;
        dadNode->P3 = newRight;
        break;
    case 3:
        dadNode->P4 = newRight;
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

static void shiftSplitPointers(BTreeNode *leftNode, BTreeNode *BottomRight, BTreeNode *topRight, BTreeNode **pointers, int place)
{

    switch (place)
    {
    case 1:

        topRight->P1 = pointers[1];
        topRight->P2 = pointers[2];
        topRight->P3 = pointers[3];

        leftNode->P2 = BottomRight;
        leftNode->P3 = NULL;
        leftNode->P4 = NULL;
        break;
    case 2:
        topRight->P1 = BottomRight;
        topRight->P2 = pointers[2];
        topRight->P3 = pointers[3];

        leftNode->P3 = NULL;
        leftNode->P4 = NULL;
        break;
    case 3:
        topRight->P1 = pointers[2];
        topRight->P2 = BottomRight;
        topRight->P3 = pointers[3];

        leftNode->P3 = NULL;
        leftNode->P4 = NULL;
        break;
    case 4:
        topRight->P1 = pointers[2];
        topRight->P2 = pointers[3];
        topRight->P3 = BottomRight;
        leftNode->P3 = NULL;
        leftNode->P4 = NULL;
        break;
    default:
        break;
    }
}
*/
int whereToInsert(BTreeNode *node, char *aux)
{
    if (stringHigherThen(aux, node->C1, 0) == 0)
        return 1;
    if (stringHigherThen(aux, node->C1, 0) == 1)
        return 1;
    else if (stringHigherThen(aux, node->C1, 0) == 2 && stringHigherThen(aux, node->C2, 0) == 1)
        return 2;
    else if (stringHigherThen(aux, node->C2, 0) == 2 && stringHigherThen(aux, node->C3, 0) == 1)
        return 3;
    else if (stringHigherThen(aux, node->C3, 0) == 2)
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
static BTreeNode *splitNode(BTreeNode *childNode, BTreeNode *newRight, char *aux)
{
    char **vector = (char **)malloc(4 * sizeof(char *));
    vector = promoteVector(childNode, vector, aux);
    childNode->C1 = vector[0];
    // shiftRightImplement(dadNode, vector[1], whereToInsert(dadNode, aux));
    newRight->C1 = vector[2];
    newRight->C2 = vector[3];
    childNode->C2 = NULL;
    childNode->C3 = NULL;
    // shiftPointers(dadNode, childNode, newRight, whereToInsert(dadNode, vector[1]));
    return newRight;
}
/*
int isRoot(BTreeNode *root, int highestTree)
{
    int local_height = heightTree(root);
    if (local_height == 1 && local_height < highestTree)
    {
        return 0; //  É nó folha
    }
    else if (highestTree == local_height)
    {
        return 1; //  É nó Raiz
    }

    else
    {
        return 0; //  É nó intermediario
    }
}
*/
// Conta a quantidade de chaves armazenadas no nó
int keysQuant(BTreeNode *node)
{

    if (node->C3)
    {
        return 3;
    }
    else if (node->C2)
    {
        return 2;
    }
    else if (node->C1)
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

    char buffer[56];

    fgets(buffer, sizeof(buffer), bin_index);
    if (buffer[0] != '$')
    {
        for (int i = 0; i < sizeof(buffer); i++)
        {
            if (buffer[i] == '$')
                buffer[i] = '\0';
        }
        prt_root->C1 = strdup(buffer);
    }
    else
        prt_root->C3 = NULL;

    fread(&prt_root->PR1, sizeof(int), 1, bin_index);
    fread(&prt_root->P2, sizeof(int), 1, bin_index);

    fgets(buffer, sizeof(buffer), bin_index);
    if (buffer[0] != '$')
    {
        for (int i = 0; i < sizeof(buffer); i++)
        {
            if (buffer[i] == '$')
                buffer[i] = '\0';
        }
        prt_root->C2 = strdup(buffer);
    }
    else
        prt_root->C3 = NULL;

    fread(&prt_root->PR2, sizeof(int), 1, bin_index);
    fread(&prt_root->P3, sizeof(int), 1, bin_index);

    fgets(buffer, sizeof(buffer), bin_index);
    if (buffer[0] != '$')
    {
        for (int i = 0; i < sizeof(buffer); i++)
        {
            if (buffer[i] == '$')
                buffer[i] = '\0';
        }
        prt_root->C3 = strdup(buffer);
    }
    else
        prt_root->C3 = NULL;

    fread(&prt_root->PR3, sizeof(int), 1, bin_index);
    fread(&prt_root->P4, sizeof(int), 1, bin_index);
    return prt_root;
}

BTreeNode *getRoot(FILE *bin_index)
{
    BTreeNode *ptr_root = initNode();

    fseek(bin_index, 0, SEEK_SET); // Vai pro começo do arquivo
    Header header;
    fread(&header.status, sizeof(char), 1, bin_index);
    fread(&header.rootNode, sizeof(int), 1, bin_index); // lê a raiz

    int rootLocal = header.rootNode; // pega o destino da raiz

    ptr_root = readIndexRegister(bin_index, ptr_root, rootLocal);
    return ptr_root;
}
/*
void acessorSun()
{

    // Caso o filho retorne um novo newRight, entra aqui para processar a promoção de dado
    if (newRight = insertIndexString(bin_index, &(*root)->P1, aux, highestTree, RRN))
    {
        promoted = newRight->promoted_aux; // Usa a o auxiliar que vem junto com o nó promovido para pegar os dados promovidos

        if (isAvailable((*root)->C3) == 1) // Se estiver disponível o nó pai, já coloca direto
        {

            shiftPointers((*root), (*root)->P1, newRight, whereToInsert((*root), promoted[1])); // shifta os ponteiros pro lugar correto
            shiftRightImplement(*root, promoted[1], whereToInsert(*root, promoted[1]));         // Shift complementar que funciona apenas para promoções, rearranjando os ponteiros do split (P1, P2,P3,P4 e P5(na prática existe))
            return NULL;                                                                        //  Retorna NULL pois não promove
        }

        else if (isRoot((*root), (*highestTree))) //  Caso seja raíz e esteja recebendo uma promoção de dados
        {
            BTreeNode *newRoot = initNode();        //  Inicializa a nova raiz
            BTreeNode *newBottomRight = newRight;   //  Inicializa o novo nó direito superior
            BTreeNode *newTopperRight = initNode(); //  Inicializa o novo nó direito inferior

            char *bottomPromoted = promoted[1];                       //  Auxiliar para pegar o nó promovido do segundo abaixo
            promoted = promoteVector((*root), promoted, promoted[1]); //  Auxiliar para pegar o nó promovido do segundo abaixo
            pointers = promotePointers((*root), pointers);            //  Auxiliar para pegar os ponteiros corretos

            // newBottomRight = splitNode((*root)->P1, newBottomRight, aux);                                              // splita o nó debaixo
            newTopperRight = splitNode(*root, newTopperRight, bottomPromoted);                                         // Splita o nó de cima
            shiftSplitPointers(*root, newBottomRight, newTopperRight, pointers, whereToInsert(*root, bottomPromoted)); // splita os ponteiros no formato de promoção

            // Nomeia os filhos e atribui o valor da raíz
            newRoot->P1 = *root;
            newRoot->P2 = newTopperRight;
            newRoot->C1 = promoted[1];
            *root = newRoot; // reestaura a nova raiz
            return NULL;     // retorna NULL pois não promove nada
        }

        // Caso seja um nó intermediário e tenha que fazer uma promoção
        else
        {
            BTreeNode *newBottomRight = newRight;   //  Inicializa o nó de baixo direito
            BTreeNode *newTopperRight = initNode(); //  Inicializa o nó de cima direito
            newTopperRight->data.RRNdoNo = *RRN;
            (*RRN)++;
            char *bottomPromoted = promoted[1];                       //  Pega o valor promovido idealmente para o nó superior
            promoted = promoteVector((*root), promoted, promoted[1]); // Auxilia na promoção, vetor completo
            pointers = promotePointers((*root), pointers);            // Auxilia na promoção, ponteiros completos

            // newBottomRight = splitNode((*root)->P1, newBottomRight, aux);                                              //  Splita o o nó direito inferior
            newTopperRight = splitNode(*root, newTopperRight, bottomPromoted);                                         //  Splita o o nó direito Superior
            shiftSplitPointers(*root, newBottomRight, newTopperRight, pointers, whereToInsert(*root, bottomPromoted)); //  Shifta os ponteiros para depois do split
            newTopperRight->promoted_aux = promoted;                                                                   // Atribui o vetor auxiliar para promoção
            return newTopperRight;                                                                                     //  Repassa o nó auxiliar
        }
    }
    return NULL; // ou retorna nada caso não entre nos casos anteriores
}

acessorNormal()
{

    // Caso não seja um nó, será nó folha, se for, insere
    if (isAvailable((*root)->C3))
    {
        shiftRightImplement(*root, aux, 1); // Dá um shift correto na inserção, colocando-o no local correto
        return NULL;                        //  Inserido corretamente sem precisar de promover
    }
    // Caso esse nó folha também seja raiz, produz uma nova raíz
    else if (isRoot((*root), (*highestTree)))
    {
        BTreeNode *newRoot = initNode();  //  Inicializa um novo nó raiz
        BTreeNode *newRight = initNode(); //  Inicializa um novo nó direito
        newRight->data.RRNdoNo = *RRN;
        (*RRN)++;
        newRoot->data.RRNdoNo = *RRN;
        (*RRN)++;
        promoted = promoteVector((*root), promoted, aux); //  Vetor auxiliar para promoção

        newRight = splitNode(*root, newRight, aux); //  Splita o nó da direita

        shiftPointers(newRoot, *root, newRight, whereToInsert(newRoot, promoted[1])); //  Shifta, dessa vez, os ponteiros no local correto

        // Nomeia os filhos e atribui o valor da raíz
        newRoot->P1 = *root;
        newRoot->P2 = newRight;
        newRoot->C1 = promoted[1];
        *root = newRoot; // reestaura a nova raiz
        return NULL;
    }
    // Caso não seja nem raíz nem um nó folha disponível, promove um novo nó direito com o dado promovido
    BTreeNode *newRight = initNode();
    newRight->data.RRNdoNo = *RRN;
    (*RRN)++;
    newRight->promoted_aux = promoteVector((*root), promoted, aux);
    newRight = splitNode((*root), newRight, aux); //  Splita o o nó direito inferior
    return newRight;                              // para caso tenha que dar split
}
*/
void updateBinArchive(FILE *bin_index, BTreeNode *node)
{

    node->nroChavesNo = keysQuant(node);
    node->alturaNo = heightTree(bin_index, node);
    char aux = '$';

    fseek(bin_index, 0, SEEK_END);
    fwrite(&node->nroChavesNo, sizeof(int), 1, bin_index);
    fwrite(&node->alturaNo, sizeof(int), 1, bin_index);
    fwrite(&node->RRNdoNo, sizeof(int), 1, bin_index);

    fwrite(&node->P1, sizeof(int), 1, bin_index);
    int teste = 1;
    for (int i = 0; i < 55; i++)
    {
        if (node->C1 == NULL || teste == 0)
        {
            teste = 0;
            fwrite(&aux, sizeof(char), 1, bin_index);
        }
        if (teste)
        {
            if (node->C1[i] == '\0')
                teste = 0;
        }
        if (teste)
        {
            fwrite(&node->C1[i], sizeof(char), 1, bin_index);
        }
    }
    fwrite(&node->PR1, sizeof(int), 1, bin_index);
    fwrite(&node->P2, sizeof(int), 1, bin_index);

    for (int i = 0; i < 55; i++)
    {
        if (node->C2 == NULL || teste == 0)
        {
            teste = 0;
            fwrite(&aux, sizeof(char), 1, bin_index);
        }
        if (teste)
        {
            if (node->C2[i] == '\0')
                teste = 0;
        }
        if (teste)
        {
            fwrite(&node->C1[i], sizeof(char), 1, bin_index);
        }
    }
    fwrite(&node->PR2, sizeof(int), 1, bin_index);
    fwrite(&node->P3, sizeof(int), 1, bin_index);

    for (int i = 0; i < 55; i++)
    {
        if (node->C1 == NULL || teste == 0)
        {
            teste = 0;
            fwrite(&aux, sizeof(char), 1, bin_index);
        }
        if (teste)
        {
            if (node->C1[i] == '\0')
                teste = 0;
        }
        if (teste)
        {
            fwrite(&node->C1[i], sizeof(char), 1, bin_index);
        }
    }
    fwrite(&node->PR3, sizeof(int), 1, bin_index);
    fwrite(&node->P4, sizeof(int), 1, bin_index);
}

/* Testa e insere o dado dentro do arquivo de index */
BTreeNode *insertIndexString(FILE *bin_index, char *aux, int *highestTree, int *nodeRRN, int referenceRRN)
{
    // char **promoted = (char **)malloc(4 * sizeof(char *));
    char promoted[4][55];

    // BTreeNode *newRight = initNode();
    BTreeNode *root = getRoot(bin_index);
    if (root->alturaNo == -1)
        root = initNode();
    if (*highestTree < heightTree(bin_index, root))
        *highestTree = heightTree(bin_index, root);
    if (root->C1 == NULL)
    {
        insertInPlace(root, aux,referenceRRN, 1);
        root->RRNdoNo = *nodeRRN;
        (*nodeRRN)++;
        updateBinArchive(bin_index, root);
    }
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

    // Não inserir pois é elemento repetido
    else if (stringHigherThen(aux, root->C1, 0) == 0 ||
             stringHigherThen(aux, root->C2, 0) == 0 ||
             stringHigherThen(aux, root->C3, 0) == 0)
    {
        printf("aux: %s não armazenado, eh igual um dos indices\n", aux);
    }

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

    // Inserir na 1ª posição
    else if (stringHigherThen(aux, root->C1, 0) == 1)
    {
        // if (root->P1 != -1)
        // {
        //     acessorSun(); // caso tenha um nó abaixo, repassa o código recursivo pra lá
        // }
        // else
        // {
        //     acessorNormal(); // insere no nó mesmo
        // }
    }

    // Inserir na ultima posição
    else if (stringHigherThen(aux, root->C3, 0) == 2)
    {
    }
}

/*s
// Testa e insere o dado dentro do arquivo de index
BTreeNode *insertIndexStringbackup(BTreeNode **root, char *aux, int *highestTree, int *RRN)
{
    char **promoted = (char **)malloc(4 * sizeof(char *));
    BTreeNode *newRight = initNode();

    BTreeNode **pointers = (BTreeNode **)malloc(4 * sizeof(BTreeNode *));
    if (*highestTree < heightTree((*root)))
        *highestTree = heightTree((*root));

    if ((*root)->C1 == NULL)
    {
        insertInPlace(*root, aux, 1);
        (*root)->data.RRNdoNo = *RRN;
        (*RRN)++;
    }
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

    // Não inserir pois é elemento repetido
    else if (stringHigherThen(aux, (*root)->C1, 0) == 0 ||
             stringHigherThen(aux, (*root)->C2, 0) == 0 ||
             stringHigherThen(aux, (*root)->C3, 0) == 0)
    {
        printf("aux: %s não armazenado, eh igual um dos indices\n", aux);
    }

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

    // Inserir na 1ª posição
    else if (stringHigherThen(aux, (*root)->C1, 0) == 1)
    {
        if (isNode((*root)->P1)) // caso tenha um nó abaixo , repassa o código recursivo pra lá
        {
            // Caso o filho retorne um novo newRight, entra aqui para processar a promoção de dado
            if (newRight = insertIndexString(&(*root)->P1, aux, highestTree, RRN))
            {
                promoted = newRight->promoted_aux; // Usa a o auxiliar que vem junto com o nó promovido para pegar os dados promovidos

                if (isAvailable((*root)->C3) == 1) // Se estiver disponível o nó pai, já coloca direto
                {

                    shiftPointers((*root), (*root)->P1, newRight, whereToInsert((*root), promoted[1])); // shifta os ponteiros pro lugar correto
                    shiftRightImplement(*root, promoted[1], whereToInsert(*root, promoted[1]));         // Shift complementar que funciona apenas para promoções, rearranjando os ponteiros do split (P1, P2,P3,P4 e P5(na prática existe))
                    return NULL;                                                                        //  Retorna NULL pois não promove
                }

                else if (isRoot((*root), (*highestTree))) //  Caso seja raíz e esteja recebendo uma promoção de dados
                {
                    BTreeNode *newRoot = initNode();        //  Inicializa a nova raiz
                    BTreeNode *newBottomRight = newRight;   //  Inicializa o novo nó direito superior
                    BTreeNode *newTopperRight = initNode(); //  Inicializa o novo nó direito inferior

                    char *bottomPromoted = promoted[1];                       //  Auxiliar para pegar o nó promovido do segundo abaixo
                    promoted = promoteVector((*root), promoted, promoted[1]); //  Auxiliar para pegar o nó promovido do segundo abaixo
                    pointers = promotePointers((*root), pointers);            //  Auxiliar para pegar os ponteiros corretos

                    // newBottomRight = splitNode((*root)->P1, newBottomRight, aux);                                              // splita o nó debaixo
                    newTopperRight = splitNode(*root, newTopperRight, bottomPromoted);                                         // Splita o nó de cima
                    shiftSplitPointers(*root, newBottomRight, newTopperRight, pointers, whereToInsert(*root, bottomPromoted)); // splita os ponteiros no formato de promoção

                    // Nomeia os filhos e atribui o valor da raíz
                    newRoot->P1 = *root;
                    newRoot->P2 = newTopperRight;
                    newRoot->C1 = promoted[1];
                    *root = newRoot; // reestaura a nova raiz
                    return NULL;     // retorna NULL pois não promove nada
                }

                // Caso seja um nó intermediário e tenha que fazer uma promoção
                else
                {
                    BTreeNode *newBottomRight = newRight;   //  Inicializa o nó de baixo direito
                    BTreeNode *newTopperRight = initNode(); //  Inicializa o nó de cima direito
                    newTopperRight->data.RRNdoNo = *RRN;
                    (*RRN)++;
                    char *bottomPromoted = promoted[1];                       //  Pega o valor promovido idealmente para o nó superior
                    promoted = promoteVector((*root), promoted, promoted[1]); // Auxilia na promoção, vetor completo
                    pointers = promotePointers((*root), pointers);            // Auxilia na promoção, ponteiros completos

                    // newBottomRight = splitNode((*root)->P1, newBottomRight, aux);                                              //  Splita o o nó direito inferior
                    newTopperRight = splitNode(*root, newTopperRight, bottomPromoted);                                         //  Splita o o nó direito Superior
                    shiftSplitPointers(*root, newBottomRight, newTopperRight, pointers, whereToInsert(*root, bottomPromoted)); //  Shifta os ponteiros para depois do split
                    newTopperRight->promoted_aux = promoted;                                                                   // Atribui o vetor auxiliar para promoção
                    return newTopperRight;                                                                                     //  Repassa o nó auxiliar
                }
            }
            return NULL; // ou retorna nada caso não entre nos casos anteriores
        }
        else
        {
            // Caso não seja um nó, será nó folha, se for, insere
            if (isAvailable((*root)->C3))
            {
                shiftRightImplement(*root, aux, 1); // Dá um shift correto na inserção, colocando-o no local correto
                return NULL;                        //  Inserido corretamente sem precisar de promover
            }
            // Caso esse nó folha também seja raiz, produz uma nova raíz
            else if (isRoot((*root), (*highestTree)))
            {
                BTreeNode *newRoot = initNode();  //  Inicializa um novo nó raiz
                BTreeNode *newRight = initNode(); //  Inicializa um novo nó direito
                newRight->data.RRNdoNo = *RRN;
                (*RRN)++;
                newRoot->data.RRNdoNo = *RRN;
                (*RRN)++;
                promoted = promoteVector((*root), promoted, aux); //  Vetor auxiliar para promoção

                newRight = splitNode(*root, newRight, aux); //  Splita o nó da direita

                shiftPointers(newRoot, *root, newRight, whereToInsert(newRoot, promoted[1])); //  Shifta, dessa vez, os ponteiros no local correto

                // Nomeia os filhos e atribui o valor da raíz
                newRoot->P1 = *root;
                newRoot->P2 = newRight;
                newRoot->C1 = promoted[1];
                *root = newRoot; // reestaura a nova raiz
                return NULL;
            }
            // Caso não seja nem raíz nem um nó folha disponível, promove um novo nó direito com o dado promovido
            BTreeNode *newRight = initNode();
            newRight->data.RRNdoNo = *RRN;
            (*RRN)++;
            newRight->promoted_aux = promoteVector((*root), promoted, aux);
            newRight = splitNode((*root), newRight, aux); //  Splita o o nó direito inferior
            return newRight;                              // para caso tenha que dar split
        }
    }
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // Inserir na 2ª posição
    else if (stringHigherThen(aux, (*root)->C1, 0) == 2 && stringHigherThen(aux, (*root)->C2, 0) == 1)
    {
        if (isNode((*root)->P2)) // caso tenha um nó abaixo , repassa o código recursivo pra lá
        {
            // Caso o filho retorne um novo newRight, entra aqui para processar a promoção de dado
            if (newRight = insertIndexString(&(*root)->P2, aux, highestTree, RRN))
            {
                promoted = newRight->promoted_aux; // Usa a o auxiliar que vem junto com o nó promovido para pegar os dados promovidos

                if (isAvailable((*root)->C3) == 1) // Se estiver disponível o nó pai, já coloca direto
                {
                    shiftPointers((*root), (*root)->P2, newRight, whereToInsert((*root), promoted[1])); // shifta os ponteiros pro lugar correto
                    shiftRightImplement(*root, promoted[1], whereToInsert(*root, promoted[1]));         // Shift complementar que funciona apenas para promoções, rearranjando os ponteiros do split (P1, P2,P3,P4 e P5(na prática existe))
                    return NULL;                                                                        //  Retorna NULL pois não promove
                }

                else if (isRoot((*root), (*highestTree))) //  Caso seja raíz e esteja recebendo uma promoção de dados
                {
                    BTreeNode *newRoot = initNode();        //  Inicializa a nova raiz
                    BTreeNode *newBottomRight = initNode(); //  Inicializa o novo nó direito superior
                    BTreeNode *newTopperRight = initNode(); //  Inicializa o novo nó direito inferior

                    char *bottomPromoted = promoted[1];                       //  Auxiliar para pegar o nó promovido do segundo abaixo
                    promoted = promoteVector((*root), promoted, promoted[1]); //  Auxiliar para pegar o nó promovido do segundo abaixo
                    pointers = promotePointers((*root), pointers);            //  Auxiliar para pegar os ponteiros corretos

                    newBottomRight = splitNode((*root)->P2, newBottomRight, aux);                                              // splita o nó debaixo
                    newTopperRight = splitNode(*root, newTopperRight, bottomPromoted);                                         // Splita o nó de cima
                    shiftSplitPointers(*root, newBottomRight, newTopperRight, pointers, whereToInsert(*root, bottomPromoted)); // splita os ponteiros no formato de promoção

                    // Nomeia os filhos e atribui o valor da raíz
                    newRoot->P1 = *root;
                    newRoot->P2 = newTopperRight;
                    newRoot->C1 = promoted[1];
                    *root = newRoot; // reestaura a nova raiz
                    return NULL;     // retorna NULL pois não promove nada
                }

                // Caso seja um nó intermediário e tenha que fazer uma promoção
                else
                {
                    BTreeNode *newBottomRight = newRight;   //  Inicializa o nó de baixo direito
                    BTreeNode *newTopperRight = initNode(); //  Inicializa o nó de cima direito
                    newTopperRight->data.RRNdoNo = *RRN;
                    (*RRN)++;
                    char *bottomPromoted = promoted[1];                       //  Pega o valor promovido idealmente para o nó superior
                    promoted = promoteVector((*root), promoted, promoted[1]); // Auxilia na promoção, vetor completo
                    pointers = promotePointers((*root), pointers);            // Auxilia na promoção, ponteiros completos

                    // newBottomRight = splitNode((*root)->P2, newBottomRight, aux);                                              //  Splita o o nó direito inferior
                    newTopperRight = splitNode(*root, newTopperRight, bottomPromoted);                                         //  Splita o o nó direito Superior
                    shiftSplitPointers(*root, newBottomRight, newTopperRight, pointers, whereToInsert(*root, bottomPromoted)); //  Shifta os ponteiros para depois do split
                    newTopperRight->promoted_aux = promoted;                                                                   // Atribui o vetor auxiliar para promoção
                    return newTopperRight;                                                                                     //  Repassa o nó auxiliar
                }
            }
            return NULL; // ou retorna nada caso não entre nos casos anteriores
        }
        else
        {
            // Caso não seja um nó, será nó folha, se for, insere
            if (isAvailable((*root)->C3))
            {
                shiftRightImplement(*root, aux, 1); // Dá um shift correto na inserção, colocando-o no local correto
                return NULL;                        //  Inserido corretamente sem precisar de promover
            }
            // Caso esse nó folha também seja raiz, produz uma nova raíz
            else if (isRoot((*root), (*highestTree)))
            {
                BTreeNode *newRoot = initNode();  //  Inicializa um novo nó raiz
                BTreeNode *newRight = initNode(); //  Inicializa um novo nó direito
                newRight->data.RRNdoNo = *RRN;
                (*RRN)++;
                newRoot->data.RRNdoNo = *RRN;
                (*RRN)++;
                promoted = promoteVector((*root), promoted, aux); //  Vetor auxiliar para promoção

                newRight = splitNode(*root, newRight, aux); //  Splita o nó da direita

                shiftPointers(newRoot, *root, newRight, whereToInsert(newRoot, promoted[1])); //  Shifta, dessa vez, os ponteiros no local correto

                // Nomeia os filhos e atribui o valor da raíz
                newRoot->P1 = *root;
                newRoot->P2 = newRight;
                newRoot->C1 = promoted[1];
                *root = newRoot; // reestaura a nova raiz
                return NULL;
            }
            // Caso não seja nem raíz nem um nó folha disponível, promove um novo nó direito com o dado promovido
            BTreeNode *newRight = initNode();
            newRight->data.RRNdoNo = *RRN;
            (*RRN)++;
            newRight->promoted_aux = promoteVector((*root), promoted, aux);
            newRight = splitNode((*root), newRight, aux); //  Splita o o nó direito inferior
            return newRight;                              // para caso tenha que dar split
        }
    }

    // Inserir na 3ª posição
    else if (stringHigherThen(aux, (*root)->C2, 0) == 2 && stringHigherThen(aux, (*root)->C3, 0) == 1)
    {
        if (isNode((*root)->P3)) // caso tenha um nó abaixo , repassa o código recursivo pra lá
        {
            // Caso o filho retorne um novo newRight, entra aqui para processar a promoção de dado
            if (newRight = insertIndexString(&(*root)->P3, aux, highestTree, RRN))
            {
                promoted = newRight->promoted_aux; // Usa a o auxiliar que vem junto com o nó promovido para pegar os dados promovidos

                if (isAvailable((*root)->C3) == 1) // Se estiver disponível o nó pai, já coloca direto
                {
                    shiftPointers((*root), (*root)->P3, newRight, whereToInsert((*root), promoted[1])); // shifta os ponteiros pro lugar correto
                    shiftRightImplement(*root, promoted[1], whereToInsert(*root, promoted[1]));         // Shift complementar que funciona apenas para promoções, rearranjando os ponteiros do split (P1, P2,P3,P4 e P5(na prática existe))
                    return NULL;                                                                        //  Retorna NULL pois não promove
                }

                else if (isRoot((*root), (*highestTree))) //  Caso seja raíz e esteja recebendo uma promoção de dados
                {
                    BTreeNode *newRoot = initNode();        //  Inicializa a nova raiz
                    BTreeNode *newBottomRight = newRight;   //  Inicializa o novo nó direito superior
                    BTreeNode *newTopperRight = initNode(); //  Inicializa o novo nó direito inferior

                    char *bottomPromoted = promoted[1];                       //  Auxiliar para pegar o nó promovido do segundo abaixo
                    promoted = promoteVector((*root), promoted, promoted[1]); //  Auxiliar para pegar o nó promovido do segundo abaixo
                    pointers = promotePointers((*root), pointers);            //  Auxiliar para pegar os ponteiros corretos

                    // newBottomRight = splitNode((*root)->P3, newBottomRight, aux);                                              // splita o nó debaixo
                    newTopperRight = splitNode(*root, newTopperRight, bottomPromoted);                                         // Splita o nó de cima
                    shiftSplitPointers(*root, newBottomRight, newTopperRight, pointers, whereToInsert(*root, bottomPromoted)); // splita os ponteiros no formato de promoção

                    // Nomeia os filhos e atribui o valor da raíz
                    newRoot->P1 = *root;
                    newRoot->P2 = newTopperRight;
                    newRoot->C1 = promoted[1];
                    *root = newRoot; // reestaura a nova raiz
                    return NULL;     // retorna NULL pois não promove nada
                }

                // Caso seja um nó intermediário e tenha que fazer uma promoção
                else
                {
                    BTreeNode *newBottomRight = newRight;   //  Inicializa o nó de baixo direito
                    BTreeNode *newTopperRight = initNode(); //  Inicializa o nó de cima direito
                    newTopperRight->data.RRNdoNo = *RRN;
                    (*RRN)++;
                    char *bottomPromoted = promoted[1];                       //  Pega o valor promovido idealmente para o nó superior
                    promoted = promoteVector((*root), promoted, promoted[1]); // Auxilia na promoção, vetor completo
                    pointers = promotePointers((*root), pointers);            // Auxilia na promoção, ponteiros completos

                    // newBottomRight = splitNode((*root)->P3, newBottomRight, aux);                                              //  Splita o o nó direito inferior
                    newTopperRight = splitNode(*root, newTopperRight, bottomPromoted);                                         //  Splita o o nó direito Superior
                    shiftSplitPointers(*root, newBottomRight, newTopperRight, pointers, whereToInsert(*root, bottomPromoted)); //  Shifta os ponteiros para depois do split
                    newTopperRight->promoted_aux = promoted;                                                                   // Atribui o vetor auxiliar para promoção
                    return newTopperRight;                                                                                     //  Repassa o nó auxiliar
                }
            }
            return NULL; // ou retorna nada caso não entre nos casos anteriores
        }
        else
        {
            // Caso não seja um nó, será nó folha, se for, insere
            if (isAvailable((*root)->C3))
            {
                shiftRightImplement(*root, aux, 1); // Dá um shift correto na inserção, colocando-o no local correto
                return NULL;                        //  Inserido corretamente sem precisar de promover
            }
            // Caso esse nó folha também seja raiz, produz uma nova raíz
            else if (isRoot((*root), (*highestTree)))
            {
                BTreeNode *newRoot = initNode();  //  Inicializa um novo nó raiz
                BTreeNode *newRight = initNode(); //  Inicializa um novo nó direito
                newRight->data.RRNdoNo = *RRN;
                (*RRN)++;
                newRoot->data.RRNdoNo = *RRN;
                (*RRN)++;
                promoted = promoteVector((*root), promoted, aux); //  Vetor auxiliar para promoção

                newRight = splitNode(*root, newRight, aux); //  Splita o nó da direita

                shiftPointers(newRoot, *root, newRight, whereToInsert(newRoot, promoted[1])); //  Shifta, dessa vez, os ponteiros no local correto

                // Nomeia os filhos e atribui o valor da raíz
                newRoot->P1 = *root;
                newRoot->P2 = newRight;
                newRoot->C1 = promoted[1];
                *root = newRoot; // reestaura a nova raiz
                return NULL;
            }
            // Caso não seja nem raíz nem um nó folha disponível, promove um novo nó direito com o dado promovido
            BTreeNode *newRight = initNode();
            newRight->data.RRNdoNo = *RRN;
            (*RRN)++;
            newRight->promoted_aux = promoteVector((*root), promoted, aux);
            newRight = splitNode((*root), newRight, aux); //  Splita o o nó direito inferior
            return newRight;                              // para caso tenha que dar split
        }
    }

    // Inserir na ultima posição
    else if (stringHigherThen(aux, (*root)->C3, 0) == 2)
    {
        if (isNode((*root)->P4)) // caso tenha um nó abaixo , repassa o código recursivo pra lá
        {
            // Caso o filho retorne um novo newRight, entra aqui para processar a promoção de dado
            if (newRight = insertIndexString(&(*root)->P4, aux, highestTree, RRN))
            {
                promoted = newRight->promoted_aux; // Usa a o auxiliar que vem junto com o nó promovido para pegar os dados promovidos

                if (isAvailable((*root)->C3) == 1) // Se estiver disponível o nó pai, já coloca direto
                {
                    shiftPointers((*root), (*root)->P4, newRight, whereToInsert((*root), promoted[1])); // shifta os ponteiros pro lugar correto
                    shiftRightImplement(*root, promoted[1], whereToInsert(*root, promoted[1]));         // Shift complementar que funciona apenas para promoções, rearranjando os ponteiros do split (P1, P2,P3,P4 e P5(na prática existe))
                    return NULL;                                                                        //  Retorna NULL pois não promove
                }

                else if (isRoot((*root), (*highestTree))) //  Caso seja raíz e esteja recebendo uma promoção de dados
                {
                    BTreeNode *newRoot = initNode();        //  Inicializa a nova raiz
                    BTreeNode *newBottomRight = newRight;   //  Inicializa o novo nó direito superior
                    BTreeNode *newTopperRight = initNode(); //  Inicializa o novo nó direito inferior

                    char *bottomPromoted = promoted[1];                       //  Auxiliar para pegar o nó promovido do segundo abaixo
                    promoted = promoteVector((*root), promoted, promoted[1]); //  Auxiliar para pegar o nó promovido do segundo abaixo
                    pointers = promotePointers((*root), pointers);            //  Auxiliar para pegar os ponteiros corretos

                    // newBottomRight = splitNode((*root)->P4, newBottomRight, aux);                                              // splita o nó debaixo
                    newTopperRight = splitNode(*root, newTopperRight, bottomPromoted);                                         // Splita o nó de cima
                    shiftSplitPointers(*root, newBottomRight, newTopperRight, pointers, whereToInsert(*root, bottomPromoted)); // splita os ponteiros no formato de promoção

                    // Nomeia os filhos e atribui o valor da raíz
                    newRoot->P1 = *root;
                    newRoot->P2 = newTopperRight;
                    newRoot->C1 = promoted[1];
                    *root = newRoot; // reestaura a nova raiz
                    return NULL;     // retorna NULL pois não promove nada
                }

                // Caso seja um nó intermediário e tenha que fazer uma promoção
                else
                {
                    BTreeNode *newBottomRight = newRight;   //  Inicializa o nó de baixo direito
                    BTreeNode *newTopperRight = initNode(); //  Inicializa o nó de cima direito
                    newTopperRight->data.RRNdoNo = *RRN;
                    (*RRN)++;
                    char *bottomPromoted = promoted[1];                       //  Pega o valor promovido idealmente para o nó superior
                    promoted = promoteVector((*root), promoted, promoted[1]); // Auxilia na promoção, vetor completo
                    pointers = promotePointers((*root), pointers);            // Auxilia na promoção, ponteiros completos

                    // newBottomRight = splitNode((*root)->P4, newBottomRight, aux);                                              //  Splita o o nó direito inferior
                    newTopperRight = splitNode(*root, newTopperRight, bottomPromoted);                                         //  Splita o o nó direito Superior
                    shiftSplitPointers(*root, newBottomRight, newTopperRight, pointers, whereToInsert(*root, bottomPromoted)); //  Shifta os ponteiros para depois do split
                    newTopperRight->promoted_aux = promoted;                                                                   // Atribui o vetor auxiliar para promoção
                    return newTopperRight;                                                                                     //  Repassa o nó auxiliar
                }
            }
            return NULL; // ou retorna nada caso não entre nos casos anteriores
        }
        else
        {
            // Caso não seja um nó, será nó folha, se for, insere
            if (isAvailable((*root)->C3))
            {
                shiftRightImplement(*root, aux, 1); // Dá um shift correto na inserção, colocando-o no local correto
                return NULL;                        //  Inserido corretamente sem precisar de promover
            }
            // Caso esse nó folha também seja raiz, produz uma nova raíz
            else if (isRoot((*root), (*highestTree)))
            {
                BTreeNode *newRoot = initNode();  //  Inicializa um novo nó raiz
                BTreeNode *newRight = initNode(); //  Inicializa um novo nó direito
                newRight->data.RRNdoNo = *RRN;
                (*RRN)++;
                newRoot->data.RRNdoNo = *RRN;
                (*RRN)++;
                promoted = promoteVector((*root), promoted, aux); //  Vetor auxiliar para promoção

                newRight = splitNode(*root, newRight, aux); //  Splita o nó da direita

                shiftPointers(newRoot, *root, newRight, whereToInsert(newRoot, promoted[1])); //  Shifta, dessa vez, os ponteiros no local correto

                // Nomeia os filhos e atribui o valor da raíz
                newRoot->P1 = *root;
                newRoot->P2 = newRight;
                newRoot->C1 = promoted[1];
                *root = newRoot; // reestaura a nova raiz
                return NULL;
            }
            // Caso não seja nem raíz nem um nó folha disponível, promove um novo nó direito com o dado promovido
            BTreeNode *newRight = initNode();
            newRight->data.RRNdoNo = *RRN;
            (*RRN)++;
            newRight->promoted_aux = promoteVector((*root), promoted, aux);
            newRight = splitNode((*root), newRight, aux); //  Splita o o nó direito inferior
            return newRight;                              // para caso tenha que dar split
        }
    }
}

*/