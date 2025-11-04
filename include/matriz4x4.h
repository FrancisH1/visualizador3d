#ifndef MATRIZ4X4_H
#define MATRIZ4X4_H

#include <vector>
#include <array>
#include <stdexcept>
#include "toradiano.h"

using std::vector;
using std::array;
using std::initializer_list;

class Matriz4x4 {
private:
    // Armazena os 9 elementos da matriz (m11, m12, m13, m21, m22, m23, m31, m32, m33)
    array<double, 16> elementos;

public:
    Matriz4x4();
    Matriz4x4(initializer_list<double> lista); // Construtor com lista de inicialização

    double& operator()(int i, int j);

    const double& operator()(int i, int j) const;

    Matriz4x4 operator*(const Matriz4x4& outra) const;

    // Usado na transformação de um único ponto: P' = M * P
    array<double, 4> operator*(const array<double, 4>& ponto) const;
    
    static Matriz4x4 identidade();

    // Transformações Simples
    static Matriz4x4 translacao(double tx, double ty, double tz);
    static Matriz4x4 escala(double sx, double sy, double sz);
    static Matriz4x4 rotacaoX(double anguloGrau);
    static Matriz4x4 rotacaoY(double anguloGrau);
    static Matriz4x4 rotacaoZ(double anguloGrau);
    static Matriz4x4 rotacao(double angX, double angY, double angZ);

    ~Matriz4x4();
};


#endif