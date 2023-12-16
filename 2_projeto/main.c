
#include "B-Tree.h"
char *outArchiveName;

int main()
{

    //  -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - --

    /*  Sintaxe muito parecida com a main do projeto introdutório
     *  pois reutiliza da mesma lógica de entrada de decisões das funcionalidades
     */

    //  leitura de entrada de decisão:

    char decision;
    char argumento_1[MAX_STRING_LENGTH];
    char argumento_2[MAX_STRING_LENGTH];
    char argumento_3[MAX_STRING_LENGTH];
    scanf("%c", &decision); // recebe a decisão

    if (decision != '8' && decision != '9' && decision != '10' && decision != '11')
    {
        printf("-> Erro de decisão\n"); // caso a escolha n seja válida
        return 1;
    }

    char *binRegName;
    int N;
    switch (decision)
    {
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
        scanf("%s", argumento_2);
        scanf("%s", argumento_3);
        binRegName = argumento_2;
        outArchiveName = argumento_3;

        functionality_5(binRegName, outArchiveName);

        break;
    case '6':
        scanf("%s", argumento_1);
        scanf("%s", argumento_2);
        scanf("%s", argumento_3);
        binRegName = argumento_1;
        outArchiveName = argumento_2;
        N = atoi(argumento_3);

        functionality_6(binRegName, outArchiveName, N);
        break;
    case '7':
        scanf("%s", argumento_1);
        scanf("%s", argumento_2);
        scanf("%s", argumento_3);
        binRegName = argumento_1;
        outArchiveName = argumento_2;
        N = atoi(argumento_3);

        functionality_7(binRegName, outArchiveName, N);
        break;
    case '8':

        break;
    default:
        break;
    }

    //  -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - --

    return 0;
}