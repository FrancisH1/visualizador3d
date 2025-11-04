#include "ponto.h"

Ponto::Ponto(double x, double y, double z, double w) : Objeto("ponto", PONTO) {
    this->coordenada[0] = x;
    this->coordenada[1] = y;
    this->coordenada[2] = z;
    this->coordenada[3] = w;
}

double Ponto::x() const { return coordenada[0]; }
double Ponto::y() const { return coordenada[1]; }
double Ponto::z() const { return coordenada[2]; }
double Ponto::w() const { return coordenada[3]; }

void Ponto::aplicarTransformacao(const Matriz4x4 &M){
    this->coordenada = M * this->coordenada;
}

vector<array<double, 4>> Ponto::getPontos(){
    return {coordenada};
}

Ponto Ponto::operator-(const Ponto &outro) const{
    return Ponto(
        this->coordenada[0] - outro.coordenada[0],
        this->coordenada[1] - outro.coordenada[1],
        this->coordenada[2] - outro.coordenada[2],
        0.0
    );
}

Ponto Ponto::operator+(const Ponto &outro) const{
    return Ponto(
        this->coordenada[0] + outro.coordenada[0],
        this->coordenada[1] + outro.coordenada[1],
        this->coordenada[2] + outro.coordenada[2],
        0.0
    );
}

double Ponto::produtoEscalar(const Ponto& outro) const {
    return coordenada[0] * outro.coordenada[0] +
           coordenada[1] * outro.coordenada[1] +
           coordenada[2] * outro.coordenada[2];
}

Ponto Ponto::produtoVetorial(const Ponto& outro) const {
    return Ponto(
        coordenada[1] * outro.coordenada[2] - coordenada[2] * outro.coordenada[1],
        coordenada[2] * outro.coordenada[0] - coordenada[0] * outro.coordenada[2],
        coordenada[0] * outro.coordenada[1] - coordenada[1] * outro.coordenada[0],
        0.0
    );
}

double Ponto::norma() const {
    return std::sqrt(
        coordenada[0] * coordenada[0] +
        coordenada[1] * coordenada[1] +
        coordenada[2] * coordenada[2]
    );
}

Ponto Ponto::normalizado() const {
    double n = norma();
    if (n == 0.0) return *this;
    return Ponto(
        coordenada[0] / n,
        coordenada[1] / n,
        coordenada[2] / n,
        0.0
    );
}

Ponto::~Ponto(){}
