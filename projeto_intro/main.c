/*
 *   Dados dos integrantes:
 *
 *   -> Lucas Sales Duarte - 11734490
 *   -> João Victor de Almeida - 13695424
 *   Alunos do curso de graduação de engenharia de computação
 */

//  inserir bibliotecas com as funções locais
#include "structs.h"
#include "func1.h"
#include "func2.h"
#include "func3.h"
#include "func4.h"

int main()
{

    //  -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - --
    //  leitura de entrada de decisão:

    char decision;
    char argumento_1[MAX_STRING_LENGTH];
    char argumento_2[MAX_STRING_LENGTH];
    char argumento_3[MAX_STRING_LENGTH];
    char argumento_4[MAX_STRING_LENGTH];
    scanf("%c", &decision); // recebe a decisão

    if (decision != '1' && decision != '2' && decision != '3' && decision != '4')
    {
        printf("-> Erro de decisão\n"); // caso a escolha n seja válida
        return 1;
    }

    // scanf("%s", argumento_4);
    // const char *argumento_4 = argv[4];   //  Caso seja necessário
    // const char *argumento_5 = argv[5];   //  Caso seja necessário

    // printf("os argumentos são \t%s\t%s\t%s\n", argumento_1, argumento_2, argumento_3);  //   Ver o que está sendo lido no momento

    // const char csvArchiveName[] = "dados1.csv";  // inserção manual do nome dos arquivos para debug
    // const char binArchiveName[] = "output_file.bin"; // inserção manual do nome dos arquivos para debug

    const char *csvArchiveName;
    const char *binArchiveName;

    int n;
    int RRN;
    switch (decision)
    {
    case '1':
        scanf("%s", argumento_2);
        scanf("%s", argumento_3);
        csvArchiveName = argumento_2;
        binArchiveName = argumento_3;
        // csvArchiveName = "dados1.csv";  // inserção manual do nome dos arquivos para debug
        // binArchiveName = "dados1.bin"; // inserção manual do nome dos arquivos para debug
        if (Functionality_1(csvArchiveName, binArchiveName))
        {
            printf("\nFalha no processamento do arquivo.");
        };
        break;
    case '2': // Supondo que a opção 2 seja para recuperar e exibir os dados
        scanf("%s", argumento_2);
        binArchiveName = argumento_2;
        functionality_2(binArchiveName);
        break;

    case '3':
        // printf("tamanho cabcalho:\t|%ld|\n",sizeof(Cabecalho)); // teste para averiguar o tamanho do cabecalho no inicio do arquivo
        scanf("%s", argumento_2);
        scanf("%s", argumento_3);

        n = atoi(argumento_3); //  pega o valor de n
        binArchiveName = argumento_2;

        functionality_3(binArchiveName, n);
        break;
    case '4':
        scanf("%s", argumento_2);
        scanf("%s", argumento_3);

        binArchiveName = argumento_2;
        RRN = atoi(argumento_3);
        functionality_4(binArchiveName, RRN);
        break;
    default:
        break;
    }

    //  -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - --

    return 0;
}
