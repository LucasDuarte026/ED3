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

typedef struct BTreeNode
{
    DataTree data;

    DataTree *P1;
    char *C1[FIELD_SIZE];
    int PR1;
    DataTree *P2;
    char *C2[FIELD_SIZE];
    int PR2;
    DataTree *P3;
    char *C3[FIELD_SIZE];
    int PR3;
    DataTree *P4;

    /*  Ordem
     * P1
     * C1
     * PR1
     * P2
     * C2
     * PR2
     * P3
     * C3
     * PR3
     * P4
     */

} BTreeNode;

#endif