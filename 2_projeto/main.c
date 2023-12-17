
#include "structs.h"
#include "func8.h"
#include "func9.h"
#include "func10.h"
#include "func12.h"
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
    // char argumento_3[MAX_STRING_LENGTH];
    scanf("%s", decision); // recebe a decisão

    if (strcmp(decision, "8") != 0 && strcmp(decision, "9") != 0 && strcmp(decision, "10") != 0 && strcmp(decision, "11") != 0&& strcmp(decision, "12") != 0)
    {
        printf("-> Erro de decisão\n"); // caso a escolha n seja válida
        return 1;
    }

    char *binRegName;
    int n_quant;
    if (strcmp(decision, "8") == 0)
    {
        scanf("%s", argumento_1);
        binRegName = argumento_1;

        functionality_8(binRegName);
    }
    //  -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - --
    if (strcmp(decision, "9") == 0)
    {
        scanf("%s", argumento_1);
        binRegName = argumento_1;

        functionality_9(binRegName);
    }
    //  -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - --
    if (strcmp(decision, "10") == 0)
    {
        scanf("%s", argumento_1);
        scanf("%s", argumento_2);
        binRegName = argumento_1;
        n_quant = atoi(argumento_2);

        functionality_10(binRegName, &n_quant);
    }
    //  -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - --
    if (strcmp(decision, "11") == 0)
    {
      
    }
    //  -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - --
    if (strcmp(decision, "12") == 0)
    {
        scanf("%s", argumento_1);
        scanf("%s", argumento_2);
        binRegName = argumento_1;
        n_quant = atoi(argumento_2);

        functionality_12(binRegName, n_quant);
    }
    return 0;
}