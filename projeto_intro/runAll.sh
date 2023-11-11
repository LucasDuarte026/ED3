#!/bin/bash

# Exemplo de loop "for" que itera de 1 a 17
make
for i in {1..17}
do
  echo "Iteração $i"
  ./main < casos_teste/$i.in > output_teste
  cmp -s output_teste ./casos_teste/$i.out
  
  if [ $? -eq 0 ]; then
    echo -e "Teste $i passou" > ./testados/interacao$i.txt
  else
    echo -e "\n\nResultado da comparação (cmp):" > ./testados/interacao$i.txt
    cmp output_teste ./casos_teste/$i.out >> ./testados/interacao$i.txt
    echo -e "\n\nConteúdo de output_teste:" >> ./testados/interacao$i.txt
    cat output_teste >> ./testados/interacao$i.txt
    echo -e "\n\nConteúdo de $i.out:" >> ./testados/interacao$i.txt
    cat ./casos_teste/$i.out >> ./testados/interacao$i.txt
  fi
  
  rm  output_teste
done
