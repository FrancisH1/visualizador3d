#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <sstream>

using std::make_unique;

void carregarOBJ(const QString& caminho, DisplayFile& display) {
    QFile file(caminho);
    if (!file.exists()) {
        qWarning() << "Arquivo não encontrado:" << caminho;
        return;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Erro ao abrir:" << file.errorString();
        return;
    }

    // Lê todo o conteúdo em memória
    QByteArray conteudo = file.readAll();
    std::istringstream stream(conteudo.toStdString());

    std::vector<Ponto> vertices;
    std::string linha;

    while (std::getline(stream, linha)) {
        std::istringstream iss(linha);
        std::string tipo;
        iss >> tipo;

        if (tipo == "v") {
            double x, y, z;
            iss >> x >> y >> z;
            vertices.emplace_back(x, y, z);
        }
        else if (tipo == "f") {
            std::vector<Ponto> pontosFace;
            std::string indice;
            while (iss >> indice) {
                // Lida com índices no formato "f a/b/c"
                size_t barra = indice.find('/');
                if (barra != std::string::npos)
                    indice = indice.substr(0, barra);

                int idx = std::stoi(indice) - 1; // .obj é 1-based
                if (idx >= 0 && idx < (int)vertices.size())
                    pontosFace.push_back(vertices[idx]);
            }

            if (pontosFace.size() >= 3) {
                std::string nomeFace = "Face_" + std::to_string(display.getObjetos().size());
                auto poligono = std::make_unique<Poligono>(nomeFace, pontosFace);
                display.adicionarObjeto(std::move(poligono));
            }
        }
    }

    qInfo() << "Arquivo OBJ carregado com sucesso!"
            << QString::number(vertices.size()) << "vértices.";
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    carregarOBJ("../../resources/Charizard/Charizard.obj", displayFile);
    carregarOBJ("../../resources/Umbreon/UmbreonLowPoly.obj", displayFile);

    areaDesenho = new WidgetDesenho(displayFile, this);

    setCentralWidget(areaDesenho);
}

MainWindow::~MainWindow()
{
    delete ui;
}
