
#include "structsBTree.h"
#include "B-Tree.h"
#include "func5.h"

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
    char argumento_4[MAX_STRING_LENGTH];
    scanf("%c", &decision); // recebe a decisão

    if (decision != '5' && decision != '6' && decision != '7' && decision != '8')
    {
        printf("-> Erro de decisão\n"); // caso a escolha n seja válida
        return 1;
    }

    char *binRegName;
    char *binArchiveName;

    switch (decision)
    {
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
        // scanf("%s", argumento_2);
        // scanf("%s", argumento_3);
        // binRegName = argumento_2;
        // binArchiveName = argumento_3;
        binRegName = "dados1.bin";     // inserção manual do nome dos arquivos para debug
        binArchiveName = "output.bin"; // inserção manual do nome dos arquivos para debug
                                       // functionality_5(binRegName, binArchiveName);


        // teste para p1
        char *a = "H";
        char *b = "G";
        char *c = "F";
        char *d = "E";
        char *e = "D";
        char *f = "C";
        char *g = "B";
        char *h = "A";

// teste para p4
        // char *a = "A";
        // char *b = "B";
        // char *c = "C";
        // char *d = "D";
        // char *e = "E";
        // char *f = "F";
        // char *g = "G";
        // char *h = "H";
        // char *i = "I";
        // char *j = "J";

        // char *a = "A";
        // char *b = "Z";
        // char *c = "W";
        // char *d = "U";
        // char *e = "V";
        // char *f = "C";
        // char *g = "I";
        // char *h = "F";
        // Dados dados;

        BTreeNode *root;
        root = initNode();
        int highestTree = 0;
        insertIndexString(&root, a, &highestTree);
        insertIndexString(&root, b, &highestTree);
        insertIndexString(&root, c, &highestTree);
        insertIndexString(&root, d, &highestTree);
        insertIndexString(&root, e, &highestTree);
        insertIndexString(&root, f, &highestTree);
        insertIndexString(&root, g, &highestTree);
        insertIndexString(&root, h, &highestTree);
        insertIndexString(&root, i, &highestTree);
        insertIndexString(&root, j, &highestTree);

        break;
    case '6':
    case '7':
    case '8':

        break;
    default:
        break;
    }

    //  -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - --

    return 0;
}