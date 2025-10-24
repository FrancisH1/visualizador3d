#include "widgetdesenho.h"

WidgetDesenho::WidgetDesenho(DisplayFile& df, QWidget *parent)
    : QWidget(parent),
    displayFile(df),
    // Cria a Window inicial
    visualizador(&df, Window(-100, -100, 100, 100),
                 // A Viewport inicial usa o tamanho atual do widget
                 Viewport(50, 50, this->width() - 50, this->height() - 50))
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
void WidgetDesenho::moverWindow(double dx, double dy) {
    this->visualizador.moverWindow(dx, dy);
    this->redesenhar();
}

void WidgetDesenho::zoomWindow(double fator) {
    this->visualizador.zoomWindow(fator);
    this->redesenhar();
}

void WidgetDesenho::rotacionarWindow(double anguloGrau) {
    this->visualizador.rotacionarWindow(anguloGrau);
    this->redesenhar();
}

void WidgetDesenho::resizeEvent(QResizeEvent *event) {
    // Obtém as novas dimensões do widget
    int novoW = this->width();
    int novoH = this->height();

    // Atualiza a Viewport dentro do Visualizador
    this->visualizador.viewport.setLimites(0, 0, novoW, novoH);

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

    painter.drawRect(vp.xMin + 20, vp.yMin + 20, vp.xMax - vp.xMin - 40, vp.yMax - vp.yMin - 40);

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
            if (desenho.nome.find("Poligono")) { // Lógica simples para identificar o Polígono
                QPointF pStart(pontos_tela.back().x, pontos_tela.back().y);
                QPointF pEnd(pontos_tela.front().x, pontos_tela.front().y);
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
    double velocidade_translacao = 10.0; // Movimento em unidades de mundo
    double fator_zoom = 1.1;             // 1.1 para zoom in, 1/1.1 para zoom out

    bool window_modificada = false;

    // Acessa o objeto Window (assumindo que ele está dentro do Visualizador)
    Window& w = this->visualizador.window;

    switch (event->key()) {

    // ------------------------------------
    // 1. Translação - Setas
    // ------------------------------------
    case Qt::Key_Left:
        w.mover(-velocidade_translacao, 0.0);
        window_modificada = true;
        break;
    case Qt::Key_Right:
        w.mover(velocidade_translacao, 0.0);
        window_modificada = true;
        break;
    case Qt::Key_Up:
        w.mover(0.0, velocidade_translacao);
        window_modificada = true;
        break;
    case Qt::Key_Down:
        w.mover(0.0, -velocidade_translacao);
        window_modificada = true;
        break;

    // ------------------------------------
    // 2. Escala (Zoom) - Teclas 'I' e 'O'
    // ------------------------------------
    case Qt::Key_I: // I de IN (Zoom In)
        w.zoom(fator_zoom); // Reduz os limites da Window (Zoom In)
        window_modificada = true;
        break;
    case Qt::Key_O: // O de OUT (Zoom Out)
        w.zoom(1.0 / fator_zoom);      // Aumenta os limites da Window (Zoom Out)
        window_modificada = true;
        break;

    default:
        // Deixa o evento ser processado pela classe base (QWidget)
        QWidget::keyPressEvent(event);
        return;
    }

    if (window_modificada) {
        // Recalcular o SCN e a Viewport
        this->visualizador.recalcularPipeline();

        // Força o redesenho da tela
        this->update();
    }
}
