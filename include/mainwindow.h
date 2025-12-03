#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>

#include "displayfile.h"
#include "widgetdesenho.h"
#include "poligono.h"
#include "aresta.h"
#include "objetoobj.h"

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
    QListWidget* listaObjetos;

    MainWindow(QWidget *parent = nullptr);
    void onCarregarOBJ();
    void onRemoverOBJ();
    void onResetCamera();
    void onObjetoSelecionado(QListWidgetItem* item);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
