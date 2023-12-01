#include "structsBTree.h"
#include "B-Tree.h"
#include "structs.h"
#include "func5.h"
#include "func1.h"
// #include "func3.h"

short int flag_origin7 = 1; // Teste para caso tenha de dar free ou não nos campos variaveis
short int flag_destino7 = 1;

char *GetNextToken(char **str, const char *delimitador)
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

static void ProcessElement(Dados *dados, const char *elemento, int elementoAtual)
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
            flag_origin7 = !flag_origin7;
        }
        else
        {

            dados->nomeTecnologiaOrigem.tamanho = strlen(elemento);
            dados->nomeTecnologiaOrigem.string = strdup(elemento);
        }
        break;
    case 2:
        if (strcmp(elemento, " NULO") == 0)
        {
            dados->grupo = -1; // e será printado NULO
        }
        else
        {
            dados->grupo = atoi(elemento);
        }
        break;
    case 3:
        if (strcmp(elemento, " NULO") == 0)
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
            flag_destino7 = !flag_destino7;
        }
        else
        {

            dados->nomeTecnologiaDestino.tamanho = strlen(elemento);
            dados->nomeTecnologiaDestino.string = strdup(elemento);
        }
        break;
    case 5:
        if (strcmp(elemento, " NULO") == 0 || strcmp(elemento, value_hex) == 0) // vê se é o fim da linha o 5 elemento, esté é nulo!. este teste eé feito pois no csv tem ,, no final e o 5º elemento nem aparece
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

static Dados getTerminalRegister()
{
    char linha[200]; // Tamanho máximo da linha
    for (int i = 0; i < sizeof(linha); i++)
    {
        linha[i] = '$';
    }

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
    if (fgets(linha, sizeof(linha), stdin) != NULL) //  Pega a linha caso não nula
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

            ProcessElement(&dados, elemento, elementoAtual); // Processa o elemento
            // printf("iterativa: linha: %s\te o elemento: %s\n", linha, elemento); // visualizar em tempo real o que está acontecendo

            elementoAtual++; //  Passa para o próximo elemento na contagem da linha
        }
    }
    // printf("%s, %d, %d, %s, %d\r\n", dados.nomeTecnologiaOrigem.string, dados.grupo, dados.popularidade, dados.nomeTecnologiaDestino.string, dados.peso);

    return dados;
}

Header *getIndexHeader(FILE *bin_index)
{
    Header *header = (Header *)malloc(sizeof(Header));
    fseek(bin_index, 0, SEEK_SET);
    fread(&header->status, sizeof(char), 1, bin_index);
    fread(&header->rootNode, sizeof(int), 1, bin_index);
    fread(&header->RRNnextNode, sizeof(int), 1, bin_index);
    for (int i = 0; i < 196; i++)
    {
        header->garbage[i] = '$';
        fread(&header->garbage[i], sizeof(char), 1, bin_index); // pular os 196 lixos
    }
    return header;
}

Cabecalho *getBinaryHeader(FILE *bin)
{
    fseek(bin, 0, SEEK_SET);
    Cabecalho *header = (Cabecalho *)malloc(sizeof(Cabecalho));
    fread(&header->status, sizeof(char), 1, bin);
    fread(&header->proxRRN, sizeof(int), 1, bin);
    fread(&header->nroTecnologia, sizeof(int), 1, bin);
    fread(&header->nroParesTecnologia, sizeof(int), 1, bin);

    return header;
}

static void writeRegister_inBinary(FILE *bin, Dados dados)
{
    /*
     * Escreve elemento a elemento com fwrite no arquivo binário
     */

    fseek(bin, 0, SEEK_END);
    fwrite(&dados.removido, sizeof(char), 1, bin);
    fwrite(&dados.grupo, sizeof(int), 1, bin);
    fwrite(&dados.popularidade, sizeof(int), 1, bin);
    fwrite(&dados.peso, sizeof(int), 1, bin);

    fwrite(&dados.nomeTecnologiaOrigem.tamanho, sizeof(int), 1, bin);
    fwrite(dados.nomeTecnologiaOrigem.string, sizeof(char), dados.nomeTecnologiaOrigem.tamanho, bin);

    fwrite(&dados.nomeTecnologiaDestino.tamanho, sizeof(int), 1, bin);
    fwrite(dados.nomeTecnologiaDestino.string, sizeof(char), dados.nomeTecnologiaDestino.tamanho, bin);

    int actualSize = sizeof(char) + 3 * sizeof(int) + 2 * sizeof(int) + dados.nomeTecnologiaOrigem.tamanho + dados.nomeTecnologiaDestino.tamanho;

    // Calcule a quantidade de lixo
    int trashQuant = TAM_REGISTRO - actualSize; //  Descobre quanto de lixo tem de ser inserido até o fim do registro de 76 bytes

    // Escreva o lixo no arquivo binário até o fim
    for (int i = 0; i < trashQuant; i++)
    {
        fwrite(LIXO, sizeof(char), 1, bin);
    }
}

static void updateHeader_inBinary(FILE *bin, char status, int nroTecnologia, int nroParesTecnologia, int prox)
{
    /*
     *   Como o próprio nome indica, atualiza os valores depois de devidamente cálculados
     */
    Cabecalho cabecalho;
    cabecalho.status = status;
    cabecalho.proxRRN = prox;
    cabecalho.nroTecnologia = nroTecnologia;
    cabecalho.nroParesTecnologia = nroParesTecnologia;

    fseek(bin, 0, SEEK_SET);                         // volta para o inicio
    fwrite(&cabecalho.status, sizeof(char), 1, bin); //  campo a campo, como foi solicidado
    fwrite(&cabecalho.proxRRN, sizeof(int), 1, bin);
    fwrite(&cabecalho.nroTecnologia, sizeof(int), 1, bin);
    fwrite(&cabecalho.nroParesTecnologia, sizeof(int), 1, bin);
}

static char **testa_unico(int *prt_quant_tec, Dados dado, char **tecnologies)
{
    int _quant = *prt_quant_tec; //  Passar o ponteiro para uma variável local
    int achouOrigin = 0;         //  Contar caso ache uma origem
    int achouDestino = 0;        // Contar caso ache um destino
    // printf("\n");
    // for (int i = 0; i < *prt_quant_tec; i++)
    // {
    //     printf("Elemento %d: %s\n", i + 1, tecnologies[i]);
    // }
    // Verifica se dado.nomeTecnologiaOrigem.string já existe em tecnologies
    for (int i = 0; i < *prt_quant_tec; i++)
    {
        if (dado.nomeTecnologiaOrigem.tamanho == 0)
        {
            achouOrigin = 1;
        }
        if (strcmp(dado.nomeTecnologiaOrigem.string, tecnologies[i]) == 0) // testa se a tecnologia existe
        {
            // Tecnologia já existe
            achouOrigin = 1;
        }
        if (dado.nomeTecnologiaDestino.tamanho == 0)
        {
            achouDestino = 1;
        }
        if (strcmp(dado.nomeTecnologiaDestino.string, tecnologies[i]) == 0) // testa se a tecnologia existe
        {
            // Tecnologia já existe
            achouDestino = 1;
            if (achouDestino && achouOrigin) // economizar poder computacional
                break;
        }
    }

    if (!achouOrigin)
    {
        _quant++;                                                           //  se não encontrou, soma na quantidade
        tecnologies = realloc(tecnologies, _quant * sizeof(char *));        //  E adiciona 1 espaço do tamanho de ponteiro de char (string) para tecnologias
        tecnologies[_quant - 1] = strdup(dado.nomeTecnologiaOrigem.string); //  E armazena tal string na posição correta
    }
    if (!achouDestino)
    {

        _quant++;                                                            //  se não encontrou, soma na quantidade
        tecnologies = realloc(tecnologies, _quant * sizeof(char *));         //  E adiciona 1 espaço do tamanho de ponteiro de char (string) para tecnologias
        tecnologies[_quant - 1] = strdup(dado.nomeTecnologiaDestino.string); //  E armazena tal string na posição correta
    }
    *prt_quant_tec = _quant; //  devolver ao ponteiro para o valor da variável local

    return tecnologies; // E devolve tecnologies devidamente modificado
}
static char **testa_par(int *prt_quant_tec_par, Dados dado, char **pares)
{
    int stringConcatMaxSize = strlen(dado.nomeTecnologiaDestino.string) + strlen(dado.nomeTecnologiaOrigem.string) + 1; //  Para se concatenar, achas-se o tamanho total da string concatenada
    char aux[stringConcatMaxSize];                                                                                      //  Cria um auxiliar para guardar tal string concatenada
    if (strcmp(dado.nomeTecnologiaOrigem.string, "") == 0 || strcmp(dado.nomeTecnologiaDestino.string, "") == 0)        // Testa se algum é nulo, pois não conta
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

void chargeTechnologies(FILE *bin, char **tecnologies, char **pares, int *quant_tec, int *duplicade_quant_tec)
{
    Dados dados;
    fseek(bin, TAM_CABECALHO, SEEK_SET);
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

            // printa_registro(&dados); //  Utiliza a função já previamente criada na funcionalidade 3 para printar n tela o devido registro

            tecnologies = testa_unico(quant_tec, dados, tecnologies); //  Função que retorna a quantidade de tecnologia unicas
            pares = testa_par(duplicade_quant_tec, dados, pares);     //  Função que retorna a quantidade de tecnologia duplicadas em pares

            free(dados.nomeTecnologiaOrigem.string); //  Libera as strings variaveis
            free(dados.nomeTecnologiaDestino.string);
        }
        else if (dados.removido == '1')
        {
            fseek(bin, TAM_REGISTRO - 1, SEEK_CUR); //  Pula para o próximo registro (-1 pois já le o '1')
        }
    }
}

void functionality_7(char *binArchiveName, char *outArchiveName, int N)
{
    FILE *bin = fopen(binArchiveName, "rb+");       // binario de registros
    FILE *bin_index = fopen(outArchiveName, "rb+"); // arquivo de indice

    if (bin == NULL || bin_index == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return; // Caso o csv não seja encontrado ou o bin não criado, passa 1 DE ERRO para o file
    }

    int highestTree = 0; // Valor da atual altura maxima da arvore
    Header *headerIndex;
    Cabecalho *headerBinary;
    char auxiliar_lixo[5];
    fgets(auxiliar_lixo, sizeof(auxiliar_lixo), stdin); // remover a primeira linha de lixo restante do fim da leitura anterior

    for (int i = 0; i < N; i++)
    {
        int proxRNN = 0;             //  armazena onde está o prox RNN
        int quant_tec = 0;           //  Contador para quantidade de tecnologias
        int duplicade_quant_tec = 0; //  Contador para quantidade de tecnologias duplicadas
        char **tecnologies = NULL;   //  Inicializa um vetor que armazena ponteiros strings dos singulares
        char **pares = NULL;         //  Inicializa um vetor que armazena ponteiros strings dos pares

        Dados dados;                   //   Inicializa o dado que será usado para ler e escrever nos arquivos
        dados = getTerminalRegister(); //  Lê o arquivo linha a linha e armazana em dados

        headerIndex = getIndexHeader(bin_index);
        headerBinary = getBinaryHeader(bin);
        chargeTechnologies(bin, tecnologies, pares, &quant_tec, &duplicade_quant_tec);

        writeRegister_inBinary(bin, dados); //  Escreve o dado no arquivo binário

        updateHeader_inBinary(bin, '0', quant_tec, duplicade_quant_tec, proxRNN); //  Atualiza o cabeçalho com as informações finais do binário

        BTreeNode *root = initNode();
        root = getRoot(bin_index, root);
        if (highestTree < heightTree(bin_index, root))
            highestTree = heightTree(bin_index, root);
        headerIndex->RRNnextNode++;
        headerBinary->proxRRN++;
        insertIndex(bin_index, &dados, &highestTree, &headerIndex->RRNnextNode, headerBinary->proxRRN);

        if (!flag_origin7) // Uso da flag para só dar free nos elementos variavéis caso eles não sejam nulos (não foram criados)
        {
            free(dados.nomeTecnologiaOrigem.string); // apaga os elementos variaveis para serem alocados novamente com seu tamanho variável
            flag_origin7 = !flag_origin7;
        }
        if (!flag_destino7) // Uso da flag para só dar free nos elementos variavéis caso eles não sejam nulos (não foram criados)
        {
            free(dados.nomeTecnologiaDestino.string); // apaga os elementos variaveis para serem alocados novamente com seu tamanho variável
            flag_destino7 = !flag_destino7;
        }
        updateHeader_inBinary(bin, '0', quant_tec, duplicade_quant_tec, proxRNN); //  Atualiza o cabeçalho com as informações finais do binário
        updateHeader(bin_index, '0', -1, &headerIndex->RRNnextNode);
    }
    char one = '1';
    fseek(bin, 0, SEEK_SET);
    fwrite(&one, sizeof(char), 1, bin);

    fseek(bin_index, 0, SEEK_SET);
    fwrite(&one, sizeof(char), 1, bin_index);

    fclose(bin);
    fclose(bin_index);
    binarioNaTela(binArchiveName);
    binarioNaTela(outArchiveName);
}
