#!/bin/bash

# Exemplo de loop "for" que itera de 1 a 17

rm binario1.bin
rm binario2.bin
rm binario3.bin
rm binario5.bin
rm binario6.bin
rm binario7.bin
rm binario8.bin
rm binario9.bin
rm binario10.bin

rm indice5.bin
rm indice6.bin
rm indice7.bin
rm indice8.bin
rm indice9.bin
rm indice10.bin

cp ./arquivos_v10/antes/binario1.bin binario1.bin
cp ./arquivos_v10/antes/binario2.bin binario2.bin
cp ./arquivos_v10/antes/binario3.bin binario3.bin
cp ./arquivos_v10/antes/binario5.bin binario5.bin
cp ./arquivos_v10/antes/binario6.bin binario6.bin
cp ./arquivos_v10/antes/binario7.bin binario7.bin
cp ./arquivos_v10/antes/binario8.bin binario8.bin
cp ./arquivos_v10/antes/binario9.bin binario9.bin
cp ./arquivos_v10/antes/binario10.bin binario10.bin

cp ./arquivos_v10/antes/indice5.bin indice5.bin
cp ./arquivos_v10/antes/indice6.bin indice6.bin
cp ./arquivos_v10/antes/indice7.bin indice7.bin
cp ./arquivos_v10/antes/indice8.bin indice8.bin
cp ./arquivos_v10/antes/indice9.bin indice9.bin
cp ./arquivos_v10/antes/indice10.bin indice10.bin
echo "all cleaned up"
make 
for i in {2..2}
do
  echo "Iteração $i"
  ./main < casosTeste/$i.in > output_teste
  cmp -s output_teste ./casosTeste/$i.out
  
  if [ $? -eq 0 ]; then
    echo -e "Teste $i passou" > ./testados/interacao$i.txt
  else
    echo -e "\n\nResultado da comparação (cmp):" > ./testados/interacao$i.txt
    cmp output_teste ./casosTeste/$i.out >> ./testados/interacao$i.txt
    echo -e "\n\nConteúdo de output_teste:" >> ./testados/interacao$i.txt
    cat output_teste >> ./testados/interacao$i.txt
    echo -e "\n\nConteúdo de $i.out:" >> ./testados/interacao$i.txt
    cat ./casosTeste/$i.out >> ./testados/interacao$i.txt
  fi
  
  rm  output_teste
done
