#include "structs.h"
#include "func2.h"
#include "func3.h"
// pois a printa_registro() está na func3

//  Declaração da functionality_2
void functionality_2(const char binArchiveName[])
{
    /*
     *   Inicialização bem semelhante as outras funcionalidades
     *   Abre o arquivo binário, lê o cabeçario para posicionar devidamente a cabeça leitora para o primeiro RRN
     */
    FILE *bin = fopen(binArchiveName, "rb");
    if (bin == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
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
    Dados dados;

    fseek(bin, TAM_CABECALHO, SEEK_SET); //  Para pular o cabeçalho

    int encontrado = 0; // para testar registro inexistente
    int counter = 0;
    // le tudo do registro desejado a seguir
    while (fread(&dados.removido, sizeof(char), 1, bin))
    {

        if (dados.removido == '0')
        {
            encontrado = 1;
            fread(&dados.grupo, sizeof(int), 1, bin);
            fread(&dados.popularidade, sizeof(int), 1, bin);
            fread(&dados.peso, sizeof(int), 1, bin);

            fread(&dados.nomeTecnologiaOrigem.tamanho, sizeof(int), 1, bin);
            dados.nomeTecnologiaOrigem.string = (char *)malloc(dados.nomeTecnologiaOrigem.tamanho + 1);
            fread(dados.nomeTecnologiaOrigem.string, sizeof(char), dados.nomeTecnologiaOrigem.tamanho, bin);
            dados.nomeTecnologiaOrigem.string[dados.nomeTecnologiaOrigem.tamanho] = '\0';

            fread(&dados.nomeTecnologiaDestino.tamanho, sizeof(int), 1, bin);
            dados.nomeTecnologiaDestino.string = (char *)malloc(dados.nomeTecnologiaDestino.tamanho + 1);
            fread(dados.nomeTecnologiaDestino.string, sizeof(char), dados.nomeTecnologiaDestino.tamanho, bin);
            dados.nomeTecnologiaDestino.string[dados.nomeTecnologiaDestino.tamanho] = '\0';

            printa_registro(&dados); //  Utiliza a função já previamente criada na funcionalidade 3 para printar n tela o devido registro

            free(dados.nomeTecnologiaOrigem.string); //  Libera as strings variaveis
            free(dados.nomeTecnologiaDestino.string);
        }
        else if (dados.removido == '1')
        {
            fseek(bin, TAM_REGISTRO - 1, SEEK_CUR); //  Pula para o próximo registro (-1 pois já le o '1')
        }
        counter++;
    }
    if (!encontrado)
    { // registro inexistente
        printf("Registro inexistente.\n");
    }
    fclose(bin);
}
