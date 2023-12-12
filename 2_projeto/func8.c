#include "func8.h"
#include "structs.h"

Dados *getRegister(FILE *bin, Dados *dados)
{
    fread(&dados->grupo, sizeof(int), 1, bin);
    fread(&dados->popularidade, sizeof(int), 1, bin);
    fread(&dados->peso, sizeof(int), 1, bin);

    fread(&dados->nomeTecnologiaOrigem.tamanho, sizeof(int), 1, bin);
    dados->nomeTecnologiaOrigem.string = (char *)malloc(dados->nomeTecnologiaOrigem.tamanho + 1);
    fread(dados->nomeTecnologiaOrigem.string, sizeof(char), dados->nomeTecnologiaOrigem.tamanho, bin);
    dados->nomeTecnologiaOrigem.string[dados->nomeTecnologiaOrigem.tamanho] = '\0';

    fread(&dados->nomeTecnologiaDestino.tamanho, sizeof(int), 1, bin);
    dados->nomeTecnologiaDestino.string = (char *)malloc(dados->nomeTecnologiaDestino.tamanho + 1);
    fread(dados->nomeTecnologiaDestino.string, sizeof(char), dados->nomeTecnologiaDestino.tamanho, bin);
    dados->nomeTecnologiaDestino.string[dados->nomeTecnologiaDestino.tamanho] = '\0';

    return dados;
}

// Funcionalidade principal chamada pela main para fazer a cração de uma Árvore de índice com base no bance de dados binário de tecnologias
void functionality_8(char *binArchiveName)
{

    /*
     *   Inicialização bem semelhante as outras funcionalidades
     *   Abre o arquivo binário, lê o cabeçalho para posicionar devidamente a cabeça leitora para o primeiro RRN
     */
    FILE *bin = fopen(binArchiveName, "rb");        // Abre o arquivo bin de registro

    if (bin == NULL    )
    {
        printf("Falha no processamento do arquivo.\n");
        return; // Caso o bin não seja encontrado ou o bin_index não criado
    }

    //  inicializa o cabeçalho
    Cabecalho cabecalho;
    fread(&cabecalho.status, sizeof(char), 1, bin);
    fread(&cabecalho.proxRRN, sizeof(int), 1, bin);
    fread(&cabecalho.nroTecnologia, sizeof(int), 1, bin);
    fread(&cabecalho.nroParesTecnologia, sizeof(int), 1, bin);

    if (cabecalho.status == '0')
    {
        printf("Falha no processamento do arquivo.\n");
        fclose(bin);
        return;
    }

    //  inicializa o cabeçalho do arquivo de indice

    Dados dados;
    int encontrado = 0;                  // para testar registro inexistente
    int highestTree = 0;                 // Valor da atual altura maxima da arvore
    int referenceRRN = 0;                // Contador para os nós inseridos
    fseek(bin, TAM_CABECALHO, SEEK_SET); //  Para pular o cabeçalho
    int counter = 0;
    // le tudo do registro desejado a seguir
    while (fread(&dados.removido, sizeof(char), 1, bin))
    {

        if (dados.removido == '0')
        {
            counter++;
            dados = *getRegister(bin, &dados);
            // printf(" -> %s%s, %d\n", dados.nomeTecnologiaOrigem.string, dados.nomeTecnologiaDestino.string, referenceRRN); // Concatena com destino

            if (dados.nomeTecnologiaOrigem.tamanho != 0 && dados.nomeTecnologiaDestino.tamanho != 0)
            { // printa_registro(&dados); //  Utiliza a função já previamente criada na funcionalidade 3 para printar n tela o devido registro

                // Usado para printar no terminal toda a arvore. Foi anteriomente usada para debug
                encontrado = 1; //  Caso encontre, pelo menos uma vez
            }
            else
            {
                referenceRRN++; //  Contador do registros de leitura do arquivo binário
                // função acessora para debig
                // printf("dados:\t|%d||%s||%d||%s||", dados.nomeTecnologiaOrigem.tamanho, dados.nomeTecnologiaOrigem.string, dados.nomeTecnologiaDestino.tamanho, dados.nomeTecnologiaDestino.string);
            }
            free(dados.nomeTecnologiaOrigem.string); //  Libera as strings variaveis
            free(dados.nomeTecnologiaDestino.string);
        }
        else if (dados.removido == '1')
        {
            referenceRRN++; //  Contador do registros de leitura do arquivo binário

            fseek(bin, TAM_REGISTRO - 1, SEEK_CUR); // Pula o registro removido            }
        }
    }
    if (!encontrado)
    { // registro inexistente
        printf("Falha no processamento do arquivo.\n");
    }
    fclose(bin);
    fclose(bin_index);
    binarioNaTela(outArchiveName);
}
