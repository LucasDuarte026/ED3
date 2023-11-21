#ifndef B_TREE
#define B_TREE
#include "structsBTree.h"

BTreeNode *initNode();
int heightTree(BTreeNode *node);                                             //  Retorna a altura daquele nó
BTreeNode *insertIndexString(BTreeNode **root, char *aux, int *highestTree,int RRN); // Testa e insere o dado dentro do arquivo de index
int keysQuant(BTreeNode *node); //  Retorna a quantidade de chaves dentro do nó especificado

#endif