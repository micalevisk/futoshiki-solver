// g++ -std=c++11 solucionador.cpp -o solucionador

#include "tabuleiro.hpp"
#include <iostream>
#include <cassert>


Tabuleiro jogo;


bool validarEDefinir(unsigned short valor, unsigned short i, unsigned short j) {
  jogo.setValorQuadrado(valor, i,j);
  jogo.matriz[i][j].valorTemporario = true;

  if ( !jogo.regrasValidasPara(valor, i,j) ) {
    jogo.matriz[i][j].valorTemporario = false; // reset
    jogo.setValorQuadrado(AUSENCIA_DE_VALOR, i,j); // reset
    return false;
  }

  jogo.matriz[i][j].valorTemporario = false; // validar
  return true;
}


bool solucionar(unsigned short i, unsigned short j) {
  if (i == jogo.tamanho) return true; // já chegou no quadrado do canto inferior direito
  if (j == jogo.tamanho) return solucionar(i+1, 0); // próxima linha
  if(jogo.matriz[i][j].valor != AUSENCIA_DE_VALOR) return solucionar(i, j+1); // próxima célula

  for (unsigned short cor=1; cor <= jogo.tamanho; ++cor) {
    if ( validarEDefinir(cor, i,j) && solucionar(i, j+1) ) return true;
  }

  // apaga o valor do quadrado e retorna 'false'
  return !jogo.setValorQuadrado(AUSENCIA_DE_VALOR, i,j);
}


int main(void) {

  unsigned short N; // tamanho do tabuleiro
  unsigned short R; // quantidade de restrições

  cin >> N;
  jogo = Tabuleiro(N);

  // leitura quadro-por-quadro dos valores do tabuleiro
  for (unsigned short i=0; i < N; i++) {
    for (unsigned short j=0; j < N; j++) {
      unsigned short valor;
      cin >> valor;
      jogo.setValorQuadrado(valor, i, j);
    }
  }

  // leitura das restrições
  for (cin >> R; R-- > 0; ) {
    unsigned short ai,aj, bi,bj;
    char operador;

    cin >> ai; --ai;
    cin >> aj; --aj;
    cin >> operador;
    cin >> bi; --bi;
    cin >> bj; --bj;

    Quadrado* quadradoA = &jogo.matriz[ai][aj];
    Quadrado* quadradoB = &jogo.matriz[bi][bj];

    if (operador == '>') {
      quadradoA->menores.push_back(quadradoB);
      quadradoB->maiores.push_back(quadradoA);
    } else {
      quadradoA->maiores.push_back(quadradoB);
      quadradoB->menores.push_back(quadradoA);
    }
  }

  #ifdef DEBUG
    jogo.mostrar(true);
    assert( solucionar(0, 0) == true );
  #else
    solucionar(0, 0);
    jogo.mostrar();
  #endif

}
