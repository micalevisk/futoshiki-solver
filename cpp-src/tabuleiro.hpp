#ifndef TABULEIRO_HPP
#define TABULEIRO_HPP


#include <vector>
#include <iostream>
#include <fstream>
#define AUSENCIA_DE_VALOR 0
#define INC_CIRCULAR(x, N) ((x+1)%N)

using namespace std;


class Quadrado {
  public:
    unsigned short valor; // valor da célula
    vector<Quadrado*> maiores, menores; // quadrados maiores e menores que este, respectivamente

};


class Tabuleiro {
  private:
    bool linhaValidaParaValor(unsigned short, unsigned short, unsigned short);
    bool colunaValidaParaValor(unsigned short, unsigned short, unsigned short);
    bool restricoesValidasParaQuadrado(unsigned short, unsigned short);

  public:
    unsigned short tamanho;
    vector< vector<Quadrado> > matriz;

    Tabuleiro(unsigned short);
    bool setValorQuadrado(unsigned short, unsigned short, unsigned short);
    void mostrar(bool);
    bool regrasValidasPara(unsigned short, unsigned short, unsigned short);
    bool coordenadaValida(unsigned short i, unsigned short j){ return (i < tamanho) && (j < tamanho); }
};


Tabuleiro::Tabuleiro(unsigned short _tamanho = 0) : tamanho(_tamanho) {
  matriz.assign(tamanho, vector<Quadrado>(tamanho));
}

bool Tabuleiro::setValorQuadrado(unsigned short valor, unsigned short i, unsigned short j) {
  return coordenadaValida(i, j) && ((matriz[i][j].valor = valor) || true);
}

void Tabuleiro::mostrar(bool comRestricoes = false) {
  for (unsigned short i=0; i < tamanho; i++) {
    for (unsigned short j=0; j < tamanho; j++) {
      Quadrado& q = matriz[i][j];
      if (comRestricoes) cout << '[' << q.valor << ':' << q.maiores.size() << ',' << q.menores.size() << ']';
      else cout << q.valor;
      cout << ' ';
    }
    cout << '\n';
  }
}


// verifica se o valor já existe na linha i
bool Tabuleiro::linhaValidaParaValor(unsigned short valor, unsigned short i, unsigned short j) {
  unsigned colunaAtual = INC_CIRCULAR(j, tamanho);

  do {
    if (matriz[i][colunaAtual].valor == valor) return false;
    colunaAtual = INC_CIRCULAR(colunaAtual, tamanho);
  } while (colunaAtual != j);

  return true;
}

// verifica se o valor já existe na coluna j
bool Tabuleiro::colunaValidaParaValor(unsigned short valor, unsigned short i, unsigned short j) {
  unsigned linhaAtual = INC_CIRCULAR(i, tamanho);

  do {
    if (matriz[linhaAtual][j].valor == valor) return false;
    linhaAtual = INC_CIRCULAR(linhaAtual, tamanho);
  } while (linhaAtual != i);

  return true;
}

// verificar se alguma restrição da célula (i,j) é violada
bool Tabuleiro::restricoesValidasParaQuadrado(unsigned short i, unsigned short j) {
  Quadrado q = matriz[i][j];

  for (const auto qMaior : q.maiores) {
    if (qMaior->valor != AUSENCIA_DE_VALOR
    &&  qMaior->valor <= q.valor) return false;
  }

  for (const auto qMenor : q.menores) {
    if (qMenor->valor != AUSENCIA_DE_VALOR
    &&  qMenor->valor >= q.valor) return false;
  }

  return true;
}

// verificar se as regras do jogo serão mantidas após a inserção de 'valor' em (i,j); admite coordenada válida
bool Tabuleiro::regrasValidasPara(unsigned short valor, unsigned short i, unsigned short j) {
  return restricoesValidasParaQuadrado(i, j)
      && linhaValidaParaValor(valor, i, j)
      && colunaValidaParaValor(valor, i, j);
}


#endif