#!/bin/bash

read -p "Digite algum valor para var: " var
xxd indice${var}.bin > criado${var}.txt
xxd indice${var}Referencia.bin > referencia${var}.txt

diff -u referencia${var}.txt criado${var}.txt > resultado.txt

rm  referencia${var}.txt
rm  criado${var}.txt