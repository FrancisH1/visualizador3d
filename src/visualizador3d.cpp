#include "visualizador3d.h"
#include "objetoobj.h"

void Visualizador3d::recalcularPipeline() {
    Mview = window.getMatrizView();

    Mproj = this->projecaoAtual == ORTOGONAL ? window.getMatrizProjecaoOrtogonal() : window.getMatrizPerspectiva(this->viewport.aspect);
    
    Mvp = Mproj * Mview;

    Mviewport = viewport.getMatrizViewport();
}

Visualizador3d::Visualizador3d(DisplayFile* df, const Window3d& w, const Viewport& vp)
    : window(w), viewport(vp), displayFile(df) {

    // Inicializa a matriz de transformação com a composição inicial
    this->recalcularPipeline();
    this->projecaoAtual = ORTOGONAL;
}

void Visualizador3d::alternarProjecao(){
    this->projecaoAtual = this->projecaoAtual == ORTOGONAL ? PERSPECTIVA : ORTOGONAL;
}

void Visualizador3d::moverCamera(double dx, double dy, double dz) {
    this->window.transladar(dx, dy, dz);
    this->recalcularPipeline();
}

void Visualizador3d::zoomCamera(double fator) {
    this->window.escalar(fator, projecaoAtual);
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
    recalcularPipeline();

    Matriz3x3 Mviewport = this->viewport.getMatrizViewport();

    for (const auto& objeto_ptr : this->displayFile->objetos) {
        DesenhoObjeto desenho;
        desenho.nome = objeto_ptr->getNome();

        const vector<array<double, 4>>& pontosMundo = objeto_ptr->getPontos();

        // transformar pontos para NDC
        vector<array<double, 4>> pontosNDC;

        if(projecaoAtual == ORTOGONAL){
            for (const auto& p : pontosMundo) {
                array<double, 4> p_ndc = Mvp * p;
                pontosNDC.push_back(p_ndc);
            }
        }
        else{
            for (const auto& p : pontosMundo) {
                array<double, 4> p_ndc = Mvp * p;

                if (p_ndc[3] <= 0.0001) {
                    // Vértice inválido - adiciona um marcador
                    pontosNDC.push_back({0, 0, 0, -1}); // w negativo = inválido
                    continue;
                }

                p_ndc[0] /= p_ndc[3];
                p_ndc[1] /= p_ndc[3];
                p_ndc[2] /= p_ndc[3];
                pontosNDC.push_back(p_ndc);
            }
        }

        //  verifica se é ObjetoOBJ
        ObjetoOBJ* objetoOBJ = dynamic_cast<ObjetoOBJ*>(objeto_ptr.get());

        if (objetoOBJ != nullptr) {
            // usar as arestas
            const auto& arestas = objetoOBJ->getArestas();

            for (const auto& aresta : arestas) {
                int idx1 = aresta.first;
                int idx2 = aresta.second;

                // Verificar se os índices são válidos
                if (idx1 < 0 || idx1 >= (int)pontosNDC.size() ||
                    idx2 < 0 || idx2 >= (int)pontosNDC.size())
                    continue;

                const auto& A = pontosNDC[idx1];
                const auto& B = pontosNDC[idx2];

                // Pular se algum vértice for inválido (w < 0)
                if (A[3] < 0 || B[3] < 0)
                    continue;

                double x1 = A[0];
                double y1 = A[1];
                double x2 = B[0];
                double y2 = B[1];

                if (clipparReta(x1, y1, x2, y2)) {
                    array<double, 3> ponto_A_VP = Mviewport * (array<double, 4>{x1, y1, 0.0, 1.0});
                    array<double, 3> ponto_B_VP = Mviewport * (array<double, 4>{x2, y2, 0.0, 1.0});

                    Ponto2D_Tela pA = {ponto_A_VP[0], ponto_A_VP[1]};
                    Ponto2D_Tela pB = {ponto_B_VP[0], ponto_B_VP[1]};

                    desenho.pontos.push_back(pA);
                    desenho.pontos.push_back(pB);
                }
            }
        }
        else {
            // conectar vértices sequencialmente
            for (size_t i = 0; i < pontosNDC.size(); ++i) {
                size_t next_i = (i + 1) % pontosNDC.size();

                const auto& A = pontosNDC[i];
                const auto& B = pontosNDC[next_i];

                array<double, 4> A2D = { A[0], A[1], 0.0, 1.0 };
                array<double, 4> B2D = { B[0], B[1], 0.0, 1.0 };

                double x1 = A2D[0];
                double y1 = A2D[1];
                double x2 = B2D[0];
                double y2 = B2D[1];

                if (clipparReta(x1, y1, x2, y2)) {
                    array<double, 3> ponto_A_VP = Mviewport * (array<double, 4>{x1, y1, 0.0, 1.0});
                    array<double, 3> ponto_B_VP = Mviewport * (array<double, 4>{x2, y2, 0.0, 1.0});

                    Ponto2D_Tela pA = {ponto_A_VP[0], ponto_A_VP[1]};
                    Ponto2D_Tela pB = {ponto_B_VP[0], ponto_B_VP[1]};

                    desenho.pontos.push_back(pA);
                    desenho.pontos.push_back(pB);
                }
            }
        }

        objetosParaDesenhar.push_back(desenho);
    }

    return objetosParaDesenhar;
}

