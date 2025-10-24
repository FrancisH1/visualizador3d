#ifndef POLIGONO_H
#define POLIGONO_H

#include <vector>
#include "objeto.h"
#include "ponto.h"
#include "matriz3x3.h"

using std::vector;
using std::array;
using std::pair;
using std::initializer_list;

class Poligono : public Objeto
{
private:
    vector<array<double,3>> pontos; // 3xN, cada coluna = um ponto homogêneo

public:
    Poligono(string nome, initializer_list<pair<double, double>> vertices);

    void aplicarTransformacao(const Matriz3x3& M) override;

    vector<array<double, 3>> getPontos() override;
};


#endif // POLIGONO_H
