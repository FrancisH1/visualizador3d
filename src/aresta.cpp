#include "aresta.h"

Aresta::Aresta(Ponto p1, Ponto p2) : Objeto("aresta", ARESTA), p1(p1), p2(p2) {}

void Aresta::aplicarTransformacao(const Matriz4x4 &M) {
    this->p1.coordenada = M * this->p1.coordenada;
    this->p2.coordenada = M * this->p2.coordenada;
}

vector<array<double, 4>> Aresta::getPontos(){
    return {this->p1.coordenada, this->p2.coordenada};
}

Aresta::~Aresta(){}
