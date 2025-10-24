#ifndef MATRIZ3X3_H
#define MATRIZ3X3_H

#include <vector>
#include <array>
#include <cmath>
#include <stdexcept>

double toRadiano(double anguloGrau);

using std::vector;
using std::array;
using std::initializer_list;

class Matriz3x3 {
private:
    // Armazena os 9 elementos da matriz (m11, m12, m13, m21, m22, m23, m31, m32, m33)
    array<double, 9> elementos;

public:
    Matriz3x3();
    Matriz3x3(initializer_list<double> lista); // Construtor com lista de inicialização

    double& operator()(int i, int j);

    const double& operator()(int i, int j) const;

    Matriz3x3 operator*(const Matriz3x3& outra) const;

    // Usado na transformação de um único ponto: P' = M * P
    array<double, 3> operator*(const array<double, 3>& ponto) const;

    static Matriz3x3 identidade();

    // Transformações Simples
    static Matriz3x3 translacao(double tx, double ty);
    static Matriz3x3 escala(double sx, double sy);
    static Matriz3x3 rotacao(double anguloGrau);
};

#endif // MATRIZ3X3_H
