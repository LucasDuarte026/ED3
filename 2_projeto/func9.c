#include "func9.h"
#include "structs.h"

//Necessita fazer as alterações após a criação real do grafo na funcionalidade 8
Graph *transposeGraph(Graph *originalGraph) {
    // iterando com um vertice do original
    for (int i = 0; i < originalGraph->numVertices; i++) {
        Vertex *current = originalGraph->vertices[i].nextVertex;
        while (current != NULL) {
            // no grafo transposto, adicionando uma aresta do destino para a origem
            addEdge(transposedGraph, current->destName, originalGraph->vertices[i].destName, current->weight);
            current = current->nextVertex;
        }
    }

    return transposedGraph;
}

void functionality_9(char *binArchiveName) {
    FILE *bin = fopen(binArchiveName, "rb");
    if (bin == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    Cabecalho cabecalho;

    if (cabecalho.status == '0') {
        printf("Falha no processamento do arquivo.\n");
        fclose(bin);
        return;
    }

    Graph *transposedGraph = createGraph(); // inicializando um novo grafo

    Dados dados;
    fseek(bin, TAM_CABECALHO, SEEK_SET); // pulando o cabecalho

    while (fread(&dados.removido, sizeof(char), 1, bin)) {
        if (dados.removido == '0') {
            dados = *getRegister(bin, &dados);

            // Adicionando aresta
            addEdge(transposedGraph, dados.nomeTecnologiaDestino.string, dados.nomeTecnologiaOrigem.string, dados.peso);

            free(dados.nomeTecnologiaOrigem.string);
            free(dados.nomeTecnologiaDestino.string);
        } else {
            fseek(bin, TAM_REGISTRO - 1, SEEK_CUR); // pulando o registro removido
        }
    }

    //print do grafo transposto
    printTransposedGraph(transposedGraph);

    fclose(bin);
    freeGraph(transposedGraph);
}