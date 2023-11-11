#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define RECORD_SIZE 205
#define GARBAGE "$"
#define ORDER 4
#define MAX_KEYS (ORDER - 1)
#define MIN_KEYS (ORDER / 2 - 1)

typedef struct{
    char status;
    int seedNode;
    int RRNnextNode;
    char garbage[196];
} Header;
    
typedef struct BTreeNode{
    int n;
    char keys[MAX_KEYS][50];
    struct BTreeNode *children[ORDER];
    int leaf;
} BTreeNode;

typedef struct structs{
    int nmbKeysNode;
    int Nodeheight;
    int NodeRRN;
} Data;

#endif