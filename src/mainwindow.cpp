#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    std::initializer_list<std::pair<double, double>> vertices_quadrado =
        { {10,50}, {10,100}, {60,100}, {60,50} };

    displayFile.adicionarObjeto(std::make_unique<Poligono>("quadrado", vertices_quadrado));

    Ponto p1(0,0), p2(400,240);

    displayFile.adicionarObjeto(std::make_unique<Aresta>(p1, p2));

    areaDesenho = new WidgetDesenho(displayFile, this);


    setCentralWidget(areaDesenho);
}

MainWindow::~MainWindow()
{
    delete ui;
}
