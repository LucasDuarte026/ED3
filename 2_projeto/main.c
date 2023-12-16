
#include "structs.h"
#include "func8.h"
char *outArchiveName;

int main()
{

    //  -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - --

    /*  Sintaxe muito parecida com a main do projeto introdutório
     *  pois reutiliza da mesma lógica de entrada de decisões das funcionalidades
     */

    //  leitura de entrada de decisão:

    char decision[MAX_STRING_LENGTH];
    char argumento_1[MAX_STRING_LENGTH];
    char argumento_2[MAX_STRING_LENGTH];
    char argumento_3[MAX_STRING_LENGTH];
    scanf("%s", decision); // recebe a decisão

    if (strcmp(decision, "8") != 0 && strcmp(decision, "9") != 0 && strcmp(decision, "10") != 0 && strcmp(decision, "11") != 0)
    {
        printf("-> Erro de decisão\n"); // caso a escolha n seja válida
        return 1;
    }

    char *binRegName;
    int N;
    if (strcmp(decision, "8") == 0)
    {
        scanf("%s", argumento_1);
        binRegName = argumento_1;

        functionality_8(binRegName);
    }
    //  -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - --

    return 0;
}