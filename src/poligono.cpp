#include "poligono.h"

Poligono::Poligono(string nome, vector<array<double, 4>> vertices) : Objeto(nome, POLIGONO) {
    for (const auto& v : vertices) {
        if (v.size() == 3)
            pontos.push_back({v[0], v[1], 0.0, 1.0});
        else if (v.size() == 4)
            pontos.push_back({v[0], v[1], v[2], 1.0});
        else
            throw std::invalid_argument("Vértice inválido (esperado 2 ou 3 coordenadas)");
    }
}

void Poligono::aplicarTransformacao(const Matriz4x4& M) {
    // Itera sobre todos os pontos do polígono
    for (auto& ponto : pontos) {
        ponto = M * ponto;
    }
}

vector<array<double, 4>> Poligono::getPontos(){
    return pontos;
}

Poligono::~Poligono(){}
