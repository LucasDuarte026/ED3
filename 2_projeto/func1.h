#ifndef FUNC1
#define FUNC1

// privatevoid readline(char *string);
// void binarioNaTela(const char *nomeArquivoBinario);
// FILE *init_bin(FILE *bin);
// void Atualizar_Cabecalho(FILE *bin, int nroTecnologia, int nroParesTecnologia, int prox);
// void Escrever_Dados(FILE *bin, Dados dados);
// Dados LerRegistroCSV(FILE *csv);
// char **testa_unico(int *prt_quant_tec, Dados dado, char **tecnologies);
// char **testa_unico(int *prt_quant_tec, Dados dado, char **tecnologies);
// char **testa_par(int *prt_quant_tec_par, Dados dado, char **pares);

// *   As funções acima não precisam ser declaradas pois são locais e privadas, não sendo elas
// *   necessárias em declaração

void binarioNaTela(const char *nomeArquivoBinario);
void scan_quote_string(char *str);
short int Functionality_1(const char csvArchiveName[], const char binArchiveName[]);

#endif