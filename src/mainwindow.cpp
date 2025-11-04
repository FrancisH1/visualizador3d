#include "mainwindow.h"
#include "ui_mainwindow.h"

using std::make_unique;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    vector<array<double, 4>> frente_vertices = {
        { -1, -1,  1, 1 },
        {  1, -1,  1, 1 },
        {  1,  1,  1, 1 },
        { -1,  1,  1, 1 }
    };

    vector<array<double, 4>> tras_vertices = {
        { -1, -1, -1, 1 },
        {  1, -1, -1, 1 },
        {  1,  1, -1, 1 },
        { -1,  1, -1, 1 }
    };

    vector<array<double, 4>> topo_vertices = {
        { -1, 1, -1, 1 },
        {  1, 1, -1, 1 },
        {  1, 1,  1, 1 },
        { -1, 1,  1, 1 }
    };

    vector<array<double, 4>> direita_vertices = {
        {  1, -1, -1, 1 },
        {  1, -1,  1, 1 },
        {  1,  1,  1, 1 },
        {  1,  1, -1, 1 }
    };

    vector<array<double, 4>> esquerda_vertices = {
        { -1, -1, -1, 1 },
        { -1, -1,  1, 1 },
        { -1,  1,  1, 1 },
        { -1,  1, -1, 1 }
    };

    vector<array<double, 4>> base_vertices = {
        { -1, -1, -1, 1 },
        {  1, -1, -1, 1 },
        {  1, -1,  1, 1 },
        { -1, -1,  1, 1 }
    };

    std::vector<std::unique_ptr<Objeto>> objetos;
    objetos.push_back(std::make_unique<Poligono>("Frente", frente_vertices));
    objetos.push_back(std::make_unique<Poligono>("Tras", tras_vertices));
    objetos.push_back(std::make_unique<Poligono>("Topo", topo_vertices));
    objetos.push_back(std::make_unique<Poligono>("Direita", direita_vertices));
    objetos.push_back(std::make_unique<Poligono>("Esquerda", esquerda_vertices));
    objetos.push_back(std::make_unique<Poligono>("Base", base_vertices));

    displayFile.adicionarObjetos(std::move(objetos));

    areaDesenho = new WidgetDesenho(displayFile, this);

    setCentralWidget(areaDesenho);
}

MainWindow::~MainWindow()
{
    delete ui;
}
