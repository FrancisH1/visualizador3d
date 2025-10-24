#include "ponto.h"

Ponto::Ponto(double x, double y) : Objeto("ponto", PONTO) {
    this->coordenada[0] = x;
    this->coordenada[1] = y;
    this->coordenada[2] = 1;
}

void Ponto::aplicarTransformacao(const Matriz3x3 &M){
    this->coordenada = M * this->coordenada;
}

vector<array<double, 3>> Ponto::getPontos(){
    return {coordenada};
}
