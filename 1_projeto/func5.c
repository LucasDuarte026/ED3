#include "structsBTree.h"
#include "B-Tree.h"
#include "func3.h"

static Dados *lerRegistro(FILE *bin, Dados *dados) // Lê o registro completo e armazena em dados
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

void functionality_5(char *binArchiveName, char *outArchiveName)
{
    /*
     *   Inicialização bem semelhante as outras funcionalidades
     *   Abre o arquivo binário, lê o cabeçario para posicionar devidamente a cabeça leitora para o primeiro RRN
     */
    FILE *bin = fopen(binArchiveName, "rb");        // Abre o arquivo bin de registro
    FILE *bin_index = fopen(outArchiveName, "wb+"); // criar o bin para colocar os index

    if (bin == NULL || bin_index == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return; // Caso o bin não seja encontrado ou o bin_index não criado
    }

    BTreeNode *root;
    root = initNode(root);

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
    Dados dados;
    int encontrado = 0;  // para testar registro inexistente
    int highestTree = 0; // para testar registro inexistente

    fseek(bin, TAM_CABECALHO, SEEK_SET); //  Para pular o cabeçalho

    // le tudo do registro desejado a seguir
    while (!feof(bin))
    {
        fread(&dados.removido, sizeof(char), 1, bin);
        if (dados.removido == '0')
        {

            dados = *lerRegistro(bin, &dados);
            // printa_registro(&dados); //  Utiliza a função já previamente criada na funcionalidade 3 para printar n tela o devido registro

            insertIndex(&root, &dados, &highestTree);
            free(dados.nomeTecnologiaOrigem.string); //  Libera as strings variaveis
            free(dados.nomeTecnologiaDestino.string);
            encontrado = 1;
        }
        else if (dados.removido == '1')
        {
            fseek(bin, TAM_REGISTRO, SEEK_CUR); // Pula o registro removido            }
        }
    }
    if (!encontrado)
    { // registro inexistente
        printf("Falha no processamento do arquivo.\n");
    }
    fclose(bin);
    fclose(bin_index);
}
