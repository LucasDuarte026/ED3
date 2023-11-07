#include "structs.h"
#include "func1.h"

//  Em static estão as funções não chamadas fora de functionality_1
void scan_quote_string(char *str)
{

    /*
     *	Use essa função para ler um campo string delimitado entre aspas (").
     *	Chame ela na hora que for ler tal campo. Por exemplo:
     *
     *	A entrada está da seguinte forma:
     *		nomeDoCampo "MARIA DA SILVA"
     *
     *	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
     *		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
     *		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
     *
     */

    char R;

    while ((R = getchar()) != EOF && isspace(R))
        ; // ignorar espaços, \r, \n...

    if (R == 'N' || R == 'n')
    { // campo NULO
        getchar();
        getchar();
        getchar();       // ignorar o "ULO" de NULO.
        strcpy(str, ""); // copia string vazia
    }
    else if (R == '\"')
    {
        if (scanf("%[^\"]", str) != 1)
        { // ler até o fechamento das aspas
            strcpy(str, "");
        }
        getchar(); // ignorar aspas fechando
    }
    else if (R != EOF)
    { // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
        str[0] = R;
        scanf("%s", &str[1]);
    }
    else
    { // EOF
        strcpy(str, "");
    }
}

static void readline(char *string)
{
    char c = 0;

    do
    {
        c = (char)getchar();

    } while (c == '\n' || c == '\r');

    int i = 0;

    do
    {
        string[i] = c;
        i++;
        c = getchar();
    } while (c != '\n' && c != '\r');

    string[i] = '\0';
}

static void binarioNaTela(const char *nomeArquivoBinario)
{ /* Você não precisa entender o código dessa função. */

    /* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
     *  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

    unsigned long i, cs;
    unsigned char *mb;
    size_t fl;
    FILE *fs;
    if (nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb")))
    {
        fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
        return;
    }
    fseek(fs, 0, SEEK_END);
    fl = ftell(fs);
    fseek(fs, 0, SEEK_SET);
    mb = (unsigned char *)malloc(fl);
    fread(mb, 1, fl, fs);

    cs = 0;
    for (i = 0; i < fl; i++)
    {
        cs += (unsigned long)mb[i];
    }
    printf("%lf\n", (cs / (double)100));
    free(mb);
    fclose(fs);
}

static FILE *init_bin(FILE *bin)
{
    Cabecalho cabecalho; //  Inicializa um cabeçario e o escreve no começo do bin com os valores iniciais a baixo
    cabecalho.status = '0';
    cabecalho.proxRRN = 0;
    cabecalho.nroTecnologia = 0;
    cabecalho.nroParesTecnologia = 0;

    fseek(bin, 0, SEEK_SET);                         // volta para o inicio, garantindo apontar para o lugar certo
    fwrite(&cabecalho.status, sizeof(char), 1, bin); //  Armazena 1  a 1 os dados como exigido
    fwrite(&cabecalho.proxRRN, sizeof(int), 1, bin);
    fwrite(&cabecalho.nroTecnologia, sizeof(int), 1, bin);
    fwrite(&cabecalho.nroParesTecnologia, sizeof(int), 1, bin);
    return bin;
}

static void Atualizar_Cabecalho(FILE *bin, char status, int nroTecnologia, int nroParesTecnologia, int prox)
{
    /*
        *   Como o próprio nome indica, atualiza os valores depois de devidamente cálculados
    */
    Cabecalho cabecalho;
    cabecalho.status = status;
    cabecalho.proxRRN = prox;
    cabecalho.nroTecnologia = nroTecnologia;
    cabecalho.nroParesTecnologia = nroParesTecnologia;

    fseek(bin, 0, SEEK_SET); // volta para o inicio
    fwrite(&cabecalho.status, sizeof(char), 1, bin);    //  campo a campo, como foi solicidado
    fwrite(&cabecalho.proxRRN, sizeof(int), 1, bin);
    fwrite(&cabecalho.nroTecnologia, sizeof(int), 1, bin);
    fwrite(&cabecalho.nroParesTecnologia, sizeof(int), 1, bin);
}

static void Escrever_Dados(FILE *bin, Dados dados)
{
    /*
        * Escreve elemento a elemento com fwrite no arquivo binário
    */
    
    fwrite(&dados.removido, sizeof(char), 1, bin);
    fwrite(&dados.grupo, sizeof(int), 1, bin);
    fwrite(&dados.popularidade, sizeof(int), 1, bin);
    fwrite(&dados.peso, sizeof(int), 1, bin);

    fwrite(&dados.nomeTecnologiaOrigem.tamanho, sizeof(int), 1, bin);
    fwrite(dados.nomeTecnologiaOrigem.string, sizeof(char), dados.nomeTecnologiaOrigem.tamanho, bin);

    fwrite(&dados.nomeTecnologiaDestino.tamanho, sizeof(int), 1, bin);
    fwrite(dados.nomeTecnologiaDestino.string, sizeof(char), dados.nomeTecnologiaDestino.tamanho, bin);

    int tamanhoAtual = sizeof(char) + 3 * sizeof(int) + 2 * sizeof(int) + dados.nomeTecnologiaOrigem.tamanho + dados.nomeTecnologiaDestino.tamanho;

    // Calcule a quantidade de lixo
    int quantidadeLixo = TAM_REGISTRO - tamanhoAtual;   //  Descobre quanto de lixo tem de ser inserido até o fim do registro de 76 bytes

    // Escreva o lixo no arquivo binário até o fim
    for (int i = 0; i < quantidadeLixo; i++)
    {
        fwrite(LIXO, sizeof(char), 1, bin);
    }
}

short int flag_origin = 1; // Teste para caso tenha de dar free ou não nos campos variaveis
short int flag_destino = 1;
static Dados LerRegistroCSV_old(FILE *csv) //  referência antiga, não funciona mais devido a necessidade do tratamento com valors NULOS
{                                          // le o registro
    Dados dados;
    dados.removido = NAO_REMOVIDO;
    dados.nomeTecnologiaOrigem.string = NULL;
    dados.nomeTecnologiaOrigem.tamanho = 0;
    dados.nomeTecnologiaDestino.string = NULL;
    dados.nomeTecnologiaDestino.tamanho = 0;

    char *nomeTecnologiaOrigem;
    int grupo;
    int popularidade;
    char *nomeTecnologiaDestino;
    int peso;

    char bufferOrigem[MAX_STRING_LENGTH];
    char bufferDestino[MAX_STRING_LENGTH];

    if (fscanf(csv, " %[^,],%d,%d, %[^,],%d", bufferOrigem, &dados.grupo, &dados.popularidade, bufferDestino, &dados.peso) == 5)
    {
        dados.nomeTecnologiaOrigem.tamanho = strlen(bufferOrigem); // documentar pq funciona
        dados.nomeTecnologiaOrigem.string = strdup(bufferOrigem);

        dados.nomeTecnologiaDestino.tamanho = strlen(bufferDestino);
        dados.nomeTecnologiaDestino.string = strdup(bufferDestino);
        printf("%s,%d,%d,%s,%d\n", dados.nomeTecnologiaOrigem.string, dados.grupo, dados.popularidade, dados.nomeTecnologiaDestino.string, dados.peso); // para ler os dados escritos
    }

    return dados;
}

static char *GetNextToken(char **str, const char *delimitador)
{

    /*
     *   Função auxiliar para pegar os valores que têm "",,"" (delimitadores consecutivos)
     */

    char *token = *str;
    if (token == NULL)
        return NULL;

    *str = strpbrk(*str, delimitador); // anda em str (na linha) até encontrar o delimitador
    if (*str != NULL)
    {
        *(*str) = '\0'; // colocar um \0 para sabermos onde é o fim da string, do elemento no caso
        (*str)++;
    }
    return token;
}

static void ProcessarElemento(Dados *dados, const char *elemento, int elementoAtual)
{
    const char value_hex[] = "\x0D\x0A"; // \r\n em hexadecimal que fica no fim da linha

    /*
     *   Em cada caso, testa-se a presença de elementos nulos, caso sejam, printa o nulo (-1 para interiros e "NULO" para strings)
     *   Caso não nulos, simplemente recebe o valor inteiro com a função atoi (string para número) ou a própria string
     */
    switch (elementoAtual) //  Testa para cada elemento da linha 1 a 5
    {
    case 1:
        if (strlen(elemento) == 0)
        {
            dados->nomeTecnologiaOrigem.tamanho = 0;
            dados->nomeTecnologiaOrigem.string = NULO;
            !flag_origin;
        }
        else
        {

            dados->nomeTecnologiaOrigem.tamanho = strlen(elemento);
            dados->nomeTecnologiaOrigem.string = strdup(elemento);
        }
        break;
    case 2:
        if (strcmp(elemento, "") == 0)
        {
            dados->grupo = -1; // e será printado NULO
        }
        else
        {
            dados->grupo = atoi(elemento);
        }
        break;
    case 3:
        if (strcmp(elemento, "") == 0)
        {
            dados->popularidade = -1; // e será printado NULO
        }
        else
        {
            dados->popularidade = atoi(elemento);
        }
        break;
    case 4:
        if (strlen(elemento) == 0)
        {
            dados->nomeTecnologiaDestino.tamanho = 0;
            dados->nomeTecnologiaDestino.string = NULO;
            !flag_destino;
        }
        else
        {

            dados->nomeTecnologiaDestino.tamanho = strlen(elemento);
            dados->nomeTecnologiaDestino.string = strdup(elemento);
        }
        break;
    case 5:
        if (strcmp(elemento, "") == 0 || strcmp(elemento, value_hex) == 0) // vê se é o fim da linha o 5 elemento, esté é nulo!. este teste eé feito pois no csv tem ,, no final e o 5º elemento nem aparece
        {
            dados->peso = -1; // e será printado NULO
        }
        else
        {
            dados->peso = atoi(elemento);
        }
        break;
    }
}

static Dados LerRegistroCSV(FILE *csv)
{
    char linha[200]; // Tamanho máximo da linha

    //  Inicializa tudo
    Dados dados;
    dados.removido = NAO_REMOVIDO;
    dados.nomeTecnologiaOrigem.string = NULL;
    dados.nomeTecnologiaOrigem.tamanho = 0;
    dados.nomeTecnologiaDestino.string = NULL;
    dados.nomeTecnologiaDestino.tamanho = 0;
    dados.popularidade = 0;
    dados.peso = 0;
    dados.grupo = 0;

    if (fgets(linha, sizeof(linha), csv) != NULL) //  Pega a linha caso não nula
    {
        char *str = linha;     //  Repassa ao ponteiro acessor
        int elementoAtual = 1; //  Inicializa o contador do elemento concorrentemente analisado

        while (1)
        {
            char *elemento = GetNextToken(&str, ","); //    Usa-se essa função pois não da pra simplesmente usar strtok, pois ele não lida com delimitadores consecutivos
            if (elemento == NULL)
            {
                break; //   Sai quando chega no fim da linha
            }

            ProcessarElemento(&dados, elemento, elementoAtual); // Processa o elemento
            // printf("iterativa: linha: %s\te o elemento: %s\n", linha, elemento); // visualizar em tempo real o que está acontecendo

            elementoAtual++; //  Passa para o próximo elemento na contagem da linha
        }
    }
    // printf("%s, %d, %d, %s, %d\r\n",  dados.nomeTecnologiaOrigem.string, dados.grupo, dados.popularidade, dados.nomeTecnologiaDestino.string, dados.peso);

    return dados;
}

static char **testa_unico(int *prt_quant_tec, Dados dado, char **tecnologies)
{
    int _quant = *prt_quant_tec; //  Passar o ponteiro para uma variável local

    // Verifica se dado.nomeTecnologiaOrigem.string já existe em tecnologies
    for (int i = 0; i < *prt_quant_tec; i++)
    {
        if (strcmp(dado.nomeTecnologiaOrigem.string, tecnologies[i]) == 0) // testa se a tecnologia existe
        {
            // Tecnologia já existe
            return tecnologies;
        }
    }

    _quant++;                                                           //  se não encontrou, soma na quantidade
    tecnologies = realloc(tecnologies, _quant * sizeof(char *));        //  E adiciona 1 espaço do tamanho de ponteiro de char (string) para tecnologias
    tecnologies[_quant - 1] = strdup(dado.nomeTecnologiaOrigem.string); //  E armazena tal string na posição correta

    *prt_quant_tec = _quant; //  devolver ao ponteiro para o valor da variável local

    return tecnologies; // E devolve tecnologies devidamente modificado
}

static char **testa_par(int *prt_quant_tec_par, Dados dado, char **pares)
{
    int stringConcatMaxSize = strlen(dado.nomeTecnologiaDestino.string) + strlen(dado.nomeTecnologiaOrigem.string) + 1; //  Para se concatenar, achas-se o tamanho total da string concatenada
    char aux[stringConcatMaxSize];                                                                                      //  Cria um auxiliar para guardar tal string concatenada
    if (dado.nomeTecnologiaOrigem.string == NULO || dado.nomeTecnologiaDestino.string == NULO)                          // Testa se algum é nulo, pois não conta
    {
        return pares;
    }
    strcpy(aux, dado.nomeTecnologiaOrigem.string);  // Copia origem na aux
    strcat(aux, dado.nomeTecnologiaDestino.string); // Concatena com destino
    int quant_tec_par = *prt_quant_tec_par;         //  Pega o ponteiro do contador analogamente ao singular
    for (int i = 0; i < quant_tec_par; i++)
    {
        if (strcmp(pares[i], aux) == 0) // Testa se existe
        {
            return pares; // Sai se existir
        }
    }

    quant_tec_par++;                                        // soma um na quantidade
    pares = realloc(pares, quant_tec_par * sizeof(char *)); // realoca dinamicamente
    pares[quant_tec_par - 1] = malloc(strlen(aux) + 1);     // Aloca memória para a nova string
    strcpy(pares[quant_tec_par - 1], aux);                  // Copia o conteúdo de aux para a nova memória alocada

    *prt_quant_tec_par = quant_tec_par; // Passa pro ponteiro
    return pares;                       //  Retorna pares devidamente modificado
}

short int Functionality_1(const char csvArchiveName[], const char binArchiveName[])
{

    FILE *csv = fopen(csvArchiveName, "r");   // buscar o csv
    FILE *bin = fopen(binArchiveName, "wb+"); // criar o bin

    if (csv == NULL || bin == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return 1; // Caso o csv não seja encontrado ou o bin não criado, passa 1 DE ERRO para o file
    }

    bin = init_bin(bin); //  Inicializar o bin

    int proxRNN = 0; //  armazena onde está o prox RNN

    int quant_tec = 0;           //  Contador para quantidade de tecnologias
    int duplicade_quant_tec = 0; //  Contador para quantidade de tecnologias duplicadas
    char **tecnologies = NULL;   //  Inicializa um vetor que armazena ponteiros strings dos singulares
    char **pares = NULL;         //  Inicializa um vetor que armazena ponteiros strings dos pares

    char headerLine[MAX_STRING_LENGTH];         //  Cria o ponteiro que armazenará a primeira linha completa
    fgets(headerLine, sizeof(headerLine), csv); //  Pular a primeira linha do csv

    while (!feof(csv)) //  Lê o arquivo até seu fim (feof)
    {

        Dados dados;                                               //   Inicializa o dado que será usado para ler e escrever nos arquivos
        dados = LerRegistroCSV(csv);                               //  Lê o arquivo linha a linha e armazana em dados
        Escrever_Dados(bin, dados);                                //  Escreve o dado no arquivo binário
        tecnologies = testa_unico(&quant_tec, dados, tecnologies); //  Função que retorna a quantidade de tecnologia unicas
        pares = testa_par(&duplicade_quant_tec, dados, pares);     //  Função que retorna a quantidade de tecnologia duplicadas em pares

        /*
        mostrar os dados -> usados para controle
            for (int i = 0; i < quant_tec; i++){printf("quant_tec:|%d|\ti:%d\t%s\n", quant_tec, i, tecnologies[i]);}
            for (int i = 0; i < quant_tec; i++)
            {
                printf("tec:\t%s", tecnologies[i]);
            }
        */

        if (!flag_origin) // Uso da flag para só dar free nos elementos variavéis caso eles não sejam nulos (não foram criados)
        {
            free(dados.nomeTecnologiaOrigem.string); // apaga os elementos variaveis para serem alocados novamente com seu tamanho variável
            !flag_origin;
        }
        if (!flag_destino) // Uso da flag para só dar free nos elementos variavéis caso eles não sejam nulos (não foram criados)
        {
            free(dados.nomeTecnologiaDestino.string); // apaga os elementos variaveis para serem alocados novamente com seu tamanho variável
            !flag_destino;
        }
        proxRNN++;
    }
    // printf("\nNumeros de tecnologia:\n\t-> Simples:\t|%d|\n\t-> Duplicado:\t|%d|\n", quant_tec, duplicade_quant_tec); // mostrar os dados para debug
    Atualizar_Cabecalho(bin, '1', quant_tec, duplicade_quant_tec, proxRNN);                                           //  Atualiza o cabeçalho com as informações finais do binário

    fclose(bin); //  Fecha o arquivo binário
    fclose(csv); //  Fecha o arquivo csv

    binarioNaTela(binArchiveName); //  Printa o valor pedido
    return 0;
}
