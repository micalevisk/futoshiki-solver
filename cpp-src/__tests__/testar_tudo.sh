#!/bin/bash
# script para compilar executar o código ../solucionador.cpp
# verificando se todos os inputs estão dando o output esperado.


g++ -std=c++11 "../solucionador.cpp" -o "solucionador"

for i in input*
do
  echo $i
  diff -w ${i/in/out} <(./solucionador.exe < $i) && echo "   ~ passou"
done

rm -f solucionador.exe*
