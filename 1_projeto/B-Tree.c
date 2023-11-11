#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "structs.h"


BTreeNode *createNode(int leaf) {
    BTreeNode *newNode = (BTreeNode *)malloc(sizeof(BTreeNode));
    newNode->n = 0;
    newNode->leaf = leaf;

    for (int i = 0; i < ORDER; i++) {
        newNode->children[i] = NULL;
    }

    return newNode;
}

void splitChild(BTreeNode *parent, int i, BTreeNode *child) {
}

void insertNonFull(BTreeNode *node, char *key) {

}

void insert(BTreeNode **root, char *key) {
    BTreeNode *r = *root;
    if (r->n == MAX_KEYS) {
        BTreeNode *newNode = createNode(0);
        *root = newNode;
        newNode->children[0] = r;
        splitChild(newNode, 0, r);
        insertNonFull(newNode, key);
    } else {
        insertNonFull(r, key);
    }
}