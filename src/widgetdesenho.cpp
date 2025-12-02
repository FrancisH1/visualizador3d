#include "widgetdesenho.h"
#include "mainwindow.h"

WidgetDesenho::WidgetDesenho(DisplayFile& df, QWidget *parent)
    : QWidget(parent),
    displayFile(df),
    // Cria a Window inicial
    visualizador(&df, Window3d(Ponto(0,0,10), Ponto(0,0,0), Ponto(0,1,0), -5, 5, -5, 5, -5, 5),
                 // A Viewport inicial usa o tamanho atual do widget
                 Viewport(0, 0, this->width(), this->height()))
{
    this->setFocusPolicy(Qt::StrongFocus); // Permite receber foco ao clicar
    this->setFocus();
    // Define um fundo para o widget
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::white);
    setPalette(pal);
}

void WidgetDesenho::redesenhar() {
    // A função 'update()' do QWidget solicita um paintEvent o mais rápido possível.
    this->update();
}

// Métodos de navegação (apenas delegam para o Visualizador e disparam o redesenho)
void WidgetDesenho::moverCamera(double dx, double dy, double dz) {
    this->visualizador.moverCamera(dx, dy, dz);
    this->redesenhar();
}

void WidgetDesenho::zoomCamera(double fator) {
    this->visualizador.zoomCamera(fator);
    this->redesenhar();
}

void WidgetDesenho::rotacionarCameraY(double anguloGrau) {
    this->visualizador.rotacionarCameraY(anguloGrau);
    this->redesenhar();
}

void WidgetDesenho::resizeEvent(QResizeEvent *event) {
    // Obtém as novas dimensões do widget
    int novoW = this->width();
    int novoH = this->height();

    // Atualiza a Viewport dentro do Visualizador
    this->visualizador.viewport.setLimites(20, 20, novoW - 20, novoH - 20);

    // Recalcula o pipeline para a nova Viewport
    this->visualizador.recalcularPipeline();

    // Chama a implementação padrão
    QWidget::resizeEvent(event);
}

void WidgetDesenho::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    // Objetos prontos para desenhar
    auto objetosParaDesenhar = visualizador.processarDisplayFileParaDesenho();
    const Viewport& vp = visualizador.viewport;

    // Desenha borda da viewport
    painter.setPen(QPen(Qt::red, 2));
    painter.drawRect(vp.xMin, vp.yMin, vp.xMax - vp.xMin, vp.yMax - vp.yMin);

    painter.setPen(Qt::black);

    // --- DESENHAR LINHAS ---
    for (const auto& desenho : objetosParaDesenhar) {

        // Os pontos vêm em pares: [pA, pB, pA, pB, ...]
        // Cada par representa uma aresta a ser desenhada
        for (size_t i = 0; i + 1 < desenho.pontos.size(); i += 2) {

            QPointF p1(desenho.pontos[i].x,     desenho.pontos[i].y);
            QPointF p2(desenho.pontos[i+1].x,   desenho.pontos[i+1].y);

            painter.drawLine(p1, p2);
        }
    }
}

void WidgetDesenho::setObjetoSelecionado(int idx) {
    this->objetoSelecionado = idx;
}

void WidgetDesenho::keyPressEvent(QKeyEvent *event) {
    
    // ESC para desselecionar objeto
    if (event->key() == Qt::Key_Escape) {
        this->objetoSelecionado = -1;
        update();
        return;
    }

    if (this->objetoSelecionado >= 0) {
        auto& obj = *displayFile.objetos[this->objetoSelecionado];
        switch (event->key()) {
            case Qt::Key_W: obj.aplicarTransformacao(Matriz4x4::translacao(0, 0.5, 0)); break;
            case Qt::Key_S: obj.aplicarTransformacao(Matriz4x4::translacao(0, -0.5, 0)); break;
            case Qt::Key_A: obj.aplicarTransformacao(Matriz4x4::translacao(-0.5, 0, 0)); break;
            case Qt::Key_D: obj.aplicarTransformacao(Matriz4x4::translacao(0.5, 0, 0)); break;
            

            case Qt::Key_Q: obj.aplicarTransformacao(Matriz4x4::rotacaoY(-5)); break;
            case Qt::Key_E: obj.aplicarTransformacao(Matriz4x4::rotacaoY(5)); break;
            case Qt::Key_Up: obj.aplicarTransformacao(Matriz4x4::rotacaoX(-5)); break;
            case Qt::Key_Down: obj.aplicarTransformacao(Matriz4x4::rotacaoX(5)); break;

            case Qt::Key_Z: obj.aplicarTransformacao(Matriz4x4::escala(0.9, 0.9, 0.9)); break;
            case Qt::Key_X: obj.aplicarTransformacao(Matriz4x4::escala(1.1, 1.1, 1.1)); break; 
        }
    } else {
        switch (event->key()) {
            case Qt::Key_W: visualizador.moverCamera(0, 0, 0.5); break; // Frente
            case Qt::Key_S: visualizador.moverCamera(0, 0, -0.5); break; // Trás
            case Qt::Key_A: visualizador.moverCamera(-0.5, 0, 0); break; // Esquerda
            case Qt::Key_D: visualizador.moverCamera(0.5, 0, 0); break;  // Direita
            case Qt::Key_R: visualizador.moverCamera(0, 0.5, 0); break;  // Subir
            case Qt::Key_F: visualizador.moverCamera(0, -0.5, 0); break; // Descer

            case Qt::Key_Q: visualizador.rotacionarCameraY(5); break;   // Gira esquerda
            case Qt::Key_E: visualizador.rotacionarCameraY(-5); break;    // Gira direita
            case Qt::Key_Up: visualizador.rotacionarCameraX(5); break;  // Gira para cima
            case Qt::Key_Down: visualizador.rotacionarCameraX(-5); break; // Gira para baixo

            case Qt::Key_Z: visualizador.zoomCamera(0.9); break;         // Zoom in
            case Qt::Key_X: visualizador.zoomCamera(1.1); break;         // Zoom out

            case Qt::Key_P: visualizador.alternarProjecao(); break;
        }
    }
    this->update();
}


