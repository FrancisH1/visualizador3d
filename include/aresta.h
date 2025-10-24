#ifndef ARESTA_H
#define ARESTA_H

#include "objeto.h"
#include "ponto.h"

class Aresta : public Objeto
{
public:
    Ponto p1, p2;

    Aresta(Ponto p1, Ponto p2);

    void aplicarTransformacao(const Matriz3x3 &M) override;

    vector<array<double, 3>> getPontos() override;
};

#endif // ARESTA_H
