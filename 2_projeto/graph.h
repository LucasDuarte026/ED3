#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>

// Define the structure for an edge in the graph
typedef struct Edge {
    int dest;   // Destination vertex
    int weight; // Weight of the edge
    struct Edge *next; // Pointer to the next edge
} Edge;

// Define the structure for a vertex in the graph
typedef struct Vertex {
    int id;     // Vertex identifier
    Edge *edges; // Pointer to the list of edges (adjacency list)
} Vertex;

// Define the structure for the graph
typedef struct Graph {
    int numVertices; // Number of vertices in the graph
    Vertex *vertices; // Array of vertices
} Graph;

// Function prototypes
Graph *createGraph(int numVertices);
void addEdge(Graph *graph, int src, int dest, int weight);
void printGraph(Graph *graph);
Graph *transposeGraph(Graph *graph);
bool isStronglyConnected(Graph *graph);
void freeGraph(Graph *graph);

#endif // GRAPH_H
