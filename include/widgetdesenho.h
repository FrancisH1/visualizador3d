#ifndef WIDGETDESENHO_H
#define WIDGETDESENHO_H

#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include "window.h"
#include "viewport.h"
#include "displayfile.h"
#include "visualizador3d.h"

class WidgetDesenho : public QWidget
{
    Q_OBJECT
public:
    Visualizador3d visualizador;

    DisplayFile& displayFile;

    int objetoSelecionado;

    WidgetDesenho(DisplayFile& df, QWidget *parent = nullptr);

    void keyPressEvent(QKeyEvent *event) override;

    void setObjetoSelecionado(int idx);

    // Sinaliza ao Qt que o widget precisa ser redesenhado
    void redesenhar();

    // Métodos de acesso para a navegação (chamados pela MainWindow)
    void moverCamera(double dx, double dy, double dz);
    void zoomCamera(double fator);
    void rotacionarCameraY(double anguloGrau);

    // Sobrescreve o evento de resize para manter a Viewport correta
    void resizeEvent(QResizeEvent *event) override;

    void paintEvent(QPaintEvent *event) override;
};

#endif // WIDGETDESENHO_H
