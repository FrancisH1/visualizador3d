#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "matriz3x3.h"

class Viewport {
public:
    // Coordenadas em pixels da área de desenho
    int xMin, yMin; // Canto superior esquerdo
    int xMax, yMax; // Canto inferior direito

    // Construtor: Recebe os limites em pixels da Viewport
    Viewport(int xMin, int yMin, int xMax, int yMax);

    // Método para atualizar os limites
    void setLimites(int xMin, int yMin, int xMax, int yMax);

    // Método Principal: Retorna a Matriz de Transformação da Viewport (M_Viewport)
    Matriz3x3 getMatrizViewport() const;
};

#endif // VIEWPORT_H
