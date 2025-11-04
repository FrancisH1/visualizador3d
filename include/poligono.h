#ifndef POLIGONO_H
#define POLIGONO_H

#include <vector>
#include <tuple>
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
    vector<array<double,4>> pontos; // 4xN, cada coluna = um ponto homogêneo

public:
    Poligono(string nome, vector<array<double,4>> vertices);

    void aplicarTransformacao(const Matriz4x4& M) override;

    vector<array<double, 4>> getPontos() override;

    ~Poligono();
};


#endif // POLIGONO_H
