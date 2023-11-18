#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structsBTree.h"
#include "B-Tree.h"

/*  Inicializa a raiz   */
BTreeNode *initNode()
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

int heightTree(BTreeNode *root)
{

    int height = 0;
    if (root == NULL)
    {
        return 0;
    }
    else
    {
        if (root->P1 != NULL)
        {
            int chilhdHeight = heightTree(root->P1);
            if (chilhdHeight > height)
            {
                height = chilhdHeight;
            }
        }
        if (root->P2 != NULL)
        {
            int chilhdHeight = heightTree(root->P2);
            if (chilhdHeight > height)
            {
                height = chilhdHeight;
            }
        }

        if (root->P3 != NULL)
        {
            int chilhdHeight = heightTree(root->P3);
            if (chilhdHeight > height)
            {
                height = chilhdHeight;
            }
        }

        if (root->P4 != NULL)
        {
            int chilhdHeight = heightTree(root->P4);
            if (chilhdHeight > height)
            {
                height = chilhdHeight;
            }
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

int isNode(BTreeNode *node)
{
    if (node)
        return 1; //  Caso o nó exista
    else
    {
        return 0; // Caso o nó não exista, logo
    }
}

/* Insere o valor de fato no local pedido */
static void insertInPlace(BTreeNode *node, char *aux, int place)
{
    switch (place)
    {
    case 1:
        node->C1 = strdup(aux);
        break;
    case 2:
        node->C2 = strdup(aux);
        break;
    case 3:
        node->C3 = strdup(aux);
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

static void shiftPointers(BTreeNode *node, BTreeNode *newRight, int place)
{
    switch (place)
    {
    case 1:

        node->P4 = node->P3;
        node->P3 = node->P2;
        node->P2 = newRight;
        break;
    case 2:
        node->P4 = node->P3;
        node->P3 = newRight;
        break;
    case 3:
        node->P4 = newRight;
        break;
    case 4:
        printf("Vê o que vai rolar\n");
        break;
    default:
        break;
    }
}

int whereToInsert(BTreeNode *node, char *aux)
{
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

static void splitNode(BTreeNode *dadNode, BTreeNode *childNode, BTreeNode *newRight, char *aux)
{
    char **vector = (char **)malloc(4 * sizeof(char *));
    int where = whereToInsert(childNode, aux);
    switch (where)
    {
    case 1:
        vector[0] = strdup(aux);
        vector[1] = strdup(childNode->C1);
        vector[2] = strdup(childNode->C2);
        vector[3] = strdup(childNode->C3);

        shiftPointers(dadNode, newRight, where);
        break;
    case 2:
        vector[0] = strdup(childNode->C1);
        vector[1] = strdup(aux);
        vector[2] = strdup(childNode->C2);
        vector[3] = strdup(childNode->C3);

        shiftPointers(dadNode, newRight, where);
        break;
    case 3:
        vector[0] = strdup(childNode->C1);
        vector[1] = strdup(childNode->C2);
        vector[2] = strdup(aux);
        vector[3] = strdup(childNode->C3);

        shiftPointers(dadNode, newRight, where);
        break;
    case 4:
        vector[0] = strdup(childNode->C1);
        vector[1] = strdup(childNode->C2);
        vector[2] = strdup(childNode->C3);
        vector[3] = strdup(aux);

        int shifPlace = whereToInsert(dadNode, aux);
        shiftPointers(dadNode, newRight, shifPlace);
        break;

    default:
        printf("erro 75\n");
        break;
    }

    childNode->C1 = vector[0];
    shiftRightImplement(dadNode, vector[1], whereToInsert(dadNode, aux));
    newRight->C1 = vector[2];
    newRight->C2 = vector[3];
    childNode->C2 = NULL;
    childNode->C3 = NULL;
}

int isRoot(BTreeNode *root, int highestTree)
{
    int local_height = heightTree(root);
    printf("Split em:\t");
    if (local_height == 1 && local_height < highestTree)
    {
        printf("É nó folha\n");
        return 0;
    }
    else if (highestTree == local_height)
    {
        printf("é Raiz\n");
        return 1;
    }

    else
    {
        printf("é nó intermediario\n");
        return 0;
    }
}

/* Testa e insere o dado dentro do arquivo de index */
int insertIndexString(BTreeNode **root, char *aux, int *highestTree)
{
    if (*highestTree < heightTree((*root)))
        *highestTree = heightTree((*root));

    if ((*root)->C1 == NULL)
    {
        insertInPlace(*root, aux, 1);
    }
    else if (stringHigherThen(aux, (*root)->C1, 0) == 0 ||
             stringHigherThen(aux, (*root)->C2, 0) == 0 ||
             stringHigherThen(aux, (*root)->C3, 0) == 0)
    {
        printf("aux: %s não armazenado, eh igual um dos indices\n", aux);
    }

    else if (stringHigherThen(aux, (*root)->C1, 0) == 1)
    {
        if (isNode((*root)->P1))
        {
            if (insertIndexString(&(*root)->P1, aux, highestTree))
            {
                if (isAvailable((*root)->C3) == 1)
                {
                    splitNode((*root), (*root)->P1, initNode(), aux);
                    return 0;
                }
                else
                    return 1;
            }
            return 0;
        }
        else
        {
            if (isAvailable((*root)->C3))
            {
                shiftRightImplement(*root, aux, 1);
                return 0; //  Inserido corretamente sem precisar de promover
            }
            else if (isRoot((*root), (*highestTree)))
            {
                BTreeNode *newRoot = initNode();
                BTreeNode *newRight = initNode();
                splitNode(newRoot, *root, newRight, aux);
                newRoot->P1 = *root;
                newRoot->P2 = newRight;
                *root = newRoot;
            }
            return 1; // para caso tenha que dar split
        }
    }

    else if (stringHigherThen(aux, (*root)->C1, 0) == 2 && stringHigherThen(aux, (*root)->C2, 0) == 1)
    {
        if (isNode((*root)->P2))
        {
            if (insertIndexString(&(*root)->P2, aux, highestTree))
            {
                if (isAvailable((*root)->C3) == 1)
                {
                    splitNode((*root), (*root)->P2, initNode(), aux);
                    return 0;
                }
                else
                    return 1;
            }
            return 0;
        }
        else
        {
            if (isAvailable((*root)->C3))
            {
                shiftRightImplement(*root, aux, 2);
                return 0; //  Inserido corretamente sem precisar de promover
            }
            else if (isRoot((*root), (*highestTree)))
            {
                BTreeNode *newRoot = initNode();
                BTreeNode *newRight = initNode();
                splitNode(newRoot, *root, newRight, aux);
                newRoot->P1 = *root;
                newRoot->P2 = newRight;
                *root = newRoot;
            }
            return 1; // para caso tenha que dar split
        }
    }

    else if (stringHigherThen(aux, (*root)->C2, 0) == 2 && stringHigherThen(aux, (*root)->C3, 0) == 1)
    {
        if (isNode((*root)->P3))
        {
            if (insertIndexString(&(*root)->P3, aux, highestTree))
            {
                if (isAvailable((*root)->C3) == 1)
                {
                    splitNode((*root), (*root)->P3, initNode(), aux);
                    return 0;
                }
                else
                    return 1;
            }
            return 0;
        }
        else
        {
            if (isAvailable((*root)->C3))
            {
                shiftRightImplement(*root, aux, 3);
                return 0; //  Inserido corretamente sem precisar de promover
            }
            else if (isRoot((*root), (*highestTree)))
            {
                BTreeNode *newRoot = initNode();
                BTreeNode *newRight = initNode();
                splitNode(newRoot, *root, newRight, aux);
                newRoot->P1 = *root;
                newRoot->P2 = newRight;
                *root = newRoot;
            }
            return 1; // para caso tenha que dar split
        }
    }

    else if (stringHigherThen(aux, (*root)->C3, 0) == 2)
    {
        if (isNode((*root)->P4))
        {
            if (insertIndexString(&(*root)->P4, aux, highestTree))
            {
                if (isAvailable((*root)->C3) == 1)
                {
                    splitNode((*root), (*root)->P4, initNode(), aux);
                    return 0;
                }
                else
                {
                    if (isRoot((*root), (*highestTree)))
                    {
                        BTreeNode *newRoot = initNode();
                        BTreeNode *newRight = initNode();
                        char **vector;
                        int where = whereToInsert((*root)->P4, aux);

                        vector[0] = strdup();

                        newRoot->P1 = *root;
                        newRoot->P2 = newRight;
                        *root = newRoot;
                    }
                    else
                    {
                        splitNode((*root), (*root)->P4, initNode(), aux);
                    }
                    return 1;
                }
            }
            return 0;
        }
        else
        {
            // vai direto pro teste da raíz, pois nunca terá como olocar, já está em C3 oc
            if (isRoot((*root), (*highestTree)))
            {
                BTreeNode *newRoot = initNode();
                BTreeNode *newRight = initNode();
                splitNode(newRoot, *root, newRight, aux);
                newRoot->P1 = *root;
                newRoot->P2 = newRight;
                *root = newRoot;
            }
            return 1; // para caso tenha que dar split
        }
    }
}

void insertIndex(BTreeNode *root, Dados *dados)
{
    int stringConcatMaxSize = strlen(dados->nomeTecnologiaDestino.string) + strlen(dados->nomeTecnologiaOrigem.string) + 1; //  Para se concatenar, achas-se o tamanho total da string concatenada
    char aux[stringConcatMaxSize];                                                                                          //  Cria um auxiliar para guardar tal string concatenada

    strcpy(aux, dados->nomeTecnologiaOrigem.string);  // Copia origem na aux
    strcat(aux, dados->nomeTecnologiaDestino.string); // Concatena com destino
    // insertAux(root,aux);
}
