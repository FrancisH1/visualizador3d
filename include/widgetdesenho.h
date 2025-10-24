#ifndef WIDGETDESENHO_H
#define WIDGETDESENHO_H

#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include "window.h"
#include "viewport.h"
#include "displayfile.h"
#include "visualizador.h"

class WidgetDesenho : public QWidget
{
    Q_OBJECT
public:
    Visualizador visualizador;

    DisplayFile& displayFile;

    WidgetDesenho(DisplayFile& df, QWidget *parent = nullptr);

    void keyPressEvent(QKeyEvent *event) override;

    // Sinaliza ao Qt que o widget precisa ser redesenhado
    void redesenhar();

    // Métodos de acesso para a navegação (chamados pela MainWindow)
    void moverWindow(double dx, double dy);
    void zoomWindow(double fator);
    void rotacionarWindow(double anguloGrau);

    // Sobrescreve o evento de resize para manter a Viewport correta
    void resizeEvent(QResizeEvent *event) override;

    void paintEvent(QPaintEvent *event) override;
};

#endif // WIDGETDESENHO_H
