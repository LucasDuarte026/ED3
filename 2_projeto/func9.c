#include "structs.h"
#include "func8.h"
#include "func9.h"

// Utilizado anteriormente mas readaptado para o grafo transposto
void addNewOrigin_T(Vertex *graph_T[], char *text, int group, int graph_size_T)
{
    Vertex *newVertex = initVertex();
    strcpy(newVertex->tecName, text);
    newVertex->group = group;
    graph_T[graph_size_T] = newVertex;
}

// Utilizado anteriormente mas readaptado para o grafo transposto
void insertRightward_T(Vertex *actualVertex, char *text, int weight)
{
    if (actualVertex->nextVertex == NULL)
    {
        Vertex *newDestinyVertex = initVertex();
        strcpy(newDestinyVertex->tecName, text);
        newDestinyVertex->weight = weight;
        actualVertex->nextVertex = newDestinyVertex;
    }
    else if (strcmp(text, actualVertex->nextVertex->tecName) < 0)
    {

        Vertex *newIntermediate = initVertex();
        strcpy(newIntermediate->tecName, text);
        newIntermediate->nextVertex = actualVertex->nextVertex;
        newIntermediate->weight = weight;
        actualVertex->nextVertex = newIntermediate;
    }
    else
    {
        insertRightward_T(actualVertex->nextVertex, text, weight);
    }
}

// Utilizado anteriormente mas readaptado para o grafo transposto
int updateGraph_T(Vertex *graph_T[], char *origin, char *destiny, int weight, int group, int graph_size_T)
{
    int place = alreadyInserted(graph_T, destiny, graph_size_T);
    if (place != -1)
    {
        // printf("place:   %d\n", place);
        // Vertex *newRight = initVertex();
        insertRightward_T(graph_T[place], origin, weight);
        return graph_size_T;
    }
    else
    {
        addNewOrigin_T(graph_T, destiny, group, graph_size_T);
        insertRightward_T(graph_T[graph_size_T], origin, weight);
        return ++graph_size_T;
    }
}

// Vê se já foi inserido no grafo_T
int alreadyTranposed(char *string, char *vectorTecnologies[], int vector_size)
{
    for (int i = 0; i < vector_size; i++)
    {
        if (strcmp(vectorTecnologies[i], string) == 0)
            return 1;
    }
    return 0;
}

// Funcção que recebe um grafo e retorna seu transposto
int transposeGraph(Vertex *graph[], Vertex *graph_T[], int graph_size, int graph_size_T)
{
    char *vectorTecnologies[MAX_ORIGINS_TECNOLOGIES];
    int vectorCounter = 0;
    for (int i = 0; i < MAX_ORIGINS_TECNOLOGIES; i++)
    {
        vectorTecnologies[i] = "$$$$$$";
    }

    // for (int i = 0; i < graph_size; i++)
    // {
    Vertex *currentRight;
    currentRight = graph[0];
    // do
    // {

    for (int i = 0; i < graph_size; i++)
    {
        if (!alreadyTranposed(currentRight->nextVertex->tecName, vectorTecnologies, vectorCounter))
        {
            addNewOrigin_T(graph_T, graph[i]->tecName, graph[i]->group, graph_size_T);
            graph_size_T++;
        }
    }
    if (currentRight->nextVertex)
    {
        if (!alreadyTranposed(currentRight->nextVertex->tecName, vectorTecnologies, vectorCounter))
        {
            for (int j = 0; j < graph_size; j++)
            {
                // if (j == i)
                //     j++;

                Vertex *second_currentRight;
                second_currentRight = graph[j];
                do
                {
                    if (second_currentRight->nextVertex)
                    {
                        // graph_T[graph_size_T] = second_currentRight->nextVertex->tecName;
                        graph_size_T = updateGraph_T(graph_T, graph[j]->tecName, second_currentRight->nextVertex->tecName, second_currentRight->nextVertex->weight, graph[j]->group, graph_size_T);

                        second_currentRight = second_currentRight->nextVertex;
                    }
                } while (second_currentRight->nextVertex != NULL);

                vectorTecnologies[vectorCounter] = strdup(currentRight->tecName);
                vectorCounter++;
                // printf("\n");
                // printGraph(graph_T, graph_size_T);
                // printf("\n");
            }
        }

        currentRight = currentRight->nextVertex;
    }
    return graph_size_T;
    //     } while (currentRight->nextVertex != NULL);
    // }
}

// função geral que abre o binário, escreve completamente o o grafo e mostra-o
void functionality_9(char *binArchiveName)
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
    int encontrado = 0;                     // para testar registro inexistente
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
            {
                // printa_registro(&dados); //  Utiliza a função já previamente criada na funcionalidade 3 para printar n tela o devido registro
                graph_size = updateGraph(graph, &dados, graph_size);
                referenceRRN++; //  Contador do registros de leitura do arquivo binário

                // Usado para printar no terminal toda a arvore. Foi anteriomente usada para debug
                encontrado = 1; //  Caso encontre, pelo menos uma vez
            }
            else
            {
                // printf("nulo");
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
    if (!encontrado)
    { // registro inexistente
        printf("Falha no processamento do arquivo.\n");
    }
    heapSort(graph, graph_size);
    countDegrees(graph, graph_size);
    // printGraph(graph, graph_size);
    // printFunc8(graph, graph_size);

    Vertex *graph_T[graph_size]; // inicializa com o máximo possível de origens. Nunca ultrapassará 200
    graph_size_T = transposeGraph(graph, graph_T, graph_size, graph_size_T);
    heapSort(graph_T, graph_size_T);
    countDegrees(graph_T, graph_size_T);
    printFunc8(graph_T, graph_size_T);
    fclose(bin);
}
