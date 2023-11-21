#ifndef B_TREE
#define B_TREE
#include "structsBTree.h"

BTreeNode *initNode();
int heightTree(BTreeNode *node);                                             //  Retorna a altura daquele nó
void insertIndex(BTreeNode **root, Dados *dados, int *highestTree);          // Testa e insere o dado dentro do arquivo de index
BTreeNode *insertIndexString(BTreeNode **root, char *aux, int *highestTree); // Testa e insere o dado dentro do arquivo de index

#endif