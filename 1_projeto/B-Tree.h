#ifndef B_TREE
#define B_TREE
#include "structsBTree.h"

BTreeNode *initNode();
// int heightTree(BTreeNode *node);                                                                        //  Retorna a altura daquele nó
BTreeNode *insertIndexString(FILE *bin_index, int node_inIndex, char *aux, int *highestTree, int *RRN, int referenceRRN); // Testa e insere o dado dentro do arquivo de index
int keysQuant(BTreeNode *node);                                                                                           //  Retorna a quantidade de chaves dentro do nó especificado
// void treePrint(BTreeNode **root);
BTreeNode *getRoot(FILE *bin_index, BTreeNode *root);
int whereToInsert(BTreeNode *node, char *aux);
BTreeNode *readIndexRegister(FILE *bin_index, BTreeNode *prt_root, int RRN);
void updateHeader(FILE *bin_index, char status, int rootNodeRRN, int *nodeRRN);
void treePrint(FILE *bin_index, int RRN);
int heightTree(FILE *bin_index, BTreeNode *node);

#endif