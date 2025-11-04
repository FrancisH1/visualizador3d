#include "matriz4x4.h"

Matriz4x4::Matriz4x4(){
    elementos = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
}

Matriz4x4::Matriz4x4(initializer_list<double> lista){
    if (lista.size() != 16) {
        // Lidar com o erro se o usuário não passar 16 elementos
        throw std::invalid_argument("Matriz4x4 precisa ser inicializada com exatamente 16 elementos.");
    }
    int i = 0;
    for (double valor : lista) {
        elementos[i++] = valor;
    }
}

Matriz4x4 Matriz4x4::identidade() {
    return Matriz4x4();
}

double& Matriz4x4::operator() (int i, int j){
    return elementos[4*i + j];
}

const double& Matriz4x4::operator()(int i, int j) const{
    return elementos[4*i + j];
}

Matriz4x4 Matriz4x4::operator* (const Matriz4x4& outra) const{
    Matriz4x4 resultado;

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            resultado(i, j) = 0;
            for(int k = 0; k < 4; k++){
                resultado(i, j) += (*this)(i, k) * outra(k, j);
            }
        }
    }
    return resultado;
}

array<double, 4> Matriz4x4::operator*(const array<double, 4>& ponto) const {
    array<double, 4> ponto_transformado;

    // x' = m11*x + m12*y + m13*z + m14*w
    ponto_transformado[0] = elementos[0]*ponto[0] + elementos[1]*ponto[1] + elementos[2]*ponto[2] + elementos[3]*ponto[3];
    // y' = m21*x + m22*y + m23*z + m24*w
    ponto_transformado[1] = elementos[4]*ponto[0] + elementos[5]*ponto[1] + elementos[6]*ponto[2] + elementos[7]*ponto[3];
    // z' = m31*x + m32*y + m33*z + m34*w
    ponto_transformado[2] = elementos[8]*ponto[0] + elementos[9]*ponto[1] + elementos[10]*ponto[2] + elementos[11]*ponto[3];
    // w' = m41*x + m42+y + m43*z + m44*w
    ponto_transformado[3] = elementos[12]*ponto[0] + elementos[13]*ponto[1] + elementos[14]*ponto[2] + elementos[15]*ponto[3];

    return ponto_transformado;
}

Matriz4x4 Matriz4x4::translacao(double tx, double ty, double tz) {
    Matriz4x4 M = Matriz4x4::identidade();

    // M[0][3] (m14) = tx
    M.elementos[3] = tx;
    // M[1][3] (m24) = ty
    M.elementos[7] = ty;
    // M[2][3] (m34) = tz
    M.elementos[11] = tz;

    return M;
}

Matriz4x4 Matriz4x4::rotacaoX(double anguloGrau){
    Matriz4x4 M = Matriz4x4::identidade();

    double anguloRadiano = toRadiano(anguloGrau);

    // M[1][1]
    M.elementos[5] = cos(anguloRadiano);
    // M[1][2]
    M.elementos[6] = -sin(anguloRadiano);
    // M[2][1]
    M.elementos[9] = sin(anguloRadiano);
    // M[2][2]
    M.elementos[10] = cos(anguloRadiano);

    return M;
}

Matriz4x4 Matriz4x4::rotacaoY(double anguloGrau){
    Matriz4x4 M = Matriz4x4::identidade();

    double anguloRadiano = toRadiano(anguloGrau);

    // M[0][0]
    M.elementos[0] = cos(anguloRadiano);
    // M[0][2]
    M.elementos[2] = sin(anguloRadiano);
    // M[2][0]
    M.elementos[8] = -sin(anguloRadiano);
    // M[2][2]
    M.elementos[10] = cos(anguloRadiano);

    return M;
}

Matriz4x4 Matriz4x4::rotacaoZ(double anguloGrau){
    Matriz4x4 M = Matriz4x4::identidade();

    double anguloRadiano = toRadiano(anguloGrau);

    // M[0][0]
    M.elementos[0] = cos(anguloRadiano);
    // M[0][1]
    M.elementos[1] = -sin(anguloRadiano);
    // M[1][0]
    M.elementos[4] = sin(anguloRadiano);
    // M[1][1]
    M.elementos[5] = cos(anguloRadiano);

    return M;
}

Matriz4x4 Matriz4x4::rotacao(double angX, double angY, double angZ) {
    return rotacaoX(angX) * rotacaoY(angY) * rotacaoZ(angZ);
}

Matriz4x4 Matriz4x4::escala(double sx, double sy, double sz){
    Matriz4x4 M = Matriz4x4::identidade();

    // M[0][0]
    M.elementos[0] = sx;
    // M[1][1]
    M.elementos[5] = sy;
    // M[2][2]
    M.elementos[10] = sz;

    return M;
}


Matriz4x4::~Matriz4x4(){}