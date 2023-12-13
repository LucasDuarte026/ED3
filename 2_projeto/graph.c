#include "graph.h"
#include <stdlib.h>
#include <stdio.h>

// Function to create a graph with a given number of vertices
Graph *createGraph(int numVertices) {
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->numVertices = numVertices;
    graph->vertices = (Vertex *)malloc(numVertices * sizeof(Vertex));

    for (int i = 0; i < numVertices; i++) {
        graph->vertices[i].id = i;
        graph->vertices[i].edges = NULL;
    }

    return graph;
}

// Function to add an edge to the graph
void addEdge(Graph *graph, int src, int dest, int weight) {
    // Create a new edge
    Edge *newEdge = (Edge *)malloc(sizeof(Edge));
    newEdge->dest = dest;
    newEdge->weight = weight;
    newEdge->next = graph->vertices[src].edges;
    graph->vertices[src].edges = newEdge;
}

// Function to print the graph
void printGraph(Graph *graph) {
    for (int i = 0; i < graph->numVertices; i++) {
        Edge *temp = graph->vertices[i].edges;
        printf("Vertex %d:\n", i);
        while (temp) {
            printf(" -> %d (Weight: %d)\n", temp->dest, temp->weight);
            temp = temp->next;
        }
        printf("\n");
    }
}

// Function to transpose the graph
Graph *transposeGraph(Graph *graph) {
    Graph *transposed = createGraph(graph->numVertices);

    for (int i = 0; i < graph->numVertices; i++) {
        Edge *temp = graph->vertices[i].edges;
        while (temp) {
            addEdge(transposed, temp->dest, i, temp->weight);
            temp = temp->next;
        }
    }

    return transposed;
}

// Function to check if the graph is strongly connected
bool isStronglyConnected(Graph *graph) {
    // This function needs a detailed implementation based on DFS or another algorithm
    // Placeholder for compilation
    return true;
}

// Function to free the graph
void freeGraph(Graph *graph) {
    if (graph) {
        for (int i = 0; i < graph->numVertices; i++) {
            Edge *temp;
            while ((temp = graph->vertices[i].edges) != NULL) {
                graph->vertices[i].edges = graph->vertices[i].edges->next;
                free(temp);
            }
        }
        free(graph->vertices);
        free(graph);
    }
}