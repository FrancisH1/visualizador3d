#ifndef OBJETO_H
#define OBJETO_H

#include <iostream>
#include <matriz3x3.h>
#include "tipoobjeto.h"

using std::string;

class Objeto
{
public:
    string nome;
    tipoObjeto tipo;

    Objeto(string nome, tipoObjeto tipo);

    virtual void aplicarTransformacao(const Matriz3x3 &M) = 0;

    string getNome();

    virtual vector<array<double, 3>> getPontos() = 0;

    virtual ~Objeto();
};

#endif // OBJETO_H
