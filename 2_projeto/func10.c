#include "func10.h"
#include "structs.h"
#include <stdio.h>
#include <stdbool.h>

void functionality_10(char *binArchiveName, int n) {
    FILE *bin = fopen(binArchiveName, "rb");
    if (bin == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Lê o cabeçalho para posicionar devidamente a cabeça leitora para o primeiro RRN
    Cabecalho cabecalho;

    if (cabecalho.status == '0') {
        printf("Falha no processamento do arquivo.\n");
        fclose(bin);
        return;
    }

    for (int i = 0; i < n; i++) {
        char nomeTecnologia[MAX_NAME_SIZE];
        printf("Digite o nome da tecnologia destino: ");
        scanf(" \"%[^\"]\"", nomeTecnologia); // Le as tecnologias com espaco

        printf("%s:", nomeTecnologia);
        listOriginTechnologies(bin, nomeTecnologia);
        printf("\n");
    }

    fclose(bin);
}

void listOriginTechnologies(FILE *bin, char *destTechName) {
    fseek(bin, TAM_CABECALHO, SEEK_SET); // Pula o cabeçalho

    Dados dados;
    bool isFirst = true;

    while (fread(&dados.removido, sizeof(char), 1, bin)) {
        if (dados.removido == '0') {
            dados = *getRegister(bin, &dados); //verificar aqui

            if (strcmp(dados.nomeTecnologiaDestino.string, destTechName) == 0) {
                if (!isFirst) {
                    printf(", ");
                }
                printf("%s", dados.nomeTecnologiaOrigem.string);
                isFirst = false;
            }

            free(dados.nomeTecnologiaOrigem.string);
            free(dados.nomeTecnologiaDestino.string);
        } else {
            fseek(bin, TAM_REGISTRO - 1, SEEK_CUR); // Skip removed record
        }
    }
}
