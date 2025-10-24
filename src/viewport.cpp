#include "viewport.h"

Viewport::Viewport(int xMin, int yMin, int xMax, int yMax) {
    this->xMin = xMin;
    this->yMin = yMin;
    this->xMax = xMax;
    this->yMax = yMax;
}

void Viewport::setLimites(int xMin, int yMin, int xMax, int yMax) {
    this->xMin = xMin;
    this->yMin = yMin;
    this->xMax = xMax;
    this->yMax = yMax;
}

Matriz3x3 Viewport::getMatrizViewport() const {
    // 1. Calcular a largura e altura da Viewport
    double deltaX = (double)this->xMax - this->xMin;
    double deltaY = (double)this->yMax - this->yMin;

    // A. Translacao para mapear [-1, 1] para [0, 2]
    // T1: Move o canto (-1, -1) para (0, 0)
    Matriz3x3 T1 = Matriz3x3::translacao(1.0, 1.0);

    // B. Escala para o tamanho da Viewport e Inversão do Eixo Y
    // Escala X: deltaX / 2.0 (mapeia 2 unidades para deltaX pixels)
    // Escala Y: -deltaY / 2.0 (mapeia 2 unidades para deltaY pixels e inverte o eixo)
    double sx = deltaX / 2.0;
    double sy = -deltaY / 2.0;
    Matriz3x3 S = Matriz3x3::escala(sx, sy);

    // C. Translação para o Canto Final
    Matriz3x3 T2 = Matriz3x3::translacao((double)this->xMin + 20, (double)this->yMin + deltaY - 20);

    // Composição Final: M_Viewport = T2 * S * T1
    return T2 * S * T1;
}
