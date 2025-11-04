#include "matriz4x4.h"
#include "ponto.h"
#include <cmath>

class Window3d {
private:
    Ponto eye;      // posição da câmera
    Ponto lookAt;   // ponto para onde a câmera está olhando
    Ponto vup;      // vetor View-Up
    double xMin, xMax, yMin, yMax, zMin, zMax;

public:
    // Construtor padrão
    Window3d(const Ponto& eye, const Ponto& lookAt, const Ponto& vup, 
             double xMin, double xMax,
             double yMin, double yMax,
             double zMin, double zMax);

    // Getters e Setters
    Ponto getEye() const;
    Ponto getLookAt() const;
    Ponto getVup() const;

    // Matriz de normalização (World → View)
    Matriz4x4 getMatrizNormalizacao() const;

    // Matriz de projeção ortogonal (View → NDC)
    Matriz4x4 getMatrizProjecaoOrtogonal() const;

    void transladar(double dx, double dy, double dz);
    void rotacionarY(double anguloGraus);
    void rotacionarX(double anguloGraus);
    void escalar(double fator);
};