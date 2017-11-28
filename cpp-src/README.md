## Entrada e Saída

A primeira lina da entrada é um número **N** (`2 < N <= 9`) indicando a ordem do tabuleiro.

Então seguem-se **N** linhas, cada uma contendo **N** números naturais de `1` a **N** representando o valore de um quadrado do tabuleiro.

Se esse valor for igual a `0` então significa que o quadrado da posição _(i,j)_ está vazio.

Após as **N**, haverá um número natural **R** (`R << N^2`) indicando a quantidade de restrições (operadores de desigualdade) dispostas no tabuleiro.

E seguirão **R** linhas descrevendo tais restrições, formatadas como: `ai aj S bi bj`, significando que a relação entre os quadrados de coordenadas _(ai, aj)_ e _(bi, bj)_ é `S`,
que pode ser `>` (maior que) ou `<` (menor que). Consequentemente, essas coordenadas deverão ser diferentes.

A quantidade de leituras será: `(1 + N^2) + (1 + 5\times R)`. Esse valor pode ser \emph{bem} reduzido se a leitura for realiza em termos de linhas, resultando em `(1 + N) + (1 + R)`.

A saída dada pelo programa que solucionará o jogo consiste em apenas **N** linhas com **N** colunas sem valores iguais a `0`.