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

            double x1 = A2D[0];
            double y1 = A2D[1];
            double x2 = B2D[0];
            double y2 = B2D[1];

            // 7. Recorta a linha. Se retornar true, a linha (ou o segmento resultante) é visível.
            if (clipparReta(x1, y1, x2, y2)) {

                // 5. Se aceita, aplica a Transformação de Viewport (M_Viewport)
                // Os novos (x1, y1, 1.0) e (x2, y2, 1.0) estão no SCN e agora serão mapeados para a tela.

                // Ponto A Viewport
                array<double, 3> ponto_A_VP = Mviewport * (array<double, 4>{x1, y1, 0.0, 1.0});

                // Ponto B Viewport
                array<double, 3> ponto_B_VP = Mviewport * (array<double, 4>{x2, y2, 0.0, 1.0});

                // 8. Converte para Coordenadas 2D de Tela (Pixel)
                Ponto2D_Tela pA = {ponto_A_VP[0], ponto_A_VP[1]};
                Ponto2D_Tela pB = {ponto_B_VP[0], ponto_B_VP[1]};

                // Adiciona os dois pontos (pA e pB) para desenhar a linha.
                desenho.pontos.push_back(pA);
                desenho.pontos.push_back(pB);
            }
        }

        objetosParaDesenhar.push_back(desenho);
    }

    return objetosParaDesenhar;
}

