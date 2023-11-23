#include "structsBTree.h"
#include "B-Tree.h"
#include "structs.h"
#include "func5.h"
#include "func4.h"
#include "func3.h"
#include "func1.h"

BTreeNode *sonRepass(FILE *bin_index, BTreeNode *node, BTreeNode *sun, int place)
{
    int sunRRN;

    switch (place)
    {
    case 1:
        sunRRN = node->P1;
        break;
    case 2:
        sunRRN = node->P2;
        break;
    case 3:
        sunRRN = node->P3;
        break;
    case 4:
        sunRRN = node->P4;
        break;
    default:
        break;
    }
    sun = readIndexRegister(bin_index, sun, sunRRN);

    return sun;
}

int getRRN(BTreeNode *node, char *valorCampo)
{
    if (node->C1)
    {
        if (strcmp(valorCampo, node->C1) == 0)
            return node->PR1;
    }
    if (node->C2)
    {
        if (strcmp(valorCampo, node->C2) == 0)
            return node->PR2;
    }
    if (node->C3)
    {
        if (strcmp(valorCampo, node->C3) == 0)
            return node->PR3;
    }
    return -1;
}

int searchRRN(FILE *bin_index, BTreeNode *node, char *valorCampo)
{
    int seekRRN = -1;
    BTreeNode *sonNode = initNode();
    switch (whereToInsert(node, valorCampo))
    {
    case 1:
        if (node->P1 != -1)
        {
            sonNode = sonRepass(bin_index, node, sonNode, 1);
            seekRRN = searchRRN(bin_index, sonNode, valorCampo);
        }
        else
            seekRRN = getRRN(node, valorCampo);

        break;
    case 2:
        if (node->P2 != -1)
        {
            sonNode = sonRepass(bin_index, node, sonNode, 2);
            seekRRN = searchRRN(bin_index, sonNode, valorCampo);
        }
        else
            seekRRN = getRRN(node, valorCampo);

        break;
    case 3:
        if (node->P3 != -1)
        {
            sonNode = sonRepass(bin_index, node, sonNode, 3);
            seekRRN = searchRRN(bin_index, sonNode, valorCampo);
        }
        else
            seekRRN = getRRN(node, valorCampo);

        break;
    case 4:
        if (node->P4 != -1)
        {
            sonNode = sonRepass(bin_index, node, sonNode, 4);
            seekRRN = searchRRN(bin_index, sonNode, valorCampo);
        }
        else
            seekRRN = getRRN(node, valorCampo);

        break;
    default:
        seekRRN = getRRN(node, valorCampo);
        break;
    }
    return seekRRN;
}

Header chargeHeader(FILE *bin_index)
{
    Header header;
    fseek(bin_index, 0, SEEK_SET);
    fread(&header.status, sizeof(char), 1, bin_index);
    fread(&header.rootNode, sizeof(int), 1, bin_index);
    fread(&header.RRNnextNode, sizeof(int), 1, bin_index);
    fread(&header.garbage, sizeof(char), 196, bin_index); // pular os 196 lixos
    return header;
}

void functionality_6(char *binArchiveName, char *outArchiveName, int N)
{

    for (int i = 0; i < N; i++)
    {
        int mode = 0;
        char nomeCampo[MAX_STRING_LENGTH];  //  Buffer preparado para pegar a string necessária ao nome do campo de entrada
        char valorCampo[MAX_STRING_LENGTH]; //  Buffer preparado para pegar o valor necessário do campo de entrada
        scanf("%s", nomeCampo);             //  Recebe o nome do campo
        if (strcmp(nomeCampo, "nomeTecnologiaOrigem") == 0 || strcmp(nomeCampo, "nomeTecnologiaDestino") == 0)
        {
            scan_quote_string(valorCampo); //  Recebe o valor do campo entre aspas
            mode = 1;
        }
        else if (strcmp(nomeCampo, "nomeTecnologiaOrigemDestino") == 0)
        {
            scan_quote_string(valorCampo); //  Recebe o valor do campo entre aspas
            mode = 2;
        }
        else
        {
            mode = 1;
            scanf("%s", valorCampo); //  Recebe o nome do campo
        }

        int registros_encontrados = 0; //  inicializa a variável de encontrar
        if (mode == 1)                 // Busca igual a func3
        {
            Dados dados; //  Inicializa os dados que serão inseridos caso ache

            FILE *bin = fopen(binArchiveName, "rb"); // Abre o arquivo bin de registro

            if (bin == NULL)
            {
                printf("Falha no processamento do arquivo.\n");
                return; // Caso o bin não seja encontrado ou o bin_index não criado
            }
            fseek(bin, TAM_CABECALHO, SEEK_SET); //  Pula o cabeçalho até o primeiro RRN
            while (fread(&dados.removido, sizeof(char), 1, bin))
            {
                if (dados.removido == '0')
                {
                    dados = *getRegister(bin, &dados);
                    if (testa_campo(nomeCampo, valorCampo, &dados))
                    {
                        registros_encontrados++;
                    }

                    free(dados.nomeTecnologiaOrigem.string); //  Libera o campo variável como desejado pelo projeto
                    free(dados.nomeTecnologiaDestino.string);
                }
                else if (dados.removido == '1')
                {

                    fseek(bin, TAM_REGISTRO - 1, SEEK_CUR);
                }
            }
            if (registros_encontrados == 0)
            {
                printf("Registro inexistente.\n"); // CASO O PEDIDO NÃO ENCONTRE NADA
            }
            fclose(bin); //  Fecha o binário devidamente
        }
        else if (mode == 2) //  Busca com a arvore de busca
        {
            FILE *bin_index = fopen(outArchiveName, "rb"); // criar o bin para colocar os index

            if (bin_index == NULL)
            {
                printf("Falha no processamento do arquivo.\n");
                return; // Caso o bin não seja encontrado ou o bin_index não criado
            }
            Header header;
            header = chargeHeader(bin_index);
            if (header.status == '0')
            {
                printf("Falha no processamento do arquivo.\n");
                fclose(bin_index); //  Fecha o binário indice devidamente
                return;            // arquivo inconsciestente
            }
            BTreeNode *root = getRoot(bin_index);
            int registerRRN = searchRRN(bin_index, root, valorCampo);
            if (registerRRN == -1)
                printf("Registro inexistente.\n");
            else
                functionality_4(binArchiveName, registerRRN);
            fclose(bin_index); //  Fecha o binário indice devidamente
        }
    }
}
