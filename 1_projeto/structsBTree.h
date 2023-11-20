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
#define FIELD_SIZE 55

//   -- -- -- --  -- -- -- -- -- -- -- --   -- -- -- --  -- -- -- --
// Anteriores  - previamente usados anteriormente no projeto introdutório

#define LIXO "$"
#define TAM_CABECALHO 13
#define TAM_REGISTRO_FIXO 21
#define TAM_REGISTRO 76
#define MAX_STRING_LENGTH 100
#define REMOVIDO '1'
#define NAO_REMOVIDO '0'
#define NULO "NULO"

typedef struct
{ // 13 bytes em status e 4 bytes no resto
    char status;
    int proxRRN;
    int nroTecnologia;
    int nroParesTecnologia;
} Cabecalho;

typedef struct
{
    int tamanho;
    char *string;

} StringVariavel;

typedef struct
{
    char removido;                        // 1 byte
    int grupo;                            // 4 bytes
    int popularidade;                     // 4 bytes
    int peso;                             // 4 bytes
    StringVariavel nomeTecnologiaOrigem;  // tamanho variável
    StringVariavel nomeTecnologiaDestino; // tamanho variável

} Dados;
//   -- -- -- --  -- -- -- -- -- -- -- --   -- -- -- --  -- -- -- --

typedef struct
{
    char status;
    int seedNode;
    int RRNnextNode;
    char garbage[196];
} Header;

typedef struct structs
{
    int nroChavesNo;
    int alturaNo;
    int RRNdoNo;
} DataTree;

/*  Ordem
 * P1 -> Ponteiro para antes do c1
 * C1
 * PR1
 * P2 -> Ponteiro para antes do c2 e depois de c1
 * C2
 * PR2
 * P3 -> Ponteiro para antes do c3 e depois de c2
 * C3
 * PR3
 * P4 -> Ponteiro para depois do c3
 */
typedef struct BTreeNode
{
    DataTree data;

    struct BTreeNode *P1;
    char *C1;
    int PR1;
    struct BTreeNode *P2;
    char *C2;
    int PR2;
    struct BTreeNode *P3;
    char *C3;
    int PR3;
    struct BTreeNode *P4;


} BTreeNode;



#endif