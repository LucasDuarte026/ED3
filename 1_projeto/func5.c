#include "structsBTree.h"
#include "B-Tree.h"
#include "func3.h"
#include "func1.h"

// Testa, obtem aux e insere o dado dentro do arquivo de index com o referenceRRN
void insertIndex(FILE *bin_index, Dados *dados, int *highestTree, int *nodeIndexRRN, int referenceRRN)
{
    int stringConcatMaxSize = strlen(dados->nomeTecnologiaDestino.string) + strlen(dados->nomeTecnologiaOrigem.string) + 1; //  Para se concatenar, achas-se o tamanho total da string concatenada
    char aux[stringConcatMaxSize];                                                                                          //  Cria um auxiliar para guardar tal string concatenada
    strcpy(aux, dados->nomeTecnologiaOrigem.string);                                                                        // Copia origem na aux
    strcat(aux, dados->nomeTecnologiaDestino.string);
    // printf(" -> %s, %d\n", aux, referenceRRN); // Concatena com destino
    fflush(stdout);

    BTreeNode *actual_root = initNode();
    actual_root = getRoot(bin_index, actual_root);
    if (actual_root->RRNdoNo == -1)
        actual_root->RRNdoNo = 0;
    actual_root = insertIndexString(bin_index, actual_root->RRNdoNo, aux, highestTree, nodeIndexRRN, referenceRRN);
}

// Lê o registro completo e armazena em dados
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

// Insere a primeira raiz
void insertFirstRoot(FILE *bin_index)
{
    char aux = '$';
    BTreeNode *newRoot = initNode();
    newRoot->alturaNo = -1;
    newRoot->RRNdoNo = -1;
    newRoot->nroChavesNo = -1;
    fwrite(&newRoot->nroChavesNo, sizeof(int), 1, bin_index);
    fwrite(&newRoot->alturaNo, sizeof(int), 1, bin_index);
    fwrite(&newRoot->RRNdoNo, sizeof(int), 1, bin_index);
    fwrite(&newRoot->P1, sizeof(int), 1, bin_index);
    for (int i = 0; i < 55; i++)
    {
        fwrite(&aux, sizeof(char), 1, bin_index);
    }
    fwrite(&newRoot->PR1, sizeof(int), 1, bin_index);
    fwrite(&newRoot->P2, sizeof(int), 1, bin_index);

    for (int i = 0; i < 55; i++)
    {
        fwrite(&aux, sizeof(char), 1, bin_index);
    }
    fwrite(&newRoot->PR2, sizeof(int), 1, bin_index);
    fwrite(&newRoot->P3, sizeof(int), 1, bin_index);

    for (int i = 0; i < 55; i++)
    {
        fwrite(&aux, sizeof(char), 1, bin_index);
    }
    fwrite(&newRoot->PR3, sizeof(int), 1, bin_index);
    fwrite(&newRoot->P4, sizeof(int), 1, bin_index);
}

// Inicializa o header do arquivo de indice
void initHeader(FILE *bin_index)
{
    Header header;
    header.status = '0';
    header.rootNode = -1;
    header.RRNnextNode = -1;
    fwrite(&header.status, sizeof(char), 1, bin_index);
    fwrite(&header.rootNode, sizeof(int), 1, bin_index);
    fwrite(&header.RRNnextNode, sizeof(int), 1, bin_index);
    for (int i = 0; i < 196; i++)
    {
        header.garbage[i] = '$';
        fwrite(&header.garbage[i], sizeof(char), 1, bin_index); // pular os 196 lixos
    }
}

// Funcionalidade principal chamada pela main para fazer a cração de uma Árvore de índice com base no bance de dados binário de tecnologias
void functionality_5(char *binArchiveName, char *outArchiveName)
{

    /*
     *   Inicialização bem semelhante as outras funcionalidades
     *   Abre o arquivo binário, lê o cabeçalho para posicionar devidamente a cabeça leitora para o primeiro RRN
     */
    FILE *bin = fopen(binArchiveName, "rb");        // Abre o arquivo bin de registro
    FILE *bin_index = fopen(outArchiveName, "wb+"); // criar o bin para colocar os index

    if (bin == NULL || bin_index == NULL)
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

    initHeader(bin_index);      // inicializa o cabeçalho
    insertFirstRoot(bin_index); // insere a primeira raiz no indice

    Dados dados;
    int encontrado = 0;                  // para testar registro inexistente
    int highestTree = 0;                 // Valor da atual altura maxima da arvore
    int nodeIndexRRN = 0;                // Contador para os nós inseridos
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
                insertIndex(bin_index, &dados, &highestTree, &nodeIndexRRN, referenceRRN);
                BTreeNode *root = initNode();
                root = getRoot(bin_index, root);
                // if (counter % 1000 == 0)
                // {
                //     printf("\nc:%d", counter);
                //     treePrint(bin_index, root->RRNdoNo);
                //     printf("\n");
                // }

                // Usado para printar no terminal toda a arvore. Foi anteriomente usada para debug
                referenceRRN++; //  Contador do registros de leitura do arquivo binário
                encontrado = 1; //  Caso encontre, pelo menos uma vez
                if (highestTree < heightTree(bin_index, root))
                    highestTree = heightTree(bin_index, root);
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
    // updateHeader(bin_index, '1', -1, &nodeIndexRRN);
    if (!encontrado)
    { // registro inexistente
        printf("Falha no processamento do arquivo.\n");
    }
    updateHeader(bin_index, '1', -1, &nodeIndexRRN);
    BTreeNode *root = initNode();
    root = getRoot(bin_index, root);

    // para degub para ver a arvore criada 
    // printf("\n");
    // treePrint(bin_index, root->RRNdoNo);
    // printf("\n");
    fclose(bin);
    fclose(bin_index);
    binarioNaTela(outArchiveName);
}
