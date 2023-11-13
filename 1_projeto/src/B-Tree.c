#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "structsBTree.h"


BTreeNode *createNode() {
    BTreeNode *newNode = (BTreeNode *)malloc(sizeof(BTreeNode));

    return newNode;
}

void splitChild(BTreeNode *parent, int i, BTreeNode *child) {
}

void insertNonFull(BTreeNode *node, char *key) {

}

void insert(BTreeNode **root, char *key) {
    // BTreeNode *r = *root;
    // if (r->n == MAX_KEYS) {
    //     BTreeNode *newNode = createNode(0);
    //     *root = newNode;
    //     newNode->children[0] = r;
    //     splitChild(newNode, 0, r);
    //     insertNonFull(newNode, key);
    // } else {
    //     insertNonFull(r, key);
    // }
}