#ifndef B_TREE
#define B_TREE
#include "structsBTree.h"

BTreeNode *createRoot(BTreeNode *raiz);
BTreeNode *createNode();
void insert(BTreeNode *raiz, Dados *registro);

#endif