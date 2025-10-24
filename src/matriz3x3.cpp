#include "matriz3x3.h"

double toRadiano(double anguloGrau){
    return (anguloGrau * M_PI) / 180;
}

Matriz3x3::Matriz3x3(){
    elementos = {
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
    };
}

Matriz3x3::Matriz3x3(initializer_list<double> lista){
    if (lista.size() != 9) {
        // Lidar com o erro se o usuário não passar 9 elementos
        throw std::invalid_argument("Matriz3x3 precisa ser inicializada com exatamente 9 elementos.");
    }
    int i = 0;
    for (double valor : lista) {
        elementos[i++] = valor;
    }
}

Matriz3x3 Matriz3x3::identidade() {
    return Matriz3x3();
}

double& Matriz3x3::operator() (int i, int j){
    return elementos[3*i + j];
}

const double& Matriz3x3::operator()(int i, int j) const{
    return elementos[3*i + j];
}

Matriz3x3 Matriz3x3::operator* (const Matriz3x3& outra) const{
    Matriz3x3 resultado;

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            resultado(i, j) = 0;
            for(int k = 0; k < 3; k++){
                resultado(i, j) += (*this)(i, k) * outra(k, j);
            }
        }
    }
    return resultado;
}

array<double, 3> Matriz3x3::operator*(const array<double, 3>& ponto) const {
    array<double, 3> ponto_transformado;

    // x' = m11*x + m12*y + m13*w
    ponto_transformado[0] = elementos[0]*ponto[0] + elementos[1]*ponto[1] + elementos[2]*ponto[2];
    // y' = m21*x + m22*y + m23*w
    ponto_transformado[1] = elementos[3]*ponto[0] + elementos[4]*ponto[1] + elementos[5]*ponto[2];
    // w' = m31*x + m32*y + m33*w
    ponto_transformado[2] = elementos[6]*ponto[0] + elementos[7]*ponto[1] + elementos[8]*ponto[2];

    return ponto_transformado;
}

Matriz3x3 Matriz3x3::translacao(double tx, double ty) {
    Matriz3x3 M = Matriz3x3::identidade();

    // M[0][2] (m13) = tx
    M.elementos[2] = tx;
    // M[1][2] (m23) = ty
    M.elementos[5] = ty;

    return M;
}

Matriz3x3 Matriz3x3::rotacao(double anguloGrau){
    Matriz3x3 M = Matriz3x3::identidade();

    double anguloRadiano = toRadiano(anguloGrau);

    // M[0][0]
    M.elementos[0] = cos(anguloRadiano);
    // M[0][1]
    M.elementos[1] = -sin(anguloRadiano);
    // M[1][0]
    M.elementos[3] = sin(anguloRadiano);
    // M[1][1]
    M.elementos[4] = cos(anguloRadiano);

    return M;
}

Matriz3x3 Matriz3x3::escala(double sx, double sy){
    Matriz3x3 M = Matriz3x3::identidade();

    // M[0][0]
    M.elementos[0] = sx;
    // M[1][1]
    M.elementos[4] = sy;

    return M;
}
