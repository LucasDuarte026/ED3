#include "structs.h"
#include "func8.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
// Bloco do HeapSort usado para ordenar o vetor de tecnologias origem
void swap(Vertex *a, Vertex *b)
{
    Vertex temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(Vertex *graph[], int n, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && strcmp(graph[left]->tecName, graph[largest]->tecName) > 0)
        largest = left;

    if (right < n && strcmp(graph[right]->tecName, graph[largest]->tecName) > 0)
        largest = right;

    if (largest != i)
    {
        swap(graph[i], graph[largest]);
        heapify(graph, n, largest);
    }
}

void heapSort(Vertex *graph[], int n)
{
    // Build max heap
    for (int i = n / 2 - 1; i >= 0; --i)
    {
        heapify(graph, n, i);
    }

    // Extract elements one by one from the heap
    for (int i = n - 1; i > 0; --i)
    {
        swap(graph[0], graph[i]);
        heapify(graph, i, 0);
    }
}
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

// Pegar um registro completo do arquivo binário
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

//  Inicializar um vértice genérico, alocando-o dinamicamente
Vertex *initVertex()
{
    Vertex *newVertex = (Vertex *)malloc(sizeof(Vertex));
    newVertex->degree = 0;
    newVertex->entrance = 0;
    newVertex->output = 0;
    newVertex->group = 0;
    newVertex->weight = 0;

    newVertex->nextVertex = NULL;
    return newVertex;
}

// Testa caso uma nova origem seja já inserida no grafo, não precisando ser colocada novamente
int alreadyInserted(Vertex *graph[], char origin[], int graph_size)
{
    for (int i = 0; i < graph_size; i++)
    {
        if (strcmp(graph[i]->tecName, origin) == 0)
        {
            return i;
        }
    }
    return -1;
}

// Adiciona uma origem no vetor do grafo
void addNewOrigin(Vertex *graph[], Dados *data, int originCounter)
{
    Vertex *newVertex = initVertex();
    strcpy(newVertex->tecName, data->nomeTecnologiaOrigem.string);
    newVertex->group = data->grupo;
    graph[originCounter] = newVertex;
}

// Adiciona um destino na lista encadeada de forma ordenada
void insertRightward(Vertex *actualVertex, Dados *data)
{
    if (actualVertex->nextVertex == NULL)
    {
        Vertex *newDestinyVertex = initVertex();
        strcpy(newDestinyVertex->tecName, data->nomeTecnologiaDestino.string);
        newDestinyVertex->weight = data->peso;
        actualVertex->nextVertex = newDestinyVertex;
    }
    else if (strcmp(data->nomeTecnologiaDestino.string, actualVertex->nextVertex->tecName) < 0)
    {

        Vertex *newIntermediate = initVertex();
        strcpy(newIntermediate->tecName, data->nomeTecnologiaDestino.string);
        newIntermediate->nextVertex = actualVertex->nextVertex;
        newIntermediate->weight = data->peso;
        actualVertex->nextVertex = newIntermediate;
    }
    else
    {
        insertRightward(actualVertex->nextVertex, data);
    }
}

// função geral que atualiza o grafo com um novo dado pego do arquivo binário
int updateGraph(Vertex *graph[], Dados *data, int graph_size)
{
    // Vertex *graph = *graph_aux;
    // if (graph == NULL)
    //     addNewOrigin(graph, data);

    int place = alreadyInserted(graph, data->nomeTecnologiaOrigem.string, graph_size);
    if (place != -1)
    {
        // printf("place:   %d\n", place);  // Usado para debug
        // Vertex *newRight = initVertex();
        insertRightward(graph[place], data);
        return graph_size;
    }
    else
    {
        addNewOrigin(graph, data, graph_size);
        insertRightward(graph[graph_size], data);
        return ++graph_size;
    }
}

// Funcionalidade exclusiva da funcionalidade 8 que mostra todo o grafo com as origens e os destinos em pares
void printFunc8(Vertex *graph[], int graph_size)
{
    for (int i = 0; i < graph_size; i++)
    {
        Vertex *currentRight;
        currentRight = graph[i];
        if (currentRight->nextVertex)
        {
            do
            {
                currentRight = currentRight->nextVertex;
                printf("%s %d %d %d %d ", graph[i]->tecName, graph[i]->group, graph[i]->entrance, graph[i]->output, graph[i]->degree);
                printf("%s %d\n", currentRight->tecName, currentRight->weight);

            } while (currentRight->nextVertex != NULL);
        }
    }
}

// Função local para printar o grafo origem e todos seus filhos
void printGraph(Vertex *graph[], int graph_size)
{
    for (int i = 0; i < graph_size; i++)
    {
        printf("|%d||G|I|O|D||%d||%d||%d||%d|\t|%s|", i, graph[i]->group, graph[i]->entrance, graph[i]->output, graph[i]->degree, graph[i]->tecName);
        Vertex *currentRight = graph[i]->nextVertex;
        if (currentRight)
        {
            int aux = 0;
            do
            {
                printf("\t%d->%s", aux++, currentRight->tecName);
                fflush(stdout);
                currentRight = currentRight->nextVertex;
            } while (currentRight);
        }
        printf("\n");
    }
}

// função completa para atualizar os graus de entrada, saida e geral de um vértice
void countDegrees(Vertex *graph[], int graph_size)
{
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // out quant contador

    for (int i = 0; i < graph_size; i++)
    {
        int outQuant = 0;

        Vertex *currentRight = graph[i]->nextVertex;
        if (currentRight)
        {
            do
            {
                outQuant++;
                currentRight = currentRight->nextVertex;
            } while (currentRight);
        }
        graph[i]->output = outQuant;
    }
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // Contar a quantidade de vertices que apontam para aquele vértice
    for (int i = 0; i < graph_size; i++)
    {

        char *desiredSeek = graph[i]->tecName;

        int inQuant = 0;
        for (int j = 0; j < graph_size; j++)
        {
            if (i == j)
            {
                j++;
                if (j >= graph_size)
                    break;
            }
            Vertex *currentRight = graph[j];
            if (currentRight->nextVertex)
            {
                do
                {
                    currentRight = currentRight->nextVertex;
                    if (strcmp(desiredSeek, currentRight->tecName) == 0)
                    {
                        inQuant++;
                    }
                } while (currentRight->nextVertex != NULL);
            }
        }
        graph[i]->entrance = inQuant;
    }
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // Contar o grau total do vértice
    for (int i = 0; i < graph_size; i++)
        graph[i]->degree = graph[i]->entrance + graph[i]->output;
}

// Funcionalidade principal chamada pela main para fazer a cração de uma Árvore de índice com base no bance de dados binário de tecnologias
void functionality_8(char *binArchiveName)
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
    printFunc8(graph, graph_size);
    fclose(bin);
}
