#include "poligono.h"

Poligono::Poligono(string nome, initializer_list<pair<double, double>> vertices) : Objeto(nome, POLIGONO) {
    for (const auto& v : vertices) {
        pontos.push_back({v.first, v.second, 1.0});
    }
}

void Poligono::aplicarTransformacao(const Matriz3x3& M) {
    // Itera sobre todos os pontos do polígono
    for (auto& ponto : pontos) {
        ponto = M * ponto;
    }
}

vector<array<double, 3>> Poligono::getPontos(){
    return pontos;
}
