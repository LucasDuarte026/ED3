# Árvore B - Projeto de Indexação para Bancos de Dados

Este repositório contém a implementação de um algoritmo de Árvore B em C, projetado para tratar e armazenar dados em memória secundária. O principal propósito é criar uma tabela de índice para facilitar o acesso a grandes bancos de dados, especialmente otimizado para chaves do tipo string.

A árvore B implementada possui capacidade de auto balanceamento durante as operações de inserção, utilizando técnicas de split de nó quando há transbordamento de dados.

A implementação em C visa otimizar a velocidade do algoritmo, especialmente no que diz respeito às operações recursivas de abertura e criação de arquivos binários.

## Funcionalidades Implementadas

1. **Criação de Tabela de Índice:** Geração de uma tabela de índice organizada em Árvore B a partir de um banco de dados presente em um arquivo binário.

2. **Busca Binária:** Realização de uma busca binária nos índices para recuperar o índice correspondente a um registro no banco de dados. Isso permite uma leitura direta, evitando buscas lineares no arquivo de registros.

3. **Inserção de Registros:** Adição de novos registros no arquivo binário, acompanhada da inserção correspondente no arquivo de índices.

## Integrantes do Projeto

1. **Lucas Sales Duarte**
   - NUSP: 11734490

2. **Joao Victor de Almeida**
   - NUSP: 13695424

## Vídeo Explicativo

[Assista ao vídeo do projeto no YouTube](https://www.youtube.com/watch?v=M1GFouMKVIs)

