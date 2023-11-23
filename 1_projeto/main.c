
#include "structsBTree.h"
#include "B-Tree.h"
#include "func5.h"
#include "func6.h"

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
        binArchiveName = argumento_3;

        functionality_5(binRegName, binArchiveName);

        /*
        // teste para p1
        char *a = "Z";
        char *b = "Y";
        char *c = "X";
        char *d = "W";
        char *e = "V";
        char *f = "U";
        char *g = "T";
        char *h = "S";
        char *i = "R";
        char *j = "Q";
        char *k = "P";
        char *l = "O";
        char *m = "N";
        char *n = "M";
        char *o = "L";
        char *p = "K";
        char *q = "J";
        char *r = "I";
        char *s = "H";
        char *t = "G";
        char *u = "F";
        char *v = "E";
        char *w = "D";
        char *x = "C";
        char *y = "B";
        char *z = "A";

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
        // insertIndexString(&root, a, &highestTree);
        // insertIndexString(&root, b, &highestTree);
        // insertIndexString(&root, c, &highestTree);
        // insertIndexString(&root, d, &highestTree);
        // insertIndexString(&root, e, &highestTree);
        // insertIndexString(&root, f, &highestTree);
        // insertIndexString(&root, g, &highestTree);
        // insertIndexString(&root, h, &highestTree);
        // insertIndexString(&root, i, &highestTree);
        // insertIndexString(&root, j, &highestTree);
        // insertIndexString(&root, k, &highestTree);
        // insertIndexString(&root, l, &highestTree);
        // insertIndexString(&root, m, &highestTree);
        // insertIndexString(&root, n, &highestTree);
        // insertIndexString(&root, o, &highestTree);
        // insertIndexString(&root, p, &highestTree);
        // insertIndexString(&root, q, &highestTree);
        // insertIndexString(&root, r, &highestTree);
        // insertIndexString(&root, s, &highestTree);
        // insertIndexString(&root, t, &highestTree);
        // insertIndexString(&root, u, &highestTree);
        // insertIndexString(&root, v, &highestTree);
        // insertIndexString(&root, w, &highestTree);
        // insertIndexString(&root, x, &highestTree);
        // insertIndexString(&root, y, &highestTree);
        // insertIndexString(&root, z, &highestTree);

        insertIndexString(&root, "z", &highestTree);
        insertIndexString(&root, "y", &highestTree);
        insertIndexString(&root, "x", &highestTree);
        insertIndexString(&root, "w", &highestTree);
        insertIndexString(&root, "v", &highestTree);
        insertIndexString(&root, "u", &highestTree);
        insertIndexString(&root, "t", &highestTree);
        insertIndexString(&root, "s", &highestTree);
        insertIndexString(&root, "r", &highestTree);
        insertIndexString(&root, "q", &highestTree);
        insertIndexString(&root, "p", &highestTree);
        insertIndexString(&root, "o", &highestTree);
        insertIndexString(&root, "n", &highestTree);
        insertIndexString(&root, "m", &highestTree);
        insertIndexString(&root, "l", &highestTree);
        insertIndexString(&root, "k", &highestTree);
        insertIndexString(&root, "j", &highestTree);
        insertIndexString(&root, "i", &highestTree);
        insertIndexString(&root, "h", &highestTree);
        insertIndexString(&root, "g", &highestTree);
        insertIndexString(&root, "f", &highestTree);
        insertIndexString(&root, "e", &highestTree);
        insertIndexString(&root, "d", &highestTree);
        insertIndexString(&root, "c", &highestTree);
        insertIndexString(&root, "b", &highestTree);
        insertIndexString(&root, "a", &highestTree);
        */
        break;
    case '6':
        scanf("%s", argumento_1);
        scanf("%s", argumento_2);
        scanf("%s", argumento_3);
        binRegName = argumento_1;
        binArchiveName = argumento_2;
        N = atoi(argumento_3);

        functionality_6(binRegName, binArchiveName,N );
    case '7':
    case '8':

        break;
    default:
        break;
    }

    //  -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - -- -- - --

    return 0;
}