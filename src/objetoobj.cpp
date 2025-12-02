#include "objetoobj.h"

ObjetoOBJ::ObjetoOBJ(std::string nome,
            const vector<array<double,4>>& v,
            const vector<std::pair<int,int>>& e)
        : Objeto(nome, POLIGONO), vertices(v), arestas(e) {}

vector<array<double,4>> ObjetoOBJ::getPontos() {
        return vertices;
}

const vector<std::pair<int,int>>& ObjetoOBJ::getArestas() const {
    return arestas;
}

void ObjetoOBJ::aplicarTransformacao(const Matriz4x4& M) {
    for (auto& v : vertices)
        v = M * v;
}