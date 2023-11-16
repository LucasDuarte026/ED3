#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structsBTree.h"
#include "B-Tree.h"

/*  Inicializa a raiz   */
BTreeNode *initNode(BTreeNode *raiz)
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

    int height = 1;
    if (root == NULL)
    {
        return 1;
    }
    else
    {
        if (root->P1 != NULL)
        {
            height += heightTree(root->P1);
        }
        if (root->P2 != NULL)
        {
            height += heightTree(root->P2);
        }
        if (root->P3 != NULL)
        {
            height += heightTree(root->P3);
        }
        if (root->P4 != NULL)
        {
            height += heightTree(root->P4);
        }
    }
    return height;
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
static BTreeNode *splitNode(BTreeNode *root, char **auxOrdered)
{
    BTreeNode *newRight;
    newRight = initNode(newRight);

    newRight->C1 = strdup(auxOrdered[2]);
    newRight->C2 = strdup(auxOrdered[3]);

    root->C1 = strdup(auxOrdered[0]);
    root->C2 = NULL;
    root->C3 = NULL;
    // printf("ainda não pronto\n");
    return newRight;
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

static char **shiftRightImplement(BTreeNode *node, char *aux, int place, char **elements)
{
    switch (place)
    {
    case 1:
        if (node->C3 != NULL)
        {
            elements[3] = strdup(node->C3);
            elements[2] = node->C2;
            elements[1] = node->C1;
            elements[0] = strdup(aux);

            node->C3 = NULL;
            node->C2 = NULL;

            return elements;
        }
        node->C3 = node->C2;
        node->C2 = node->C1;
        node->C1 = strdup(aux);
        break;
    case 2:
        if (node->C3 != NULL)
        {
            elements[3] = node->C3;
            elements[2] = node->C2;
            elements[1] = strdup(aux);
            elements[0] = node->C1;

            node->C3 = NULL;
            node->C2 = NULL;

            return elements;
        }
        node->C3 = node->C2;
        node->C2 = strdup(aux);
        break;
    case 3:
        if (node->C3 != NULL)
        {
            elements[3] = node->C3;
            elements[2] = strdup(aux);
            elements[1] = node->C2;
            elements[0] = node->C1;

            node->C3 = NULL;
            node->C2 = NULL;

            return elements;
        }
        node->C3 = strdup(aux);
        break;
    default:
        break;
    }
    return NULL;
}

/* Testa e insere o dado dentro do arquivo de index */
char **insertIndexString(BTreeNode *root, char *aux)
{
    //  Primeira inserção da arvore
    char **promoted = malloc(4 * sizeof(char *)); // alocar mem[oria para os elementos]
    if (root->C1 == NULL)
    {
        insertInPlace(root, aux, 1);
    }

    // Teste para caso seja igual a algo já inserido
    else if (stringHigherThen(aux, root->C1, 0) == 0 || stringHigherThen(aux, root->C2, 0) == 0 || stringHigherThen(aux, root->C3, 0) == 0)
    {
        printf("aux: %s não armazenado, eh igual um dos indices\n", aux);
    }

    else if (stringHigherThen(aux, root->C1, 0) == 1)
    {
        if (isNode(root->P1))
        {
            if (promoted = insertIndexString(root->P1, aux)) // Implementa
            {
                BTreeNode *newRoot;
                newRoot = initNode(newRoot);

                BTreeNode *newRight = splitNode(root, promoted);
                newRoot->P1 = root;
                newRoot->P2 = newRight;
                newRight->C1 = promoted[2];
                newRight->C2 = promoted[3];
                root = newRoot;
                return shiftRightImplement(root, aux, 1, promoted);
            }
        }
        else
        {
            if (promoted = shiftRightImplement(root, aux, 1, promoted)) // Simplemente coloca o valor no local
            {
                if (heightTree(root) == 1)
                {
                    BTreeNode *newRoot;
                    newRoot = initNode(newRoot);

                    BTreeNode *newRight = splitNode(root, promoted);
                    newRoot->P1 = root;
                    newRoot->P2 = newRight;
                    newRoot->C1 = promoted[1];
                    newRight->C1 = promoted[2];
                    newRight->C2 = promoted[3];
                    root = newRoot;
                }
                else{

                    BTreeNode *newRoot;
                    newRoot = initNode(newRoot);

                    BTreeNode *newRight = splitNode(root, promoted);
                    newRoot->P1 = root;
                    newRoot->P2 = newRight;
                    newRight->C1 = promoted[2];
                    newRight->C2 = promoted[3];
                    root = newRoot;
                    return promoted;
                }
            }
        }
    }

    else if (stringHigherThen(aux, root->C1, 0) == 2 && stringHigherThen(aux, root->C2, 0) == 1)
    {
        if (root->P2 == NULL) // ou seja, n tem filho
        {
            // troca c1 para c2 e c2 para c3 e armazena aux em c1;
            shiftRightImplement(root, aux, 2, promoted);
        }
    }

    else if (stringHigherThen(aux, root->C2, 0) == 2 && stringHigherThen(aux, root->C3, 0) == 1)
    {
        if (root->P3 == NULL) // ou seja, n tem filho
        {
            // troca c1 para c2 e c2 para c3 e armazena aux em c1;
            shiftRightImplement(root, aux, 3, promoted);
        }
    }

    else if (stringHigherThen(aux, root->C3, 0) == 2)
    {
        if (root->P4 == NULL) // ou seja, n tem filho
        {
            // troca c1 para c2 e c2 para c3 e armazena aux em c1;
            // shiftRightImplement(root, aux, 3,promoted);
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
