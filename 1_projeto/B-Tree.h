#ifndef B_TREE
#define B_TREE
#include "structsBTree.h"

int heightTree(BTreeNode *raiz);                   //  Retorna a altura daquele nó
void insertIndex(BTreeNode *root, Dados *dados);   // Testa e insere o dado dentro do arquivo de index
char **insertIndexString(BTreeNode *root, char *aux); // Testa e insere o dado dentro do arquivo de index

BTreeNode *initNode(BTreeNode *raiz);

#endif