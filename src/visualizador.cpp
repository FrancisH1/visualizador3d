#include "visualizador.h"

void Visualizador::recalcularPipeline() {
    // 1. Obter a Matriz de Normalização (SCN)
    Matriz3x3 M_Normalizacao = this->window.getMatrizNormalizacao();

    // 2. Obter a Matriz da Viewport (SCN -> Pixels)
    Matriz3x3 M_Viewport = this->viewport.getMatrizViewport();

    // 3. Composição (M_Final = M_Viewport * M_Normalizacao)
    this->matrizTransformacaoFinal = M_Viewport * M_Normalizacao;
}

Visualizador::Visualizador(DisplayFile* df, const Window& w, const Viewport& vp)
    : window(w), viewport(vp), displayFile(df) {

    // Inicializa a matriz de transformação com a composição inicial
    this->recalcularPipeline();
}

void Visualizador::moverWindow(double dx, double dy) {
    this->window.mover(dx, dy);
    this->recalcularPipeline();
}

void Visualizador::zoomWindow(double fator) {
    this->window.zoom(fator);
    this->recalcularPipeline();
}

void Visualizador::rotacionarWindow(double anguloGrau) {
    this->window.rotacionar(anguloGrau);
    this->recalcularPipeline();
}

std::vector<DesenhoObjeto> Visualizador::processarDisplayFileParaDesenho() {
    std::vector<DesenhoObjeto> objetosParaDesenhar;

    const Matriz3x3& M_Final = this->matrizTransformacaoFinal;

    for (const auto& objeto_ptr : this->displayFile->objetos) {
        DesenhoObjeto desenho;
        desenho.nome = objeto_ptr->getNome();

        // 1. Obter os pontos do objeto em Coordenadas de Mundo
        const std::vector<std::array<double, 3>>& pontosMundo = objeto_ptr->getPontos();

        // Itera sobre as arestas do polígono (Ponto I ao Ponto I+1)
        for (size_t i = 0; i < pontosMundo.size(); ++i) {

            // Pontos A e B no SCM (Normalized Coordinate System: [-1, 1] x [-1, 1])
            std::array<double, 3> ponto_A_SCN = this->window.getMatrizNormalizacao() * pontosMundo[i];
            std::array<double, 3> ponto_B_SCN;

            // Para polígonos, a aresta de fechamento é do último ponto para o primeiro.
            size_t next_i = (i + 1) % pontosMundo.size();
            ponto_B_SCN = this->window.getMatrizNormalizacao() * pontosMundo[next_i];

            // --- ETAPA DE CLIPPING (Cohen-Sutherland) ---

            // Pega as coordenadas X e Y, que serão cortadas por referência (modificadas)
            double x1 = ponto_A_SCN[0];
            double y1 = ponto_A_SCN[1];
            double x2 = ponto_B_SCN[0];
            double y2 = ponto_B_SCN[1];

            // 4. Recorta a linha. Se retornar true, a linha (ou o segmento resultante) é visível.
            if (clipparReta(x1, y1, x2, y2)) {

                // 5. Se aceita, aplica a Transformação de Viewport (M_Viewport)
                // Os novos (x1, y1, 1.0) e (x2, y2, 1.0) estão no SCN e agora serão mapeados para a tela.

                // Ponto A Viewport
                std::array<double, 3> ponto_A_VP = this->viewport.getMatrizViewport() * (std::array<double, 3>{x1, y1, 1.0});

                // Ponto B Viewport
                std::array<double, 3> ponto_B_VP = this->viewport.getMatrizViewport() * (std::array<double, 3>{x2, y2, 1.0});

                // 6. Converte para Coordenadas 2D de Tela (Pixel)
                Ponto2D_Tela pA = {ponto_A_VP[0], ponto_A_VP[1]};
                Ponto2D_Tela pB = {ponto_B_VP[0], ponto_B_VP[1]};

                // Adiciona os dois pontos (pA e pB) para desenhar a linha.
                desenho.pontos.push_back(pA);
                desenho.pontos.push_back(pB);

                // Se o objeto for um POLÍGONO, você deve fazer uma lógica mais complexa
                // para garantir que os pontos de intersecção se liguem, mas para
                // o Cohen-Sutherland de linha (simples), isso basta.
            }
        }

        objetosParaDesenhar.push_back(desenho);
    }

    return objetosParaDesenhar;
}
