#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_NAME_SIZE 55
#define MAX_ORIGINS_TECNOLOGIES 200
#define EMPTY_CONTROL "$$$$$$"
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
// -- -- -- -- -- -- -- -- -- -- -- -- Funcionalidades antigas - -- -- -- -- -- -- -- -- -- --
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
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

typedef struct Vertex
{
    char tecName[MAX_NAME_SIZE];
    int entrance;
    int degree;
    int output;
    int group;
    int weight;
    struct Vertex *nextVertex;
} Vertex;

#endif