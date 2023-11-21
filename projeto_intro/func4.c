#include "structs.h"
#include "func4.h"
#include "func3.h"

static void pula_RRN(FILE *bin, int RRN)
{

    fseek(bin, 13, SEEK_SET);                 //  Aponta para o primeiro registro
    Dados dados_provisorio;                   //  Inicializa o dado que será só para correr acima dos outros até o RRN desejado
    fseek(bin, TAM_REGISTRO * RRN, SEEK_CUR); //  Aponta para o primeiro registro
}

void functionality_4(const char binArchiveName[], const int RRN)
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

    pula_RRN(bin, RRN); //  Função base para pular até o RRN desejado

    // le tudo do registro desejado a seguir
    (fread(&dados.removido, sizeof(char), 1, bin));

    if (dados.removido == '0') //  Análogo a função já implmentada
    {
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
    }
    else
    {
        printf("Registro inexistente.\n");
        fclose(bin);
        return;
    }

    /*
     *   Caso o programa ainda não tenha encontrado e continue procurando, eventualmente chegará ao fim do arquivo
     *   Se isso acontecer irá encontrar apenas valores apontantes para NULL. caso isso aconteça, o seguinte teste termina o programa
     */

    printa_registro(&dados);                 //  Utiliza a função já previamente criada na funcionalidade 3 para printar n tela o devido registro
    free(dados.nomeTecnologiaOrigem.string); //  Libera as strings variaveis
    free(dados.nomeTecnologiaDestino.string);

    fclose(bin); //  Fecha o arquivo
    return;
}