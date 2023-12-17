#ifndef FUNC8
#define FUNC8

void functionality_8(char *binArchiveName);
Vertex *initVertex();
void heapSort(Vertex *graph[], int n);
int updateGraph(Vertex *graph[], Dados *data, int graph_size);
Dados *getRegister(FILE *bin, Dados *dados);
int alreadyInserted(Vertex *graph[], char origin[],int graph_size);
void countDegrees(Vertex *graph[], int graph_size);
void printFunc8(Vertex *graph[], int graph_size);
void printGraph(Vertex *graph[], int graph_size);

#endif