#ifndef VISUALIZADOR3D_H
#define VISUALIZADOR3D_H

#include "window3d.h"
#include "viewport.h"
#include "displayfile.h"
#include "clipping.h"
#include "tipoprojecao.h"

struct Ponto2D_Tela {
    double x;
    double y;
};

struct DesenhoObjeto {
    string nome;
    vector<Ponto2D_Tela> pontos;
};

class Visualizador3d {
public:
    Window3d window;
    Viewport viewport;
    DisplayFile* displayFile;
    tipoProjecao projecaoAtual;
    double aspect;

    // Matrizes intermediárias
    Matriz4x4 Mview;
    Matriz4x4 Mproj;
    Matriz4x4 Mvp;
    Matriz3x3 Mviewport;

    Visualizador3d(DisplayFile* df, const Window3d& w, const Viewport& vp);

    void moverCamera(double dx, double dy, double dz);
    void zoomCamera(double fator);
    void rotacionarCameraY(double anguloGraus);
    void rotacionarCameraX(double anguloGraus);

    void alternarProjecao();

    void recalcularPipeline();
    vector<DesenhoObjeto> processarDisplayFileParaDesenho();
};

#endif