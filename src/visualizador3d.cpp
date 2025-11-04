#include "visualizador3d.h"

void Visualizador3d::recalcularPipeline() {
    Mview = window.getMatrizNormalizacao();
    Mproj = window.getMatrizProjecaoOrtogonal();
    Mvp   = viewport.getMatrizViewport();
}

Visualizador3d::Visualizador3d(DisplayFile* df, const Window3d& w, const Viewport& vp)
    : window(w), viewport(vp), displayFile(df) {

    // Inicializa a matriz de transformação com a composição inicial
    this->recalcularPipeline();
}

void Visualizador3d::moverCamera(double dx, double dy, double dz) {
    this->window.transladar(dx, dy, dz);
    this->recalcularPipeline();
}

void Visualizador3d::zoomCamera(double fator) {
    this->window.escalar(fator);
    this->recalcularPipeline();
}

void Visualizador3d::rotacionarCameraY(double anguloGraus) {
    this->window.rotacionarY(anguloGraus);
    this->recalcularPipeline();
}

void Visualizador3d::rotacionarCameraX(double anguloGraus) {
    this->window.rotacionarX(anguloGraus);
    this->recalcularPipeline();
}

std::vector<DesenhoObjeto> Visualizador3d::processarDisplayFileParaDesenho() {
    std::vector<DesenhoObjeto> objetosParaDesenhar;

    // 1️. Obter matrizes do pipeline 3D
    Matriz4x4 Mview = this->window.getMatrizNormalizacao();         // World → View
    Matriz4x4 Mproj = this->window.getMatrizProjecaoOrtogonal();    // View → NDC
    Matriz4x4 Mvp   = Mproj * Mview;                                // World → NDC

    // 2️. Matriz de viewport (3x3, usada no 2D final)
    Matriz3x3 Mviewport = this->viewport.getMatrizViewport();

    // 3️. Itera sobre todos os objetos da cena
    for (const auto& objeto_ptr : this->displayFile->objetos) {
        DesenhoObjeto desenho;
        desenho.nome = objeto_ptr->getNome();

        // 4️. Obter pontos do objeto em coordenadas de mundo
        const vector<array<double, 4>>& pontosMundo = objeto_ptr->getPontos();

        // 5️. Transformar todos os pontos para o espaço de NDC ([-1,1]³)
        vector<array<double, 4>> pontosNDC;
        for (const auto& p : pontosMundo) {
            array<double, 4> p_ndc = Mvp * p; // Matriz4x4 × vetor4
            pontosNDC.push_back(p_ndc);
        }

        // 6️. Projetar em 2D (descartar o eixo Z)
        for (size_t i = 0; i < pontosNDC.size(); ++i) {
            size_t next_i = (i + 1) % pontosNDC.size();

            const auto& A = pontosNDC[i];
            const auto& B = pontosNDC[next_i];

            // Monta pontos homogêneos 3D → 2D (x, y, 1)
            array<double, 4> A2D = { A[0], A[1], 0.0, 1.0 };
            array<double, 4> B2D = { B[0], B[1], 0.0, 1.0 };

            // 7️. Aplicar transformação de viewport
            array<double, 3> AVP = Mviewport * A2D;
            array<double, 3> BVP = Mviewport * B2D;

            // 8️. Converter para coordenadas de tela (pixels)
            Ponto2D_Tela pA = { AVP[0], AVP[1] };
            Ponto2D_Tela pB = { BVP[0], BVP[1] };

            desenho.pontos.push_back(pA);
            desenho.pontos.push_back(pB);
        }

        objetosParaDesenhar.push_back(desenho);
    }

    return objetosParaDesenhar;
}

