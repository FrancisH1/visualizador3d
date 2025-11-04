#ifndef PONTO_H
#define PONTO_H

#include "objeto.h"

class Ponto : public Objeto
{
public:
    array<double, 4> coordenada;

    Ponto(double x, double y, double z = 0.0, double w = 1.0);

    // Getters para coordenadas individuais
    double x() const;
    double y() const;
    double z() const;
    double w() const;

    void aplicarTransformacao(const Matriz4x4 &M) override;

    vector<array<double, 4>> getPontos() override;

    Ponto operator - (const Ponto &outro) const;
    Ponto operator + (const Ponto &outro) const;

    double produtoEscalar(const Ponto& outro) const;
    Ponto produtoVetorial(const Ponto& outro) const;
    double norma() const;
    Ponto normalizado() const;

    ~Ponto();
};

#endif // PONTO_H
