#include "structs.h"
#include "func8.h"
#include "func9.h"
#include "func10.h"
#include "func11.h"
#include "stack.h"

void DFS(Vertex *graph[], bool visited[], int v, Stack *stack) {
    visited[v] = true;
    Vertex *current = graph[v]->nextVertex;
    while (current != NULL) {
        if (!visited[current->index]) {
            DFS(graph, visited, current->index, stack);
        }
        current = current->nextVertex;
    }
    if (stack != NULL) {
        push(stack, v);
    }
}

void fillOrder(Vertex *graph[], int graph_size, Stack *stack, bool visited[]) {
    for (int i = 0; i < graph_size; i++) {
        if (!visited[i]) {
            DFS(graph, visited, i, stack);
        }
    }
}

int countSCCs(Vertex *graph_T[], int graph_size, Stack *stack) {
    bool *visited = (bool *)malloc(graph_size * sizeof(bool));
    memset(visited, 0, graph_size * sizeof(bool));

    int numSCCs = 0;
    while (!isStackEmpty(stack)) {
        int v = pop(stack);
        if (!visited[v]) {
            DFS(graph_T, visited, v, NULL);
            numSCCs++;
        }
    }

    free(visited);
    return numSCCs;
}

void functionality_11(char *binArchiveName) {
    Vertex *graph[MAX_ORIGINS_TECNOLOGIES], *graph_T[MAX_ORIGINS_TECNOLOGIES];
    int graph_size = 0, graph_size_T = 0;

    functionality_8(binArchiveName, graph, &graph_size);
    functionality_9(binArchiveName, graph, graph_T, &graph_size, &graph_size_T);

    Stack *stack = createStack(graph_size);
    bool *visited = (bool *)malloc(graph_size * sizeof(bool));
    memset(visited, 0, graph_size * sizeof(bool));

    fillOrder(graph, graph_size, stack, visited);
    int numSCCs = countSCCs(graph_T, graph_size, stack);

    free(visited);
    freeStack(stack);

    if (numSCCs == 1) {
        printf("Sim, o grafo e fortemente conexo e possui 1 componente.\n");
    } else {
        printf("Nao, o grafo nao e fortemente conexo e possui %d componentes.\n", numSCCs);
    }
}

