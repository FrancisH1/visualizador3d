#ifndef VISUALIZADOR_H
#define VISUALIZADOR_H

#include "displayfile.h"
#include "window.h"
#include "viewport.h"
#include "matriz3x3.h"
#include "clipping.h"

// Define um tipo de retorno para os pontos prontos para o desenho (pixels)
struct Ponto2D_Tela {
    double x;
    double y;
};

// Estrutura para retorno de todos os pontos de um objeto
struct DesenhoObjeto {
    string nome;
    vector<Ponto2D_Tela> pontos;
};

class Visualizador {
public:
    // Componentes do Pipeline de Visualização
    Window window;
    Viewport viewport;

    // Armazena a matriz final M_Final = M_Viewport * M_Normalizacao
    Matriz3x3 matrizTransformacaoFinal;

    // Referência ao DisplayFile, que contém todos os objetos da cena
    DisplayFile* displayFile;

    // Método para atualizar a Matriz Final
    void recalcularPipeline();

    // Construtor: Inicializa a Window, Viewport e recebe o DisplayFile
    Visualizador(DisplayFile* df, const Window& w, const Viewport& vp);

    // Métodos para navegação da Window que exigem recálculo
    void moverWindow(double dx, double dy);
    void zoomWindow(double fator);
    void rotacionarWindow(double anguloGrau);

    // Método principal de renderização/processamento
    vector<DesenhoObjeto> processarDisplayFileParaDesenho();

};

#endif // VISUALIZADOR_H
