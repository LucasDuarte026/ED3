#include "structsBTree.h"
#include "B-Tree.h"
#include "func3.h"

void insertIndex(FILE *bin_index, Dados *dados, int *highestTree, int *nodeRRN, int referenceRRN) // Testa e insere o dado dentro do arquivo de index
{
    int stringConcatMaxSize = strlen(dados->nomeTecnologiaDestino.string) + strlen(dados->nomeTecnologiaOrigem.string) + 1; //  Para se concatenar, achas-se o tamanho total da string concatenada
    char aux[stringConcatMaxSize];                                                                                          //  Cria um auxiliar para guardar tal string concatenada
    strcpy(aux, dados->nomeTecnologiaOrigem.string);                                                                        // Copia origem na aux
    strcat(aux, dados->nomeTecnologiaDestino.string);
    // printf("%s\n", aux); // Concatena com destino
    insertIndexString(bin_index, aux, highestTree, nodeRRN, referenceRRN);
}

Dados *getRegister(FILE *bin, Dados *dados) // Lê o registro completo e armazena em dados
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
/*
void updateTreeValues(BTreeNode **root)
{
    BTreeNode *ptr_root = (*root);

    ptr_root->alturaNo = heightTree(ptr_root);
    ptr_root->nroChavesNo = keysQuant(ptr_root);
    // printf("altura: |%2d| e quant key: |%d|\n", ptr_root->data.alturaNo, ptr_root->data.nroChavesNo);
    // Repassar a funcção recursiva aos filhos
    if (ptr_root->P1)
    {
        updateTreeValues(&ptr_root->P1);
    }
    if (ptr_root->P2)
    {
        updateTreeValues(&ptr_root->P2);
    }
    if (ptr_root->P3)
    {
        updateTreeValues(&ptr_root->P3);
    }
    if (ptr_root->P4)
    {
        updateTreeValues(&ptr_root->P4);
    }
}
*/

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

void initHeader(FILE *bin_index)
{
    Header header;
    header.status = '0';
    header.rootNode = 0;
    header.RRNnextNode = 0;
    fwrite(&header.status, sizeof(char), 1, bin_index);
    fwrite(&header.rootNode, sizeof(int), 1, bin_index);
    fwrite(&header.RRNnextNode, sizeof(int), 1, bin_index);
    for (int i = 0; i < 196; i++)
    {
        header.garbage[i] = '$';
        fwrite(&header.garbage[i], sizeof(char), 1, bin_index); // pular os 196 lixos
    }
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

    initHeader(bin_index);
    // insertFirstRoot(bin_index);

    Dados dados;
    int encontrado = 0;                  // para testar registro inexistente
    int highestTree = 0;                 // Valor da atual altura maxima da arvore
    int nodeRRN = 0;                     // Contador para os nós inseridos
    int referenceRRN = 0;                // Contador para os nós inseridos
    fseek(bin, TAM_CABECALHO, SEEK_SET); //  Para pular o cabeçalho

    // le tudo do registro desejado a seguir
    while (fread(&dados.removido, sizeof(char), 1, bin))
    {
        if (dados.removido == '0')
        {

            dados = *getRegister(bin, &dados);
            // printa_registro(&dados); //  Utiliza a função já previamente criada na funcionalidade 3 para printar n tela o devido registro
            insertIndex(bin_index, &dados, &highestTree, &nodeRRN, referenceRRN);
            // updateTreeValues(&root);
            // printf("\n");
            // treePrint(&root);
            // printf("\n");
            // writeIndexFile();

            free(dados.nomeTecnologiaOrigem.string); //  Libera as strings variaveis
            free(dados.nomeTecnologiaDestino.string);
            encontrado = 1;
        }
        else if (dados.removido == '1')
        {
            fseek(bin, TAM_REGISTRO - 1, SEEK_CUR); // Pula o registro removido            }
        }
        if(encontrado){
            return;
        }
    }
    if (!encontrado)
    { // registro inexistente
        printf("Falha no processamento do arquivo.\n");
    }
    // treePrint(&root);

    fclose(bin);
    fclose(bin_index);
}
