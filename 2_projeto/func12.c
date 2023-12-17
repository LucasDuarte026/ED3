#include "structs.h"
#include "func1.h"
#include "func8.h"


//  Vê se uma string está dentro do vetor, retorna 1 para sim, 0 para não.
int inVector(char *string, char *alreadySeen[], int alreadySeen_size)
{
    for (int i = 0; i < alreadySeen_size; i++)
    {
        if (strcmp(alreadySeen[i], string) == 0)
        {
            return 1;
        }
    }
    return 0;
}

// função que de fato mede a distância 
int searchDestiny(Vertex *graph[], char *origin, char *destiny, int graph_size, char *alreadySeen[], int alreadySeen_size)
{
    int totalWeight = 0;
    Vertex *currentRight;
    for (int i = 0; i < graph_size; i++)
    {
        if (strcmp(origin, graph[i]->tecName) == 0)
        {
            currentRight = graph[i];
            break;
        }
    }
    alreadySeen[alreadySeen_size] = strdup(currentRight->tecName);
    alreadySeen_size++;

    if (currentRight->nextVertex)
    {
        do
        {
            currentRight = currentRight->nextVertex;
            if (currentRight->nextVertex)
            {
                if (strcmp(currentRight->tecName, destiny) == 0)
                {

                    return totalWeight + currentRight->weight;
                    break;
                }
                else if (!inVector(currentRight->tecName, alreadySeen, alreadySeen_size))
                {
                    return searchDestiny(graph, currentRight->nextVertex->tecName, destiny, graph_size, alreadySeen, alreadySeen_size);
                }
            }
        } while (currentRight->nextVertex != NULL);
    }
    return totalWeight;
}

//  Função acessora que prepara a busca dos n pedidos
void measureDistance(Vertex *graph[], int graph_size, int n)
{

    for (int i = 0; i < n; i++)
    {
        char origin[MAX_NAME_SIZE];
        char destiny[MAX_NAME_SIZE];
        scan_quote_string(origin);
        scan_quote_string(destiny);
        char *alreadySeen[200];
        int alreadySeen_size = 0;
        int weight_size = searchDestiny(graph, origin, destiny, graph_size, alreadySeen, alreadySeen_size);
        printf("\n\n Tamanho da distância é: %d", weight_size);
        fflush(stdout);
    }
}

// Funcionalidade principal, que abre o binário e puxa a busca do caminho de dos vértices de A para B
void functionality_12(char *binArchiveName, int n)
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
    countDegrees(graph, graph_size);
    measureDistance(graph, graph_size, n);
    // printFunc8(graph_T, graph_size_T);
    fclose(bin);
}
