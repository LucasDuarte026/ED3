#include "func11.h"
#include "structs.h"
#include "func9.h"

//void DFS(Vertex *graph[], bool visited[], int v, Stack *stack) {
//}

//void fillOrder(Vertex *graph[], int v, bool visited[], Stack *stack) {
//}

//Vertex **transposeGraph(Vertex *graph[], int n) {}

int countSCCs(Vertex *graph[], int n) {
    Stack *stack = createStack(n);

    // Mark all vertices as not visited
    bool *visited = (bool *)malloc(n * sizeof(bool));
    memset(visited, 0, n * sizeof(bool));

    for (int i = 0; i < n; i++)
        if (!visited[i])
            fillOrder(graph, i, visited, stack);

    Vertex **transposedGraph = transposeGraph(graph, n);

    memset(visited, 0, n * sizeof(bool));

    int count = 0;
    while (!isStackEmpty(stack)) {
        int v = pop(stack);

        if (!visited[v]) {
            DFS(transposedGraph, visited, v, NULL);
            count++;
        }
    }

    free(visited);
    freeStack(stack);
    freeGraph(transposedGraph, n);
    return count;
}

void functionality_11(char *binArchiveName) {
    Vertex *graph[MAX_ORIGINS_TECNOLOGIES];
    int graph_size = readGraphFromBin(binArchiveName, graph);

    int numSCCs = countSCCs(graph, graph_size);

    if (numSCCs == 1) {
        printf("Sim, o grafo e fortemente conexo e possui 1 componente.\n");
    } else {
        printf("Nao, o grafo nao e fortemente conexo e possui %d componentes.\n", numSCCs);
    }
}
