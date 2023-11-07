#include "structs.h"
#include "func3.h"
#include "func1.h"
//   Precisa do scan_quote_string()

/*  Como não é mais sada, fica comentada
void functionality_3old(const char binArchiveName[], int n) //  Funcionalidade usada antigamente sem tratamento de nulos
{
    FILE *bin = fopen(binArchiveName, "rb");
    if (bin == NULL)
    {
        printf("\nFalha no processamento do arquivo.\n");
        return;
    }

    Cabecalho cabecalho;
    fread(&cabecalho.status, sizeof(char), 1, bin);
    fread(&cabecalho.proxRRN, sizeof(int), 1, bin);
    fread(&cabecalho.nroTecnologia, sizeof(int), 1, bin);
    fread(&cabecalho.nroParesTecnologia, sizeof(int), 1, bin);

    if (cabecalho.status == '0')
    {
        printf("\nArquivo binário não está consistente.\n");
        fclose(bin);
        return;
    }

    for (int i = 0; i < n; i++)
    {
        char nomeCampo[MAX_STRING_LENGTH];
        char valorCampo[MAX_STRING_LENGTH];
        scanf("%s", nomeCampo);
        scan_quote_string(valorCampo);

        int found = 0;
        Dados dados;

        fseek(bin, sizeof(Cabecalho), SEEK_SET);

        while (fread(&dados.removido, sizeof(char), 1, bin))
        {
            if (dados.removido == '0')
            {
                fread(&dados.grupo, sizeof(int), 1, bin);
                fread(&dados.popularidade, sizeof(int), 1, bin);
                fread(&dados.peso, sizeof(int), 1, bin);

                fread(&dados.nomeTecnologiaOrigem.tamanho, sizeof(int), 1, bin);
                dados.nomeTecnologiaOrigem.string = (char *)malloc(dados.nomeTecnologiaOrigem.tamanho + 1);
                fread(dados.nomeTecnologiaOrigem.string, sizeof(char), dados.nomeTecnologiaOrigem.tamanho, bin);
                dados.nomeTecnologiaOrigem.string[dados.nomeTecnologiaOrigem.tamanho] = '\0';

                fread(&dados.nomeTecnologiaDestino.tamanho, sizeof(int), 1, bin);
                dados.nomeTecnologiaDestino.string = (char *)malloc(dados.nomeTecnologiaDestino.tamanho + 1);
                fread(dados.nomeTecnologiaDestino.string, sizeof(char), dados.nomeTecnologiaDestino.tamanho, bin);
                dados.nomeTecnologiaDestino.string[dados.nomeTecnologiaDestino.tamanho] = '\0';

                if ((strcmp(nomeCampo, "nomeTecnologiaOrigem") == 0 && strcmp(valorCampo, dados.nomeTecnologiaOrigem.string) == 0) ||
                    (strcmp(nomeCampo, "nomeTecnologiaDestino") == 0 && strcmp(valorCampo, dados.nomeTecnologiaDestino.string) == 0) ||
                    (strcmp(nomeCampo, "grupo") == 0 && atoi(valorCampo) == dados.grupo) ||
                    (strcmp(nomeCampo, "popularidade") == 0 && atoi(valorCampo) == dados.popularidade) ||
                    (strcmp(nomeCampo, "peso") == 0 && atoi(valorCampo) == dados.peso))
                {
                    printf("%s,%d,%d,%s,%d\n", dados.nomeTecnologiaOrigem.string, dados.grupo, dados.popularidade, dados.nomeTecnologiaDestino.string, dados.peso);
                    found = 1;
                }

                free(dados.nomeTecnologiaOrigem.string);
                free(dados.nomeTecnologiaDestino.string);
            }
            else
            {
                fseek(bin, TAM_REGISTRO, SEEK_CUR);
            }
        }

        if (!found)
        {
            printf("Registro inexistente.\n");
        }
    }

    fclose(bin);
}
*/
void printa_registro(Dados *dados)
{
    /*
     *  Ordem necessária de print:
     *  nomeTecnologiaOrigem,grupo,popularidade,nomeTecnologiaDestino,peso
     */

    // Mostra o nome da origem
    if (dados->nomeTecnologiaOrigem.tamanho == 0)
    {
        printf("NULO, ");
    }
    else
    {
        printf("%s, ", dados->nomeTecnologiaOrigem.string);
    }

    // Mostra o nome do grupo
    if (dados->grupo == -1)
    {
        printf("NULO, ");
    }
    else
    {
        printf("%d, ", dados->grupo);
    }
    // Mostra o nome do grupo
    if (dados->popularidade == -1)
    {
        printf("NULO, ");
    }
    else
    {
        printf("%d, ", dados->popularidade);
    }

    // Mostra o nome da destino
    if (dados->nomeTecnologiaDestino.tamanho == 0)
    {
        printf("NULO, ");
    }
    else
    {
        printf("%s, ", dados->nomeTecnologiaDestino.string);
    }

    // Mostra o nome do grupo
    if (dados->peso == -1)
    {
        printf("NULO");
    }
    else
    {
        printf("%d", dados->peso);
    }
    printf("\r\n"); //  Último pulo delinha
}

void functionality_3(const char binArchiveName[], int n)
{
    /*
     *   Inicialização bem semelhante as outras funcionalidades
     *   Abre o arquivo binário, lê o cabeçario para posicionar devidamente a cabeça leitora para o primeiro RRN
     */
    FILE *bin = fopen(binArchiveName, "rb");
    if (bin == NULL)
    {
        printf("\nFalha no processamento do arquivo.\n");
        return;
    }

    //  Lê o cabeçalho
    Cabecalho cabecalho;
    fread(&cabecalho.status, sizeof(char), 1, bin);
    fread(&cabecalho.proxRRN, sizeof(int), 1, bin);
    fread(&cabecalho.nroTecnologia, sizeof(int), 1, bin);
    fread(&cabecalho.nroParesTecnologia, sizeof(int), 1, bin);
    if (cabecalho.status == '0')
    {
        printf("Falha no processamento do arquivo.\r\n");
        fclose(bin);
        return;
    }

    // Para cada n inserido, uma rodada de leitura é feita com auxílio do for
    for (int i = 0; i < n; i++)
    {
        char nomeCampo[MAX_STRING_LENGTH];  //  Buffer preparado para pegar a string necessária ao nome do campo de entrada
        char valorCampo[MAX_STRING_LENGTH]; //  Buffer preparado para pegar o valor necessário do campo de entrada
        scanf("%s", nomeCampo);             //  Recebe o nome do campo
        scan_quote_string(valorCampo);      //  Recebe o valor do campo entre aspas

        int found = 0; //  inicializa a variável de encontrar
        Dados dados;   //  Inicializa os dados que serão inseridos caso ache

        fseek(bin, 13, SEEK_SET); //  Pula o cabeçalho até o primeiro RRN

        while (fread(&dados.removido, sizeof(char), 1, bin)) //  lê enquanto houver caracter não nulo
        {
            if (dados.removido == '0') //  Caso esse caracter lido seja o removido, lê o registro todo a frente elemento por elemento
            {
                fread(&dados.grupo, sizeof(int), 1, bin);
                fread(&dados.popularidade, sizeof(int), 1, bin);
                fread(&dados.peso, sizeof(int), 1, bin);

                fread(&dados.nomeTecnologiaOrigem.tamanho, sizeof(int), 1, bin);
                dados.nomeTecnologiaOrigem.string = (char *)malloc(dados.nomeTecnologiaOrigem.tamanho + 1);
                fread(dados.nomeTecnologiaOrigem.string, sizeof(char), dados.nomeTecnologiaOrigem.tamanho, bin);
                dados.nomeTecnologiaOrigem.string[dados.nomeTecnologiaOrigem.tamanho] = '\0';

                fread(&dados.nomeTecnologiaDestino.tamanho, sizeof(int), 1, bin);
                dados.nomeTecnologiaDestino.string = (char *)malloc(dados.nomeTecnologiaDestino.tamanho + 1);
                fread(dados.nomeTecnologiaDestino.string, sizeof(char), dados.nomeTecnologiaDestino.tamanho, bin);
                dados.nomeTecnologiaDestino.string[dados.nomeTecnologiaDestino.tamanho] = '\0';

                /*
                 *  IF gigante para teste de todas as possibilidades,
                 *  cruzando o nomeCampo e o valorCampo com todas
                 *  as entradas e saídas de uma vez só, economizando código
                 */

                if ((strcmp(nomeCampo, "nomeTecnologiaOrigem") == 0 && strcmp(valorCampo, dados.nomeTecnologiaOrigem.string) == 0) ||
                    (strcmp(nomeCampo, "nomeTecnologiaDestino") == 0 && strcmp(valorCampo, dados.nomeTecnologiaDestino.string) == 0) ||
                    (strcmp(nomeCampo, "grupo") == 0 && atoi(valorCampo) == dados.grupo) ||
                    (strcmp(nomeCampo, "popularidade") == 0 && atoi(valorCampo) == dados.popularidade) ||
                    (strcmp(nomeCampo, "peso") == 0 && atoi(valorCampo) == dados.peso))
                {
                    printa_registro(&dados); // função que printa o registro inteiro, com ou sem nulos
                    // printf("%s, %d, %d, %s, %d\n",  dados.nomeTecnologiaOrigem.string, dados.grupo, dados.popularidade, dados.nomeTecnologiaDestino.string, dados.peso); // Antiga forma de se printar os campos, sem tratamento de NULOS
                    found = 1; //  Caso encontre
                }

                free(dados.nomeTecnologiaOrigem.string); //  Libera o campo variável como desejado pelo projeto
                free(dados.nomeTecnologiaDestino.string);
            }
        }

        if (!found)
        {
            printf("Registro inexistente.\n"); // CASO O PEDIDO NÃO ENCONTRE NADA
        }
    }

    fclose(bin); //  Fecha o binário devidamente
}