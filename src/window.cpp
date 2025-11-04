#include "window.h"

Window::Window(double xMin, double yMin, double xMax, double yMax) {
    this->xMin = xMin;
    this->yMin = yMin;
    this->xMax = xMax;
    this->yMax = yMax;
    this->angulo = 0.0;
    this->atualizarCentro();
}

void Window::atualizarCentro() {
    this->centroX = (this->xMin + this->xMax) / 2.0;
    this->centroY = (this->yMin + this->yMax) / 2.0;
}

void Window::mover(double dx, double dy) {
    // Apenas translada os limites da Window. O centro é atualizado automaticamente.
    this->xMin += dx;
    this->xMax += dx;
    this->yMin += dy;
    this->yMax += dy;
    this->atualizarCentro();
}

void Window::zoom(double fator) {
    double largura = this->xMax - this->xMin;
    double altura = this->yMax - this->yMin;

    double novaLargura = largura / fator;
    double novaAltura = altura / fator;

    double deltaX = (novaLargura - largura) / 2.0;
    double deltaY = (novaAltura - altura) / 2.0;

    // Recalcula os novos limites
    this->xMin -= deltaX;
    this->xMax += deltaX;
    this->yMin -= deltaY;
    this->yMax += deltaY;

    this->atualizarCentro();
}

void Window::rotacionar(double anguloGrau) {
    this->angulo += anguloGrau;
}

Matriz3x3 Window::getMatrizNormalizacao() const {
    // 1. Matriz de Translação do Centro para a Origem (T)
    Matriz3x3 T = Matriz3x3::translacao(-this->centroX, -this->centroY);

    // 2. Matriz de Rotação Inversa (para alinhar a Window com os eixos, R)
    Matriz3x3 R = Matriz3x3::rotacao(-this->angulo);

    // 3. Matriz de Escala para o SCN (S)
    double largura = this->xMax - this->xMin;
    double altura = this->yMax - this->yMin;

    // SCN: Escala de (Largura/Altura) para (2x2), para o range [-1, 1]
    double sx = 2.0 / largura;
    double sy = 2.0 / altura;

    Matriz3x3 S = Matriz3x3::escala(sx, sy);

    // Composição: M_Normalizacao = S * R * T1
    return S * R * T;
}

Window::~Window(){}
