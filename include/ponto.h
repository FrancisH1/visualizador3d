#ifndef PONTO_H
#define PONTO_H

#include "objeto.h"

class Ponto : public Objeto
{
public:
    array<double, 3> coordenada;

    Ponto(double x, double y);

    void aplicarTransformacao(const Matriz3x3 &M) override;

    vector<array<double, 3>> getPontos() override;
};

#endif // PONTO_H
