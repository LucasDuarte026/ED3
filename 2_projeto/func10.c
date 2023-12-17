#include "structs.h"
#include "func10.h"
#include "func9.h"
#include "func8.h"
#include "func1.h"

int seekTechnology(Vertex *graph[], int graph_size, char *seekTec)
{
    int found = 0;
    for (int i = 0; i < graph_size; i++)
    {
        if (strcmp(seekTec, graph[i]->tecName) == 0)
        {
            found = 1;
            printf("%s: ", graph[i]->tecName);
            Vertex *currentRight;
            currentRight = graph[i];
            if (currentRight->nextVertex)
            {
                do
                {
                    currentRight = currentRight->nextVertex;
                    if (currentRight->nextVertex == NULL)
                    {
                        printf("%s", currentRight->tecName);
                    }
                    else
                        printf("%s, ", currentRight->tecName);

                } while (currentRight->nextVertex != NULL);
            }
            printf("\n\n");
            break;
        }
    }
    return found;
}

void showConnections(Vertex *graph[], int graph_size, int N)
{
    for (int i = 0; i < N; i++)
    {
        char seekTec[55];
        scan_quote_string(seekTec);
        // printf("%s\n", seekTec);

        int found = seekTechnology(graph, graph_size, seekTec);
        if (!found)
        { // registro inexistente
            printf("Registro inexistente.\n");
        }
    }
}

void functionality_10(char *binArchiveName, int *n_quant)
{

    /*
     *   Inicialização bem semelhante as outras funcionalidades
     *   Abre o arquivo binário, lê o cabeçalho para posicionar devidamente a cabeça leitora para o primeiro RRN
     */
    FILE *bin = fopen(binArchiveName, "rb"); // Abre o arquivo bin de registro

    if (bin == NULL)
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
    int referenceRRN = 0;                   // Contador para os nós inseridos
    Vertex *graph[MAX_ORIGINS_TECNOLOGIES]; // inicializa com o máximo possível de origens. Nunca ultrapassará 200
    char *auxInit = EMPTY_CONTROL;
    int graph_size = 0;
    int graph_size_T = 0;
    
    for (int i = 0; i < MAX_ORIGINS_TECNOLOGIES; i++)
    {
        graph[i] = initVertex();
        strcpy(graph[i]->tecName, auxInit);
    }

    fseek(bin, TAM_CABECALHO, SEEK_SET); //  Para pular o cabeçalho

    // le tudo do registro desejado a seguir
    while (fread(&dados.removido, sizeof(char), 1, bin))
    {

        if (dados.removido == '0')
        {
            dados = *getRegister(bin, &dados);
            // printf(" -> %s%s, %d\n", dados.nomeTecnologiaOrigem.string, dados.nomeTecnologiaDestino.string, referenceRRN); // Concatena com destino

            if (dados.nomeTecnologiaOrigem.tamanho != 0 && dados.nomeTecnologiaDestino.tamanho != 0)
            {
                // printa_registro(&dados); //  Utiliza a função já previamente criada na funcionalidade 3 para printar n tela o devido registro
                graph_size = updateGraph(graph, &dados, graph_size);
                referenceRRN++; //  Contador do registros de leitura do arquivo binário

                // Usado para printar no terminal toda a arvore. Foi anteriomente usada para debug
            }
            else
            {
                // printf("nulo: origem:|%s|\t\t\tDestino|%s|\n", dados.nomeTecnologiaOrigem.string, dados.nomeTecnologiaDestino.string);
                // referenceRRN++; //  Contador do registros de leitura do arquivo binário
                // função acessora para debig
                // printf("dados:\t|%d||%s||%d||%s||", dados.nomeTecnologiaOrigem.tamanho, dados.nomeTecnologiaOrigem.string, dados.nomeTecnologiaDestino.tamanho, dados.nomeTecnologiaDestino.string);
            }

            // printf("\n");
            // printGraph(graph, graph_size);
            // printf("\n");

            free(dados.nomeTecnologiaOrigem.string); //  Libera as strings variaveis
            free(dados.nomeTecnologiaDestino.string);
        }
        else if (dados.removido == '1')
        {
            referenceRRN++; //  Contador do registros de leitura do arquivo binário

            fseek(bin, TAM_REGISTRO - 1, SEEK_CUR); // Pula o registro removido            }
        }
    }

    heapSort(graph, graph_size);

    Vertex *graph_T[graph_size]; // inicializa com o máximo possível de origens. Nunca ultrapassará 200
    graph_size_T = transposeGraph(graph, graph_T, graph_size, graph_size_T);
    heapSort(graph_T, graph_size_T);
    countDegrees(graph_T, graph_size_T);
    // printFunc8(graph_T, graph_size_T);
    showConnections(graph_T, graph_size_T, *n_quant);
    fclose(bin);
}