#include "widgetdesenho.h"

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

    // O Visualizador retorna os pontos já transformados em coordenadas de PIXEL
    std::vector<DesenhoObjeto> objetosParaDesenhar =
        this->visualizador.processarDisplayFileParaDesenho();

    const Viewport& vp = this->visualizador.viewport;

    // Configurações do Desenho
    int penWidth = 2;
    painter.setPen(QPen(Qt::red, penWidth, Qt::SolidLine));
    painter.setBrush(Qt::NoBrush);

    painter.drawRect(vp.xMin, vp.yMin, vp.xMax - vp.xMin, vp.yMax - vp.yMin);

    painter.setPen(Qt::black);

    // --- LOOP DE DESENHO ---
    for (const auto& desenho : objetosParaDesenhar) {

        const auto& pontos_tela = desenho.pontos;

        // Se o objeto tem pelo menos 2 pontos (Reta ou Polígono)
        if (pontos_tela.size() >= 2) {

            // Desenha as arestas
            for (size_t i = 0; i < pontos_tela.size() - 1; i++) {
                QPointF p1(pontos_tela[i].x, pontos_tela[i].y);
                QPointF p2(pontos_tela[i+1].x, pontos_tela[i+1].y);
                painter.drawLine(p1, p2);
            }

            // Se for um polígono, fechar a forma
            if (desenho.pontos.size() > 2) { // Lógica simples para identificar o Polígono
                QPointF pStart(pontos_tela.front().x, pontos_tela.front().y);
                QPointF pEnd(pontos_tela.back().x, pontos_tela.back().y);
                painter.drawLine(pEnd, pStart);
            }
        }

        // Se for um Ponto (desenha um círculo ou pixel)
        if (desenho.nome.find("Ponto") != std::string::npos && !pontos_tela.empty()) {
            QPointF p(pontos_tela[0].x, pontos_tela[0].y);
            painter.drawPoint(p);
        }
    }
}

void WidgetDesenho::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_W: visualizador.moverCamera(0, 0, -0.5); break; // Frente
        case Qt::Key_S: visualizador.moverCamera(0, 0,  0.5); break; // Trás
        case Qt::Key_A: visualizador.moverCamera(-0.5, 0, 0); break; // Esquerda
        case Qt::Key_D: visualizador.moverCamera(0.5, 0, 0); break;  // Direita
        case Qt::Key_R: visualizador.moverCamera(0, 0.5, 0); break;  // Subir
        case Qt::Key_F: visualizador.moverCamera(0, -0.5, 0); break; // Descer

        case Qt::Key_Q: visualizador.rotacionarCameraY(-5); break;   // Gira esquerda
        case Qt::Key_E: visualizador.rotacionarCameraY(5); break;    // Gira direita
        case Qt::Key_Up: visualizador.rotacionarCameraX(-5); break;  // Gira para cima
        case Qt::Key_Down: visualizador.rotacionarCameraX(5); break; // Gira para baixo

        case Qt::Key_Z: visualizador.zoomCamera(0.9); break;         // Zoom in
        case Qt::Key_X: visualizador.zoomCamera(1.1); break;         // Zoom out

        case Qt::Key_P: visualizador.alternarProjecao(); break;
    }
    this->update();
}


