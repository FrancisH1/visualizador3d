#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "displayfile.h"
#include "widgetdesenho.h"
#include "poligono.h"
#include "aresta.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    DisplayFile displayFile;
    WidgetDesenho* areaDesenho;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
