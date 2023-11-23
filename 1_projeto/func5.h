#ifndef FUNC5
#define FUNC5
#include "structs.h"
#include "structsBTree.h"

void insertIndex(FILE *bin_index, Dados *dados, int *highestTree, int *RRN); // Testa e insere o dado dentro do arquivo de index
void functionality_5(char *binArchiveName, char *outArchiveName);
Dados *getRegister(FILE *bin, Dados *dados); // Lê o registro completo e armazena em dados

#endif