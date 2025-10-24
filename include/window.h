#ifndef WINDOW_H
#define WINDOW_H

#include "matriz3x3.h"

class Window {
public:
    double xMin, yMin, xMax, yMax;
    double centroX, centroY;
    double angulo; // Angulo de rotação da Window

    void atualizarCentro();

    Window(double xMin, double yMin, double xMax, double yMax);

    // Métodos de Transformação
    void mover(double dx, double dy);
    void zoom(double fator);
    void rotacionar(double anguloGrau);

    Matriz3x3 getMatrizNormalizacao() const;
};

#endif // WINDOW_H
