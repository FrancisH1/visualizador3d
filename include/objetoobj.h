#ifndef OBJETOOBJ_H
#define OBJETOOBJ_H

#include "objeto.h"

class ObjetoOBJ : public Objeto {
private:
    vector<array<double,4>> vertices;
    vector<std::pair<int,int>> arestas;

public:
    ObjetoOBJ(std::string nome,
            const vector<array<double,4>>& v,
            const vector<std::pair<int,int>>& e);

    vector<array<double,4>> getPontos() override;

    const vector<std::pair<int,int>>& getArestas() const;

    void aplicarTransformacao(const Matriz4x4& M) override;
};

#endif