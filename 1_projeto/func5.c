#include "structsBTree.h"
#include "B-Tree.h"
#include "func3.h"

void insertIndex(BTreeNode **root, Dados *dados, int *highestTree, int RRN)
{
    int stringConcatMaxSize = strlen(dados->nomeTecnologiaDestino.string) + strlen(dados->nomeTecnologiaOrigem.string) + 1; //  Para se concatenar, achas-se o tamanho total da string concatenada
    char aux[stringConcatMaxSize];                                                                                          //  Cria um auxiliar para guardar tal string concatenada
    strcpy(aux, dados->nomeTecnologiaOrigem.string);                                                                        // Copia origem na aux
    strcat(aux, dados->nomeTecnologiaDestino.string);                                                                       // Concatena com destino
    printf("%s\n", aux);
    insertIndexString(root, aux, highestTree, RRN);
}

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

void updateTreeValues(BTreeNode **root)
{
    BTreeNode *ptr_root = (*root);

    ptr_root->data.alturaNo = heightTree(ptr_root);
    ptr_root->data.nroChavesNo = keysQuant(ptr_root);
    printf("altura: |%2d| e quant key: |%d|\n", ptr_root->data.alturaNo, ptr_root->data.nroChavesNo);
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
    int RRN = 1;
    fseek(bin, TAM_CABECALHO, SEEK_SET); //  Para pular o cabeçalho

    // le tudo do registro desejado a seguir
    while (!feof(bin))
    {
        fread(&dados.removido, sizeof(char), 1, bin);
        if (dados.removido == '0')
        {

            dados = *lerRegistro(bin, &dados);
            printa_registro(&dados); //  Utiliza a função já previamente criada na funcionalidade 3 para printar n tela o devido registro

            insertIndex(&root, &dados, &highestTree,RRN);
            free(dados.nomeTecnologiaOrigem.string); //  Libera as strings variaveis
            free(dados.nomeTecnologiaDestino.string);
            encontrado = 1;
            RRN++;
        }
        else if (dados.removido == '1')
        {
            RRN++;
            fseek(bin, TAM_REGISTRO, SEEK_CUR); // Pula o registro removido            }
        }
    }
    if (!encontrado)
    { // registro inexistente
        printf("Falha no processamento do arquivo.\n");
    }

    updateTreeValues(&root);

    fclose(bin);
    fclose(bin_index);
}
