#ifndef TABULEIRO_HPP
#define TABULEIRO_HPP



#include <vector>
#include <iostream>
#include <fstream>
#define AUSENCIA_DE_VALOR 0

using namespace std;


class Quadrado {
  public:
    unsigned short valor; // valor da célula
    bool valorTemporario; // indicar que o valor só foi definido para verificar as restrições
    vector<Quadrado*> maiores, menores; // quadrados maiores e menores que este, respectivamente

};


class Tabuleiro {
  private:
    bool linhaOuColunaValidaParaValor(unsigned, unsigned, unsigned, unsigned, unsigned);

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

bool Tabuleiro::regrasValidasPara(unsigned short valor, unsigned short i, unsigned short j) {
  return coordenadaValida(i, j)
      && linhaOuColunaValidaParaValor(valor, i, 0, 0, 1)  // checar linha
      && linhaOuColunaValidaParaValor(valor, 0, j, 1, 0); // checar coluna
}

// admite que o valor passado e as coordenadas são válidas
bool Tabuleiro::linhaOuColunaValidaParaValor(unsigned valor, unsigned i, unsigned j, unsigned incrLinha, unsigned incrColuna) {
  if ( !coordenadaValida(i, j) ) return true;

  Quadrado quadradoAtual = matriz[i][j];

  if ((quadradoAtual.valor == valor) && (!quadradoAtual.valorTemporario)) return false; // já existe célula com o valor passado

  if (quadradoAtual.valor != AUSENCIA_DE_VALOR) {

    for (const auto quadradoMaior : quadradoAtual.maiores) {
      if ( (quadradoMaior->valor != AUSENCIA_DE_VALOR)
        && (!quadradoMaior->valorTemporario)
        && (quadradoMaior->valor < quadradoAtual.valor)) return false;
    }

    for (const auto quadradoMenor : quadradoAtual.menores) {
      if ( (quadradoMenor->valor != AUSENCIA_DE_VALOR)
        && (!quadradoMenor->valorTemporario)
        && (quadradoMenor->valor > quadradoAtual.valor)) return false;
    }

  }

  return linhaOuColunaValidaParaValor(valor, i+incrLinha, j+incrColuna, incrLinha, incrColuna); // verificar próxima linha/coluna
}



#endif