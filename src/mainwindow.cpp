#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QBoxLayout>
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

    QByteArray conteudo = file.readAll();
    std::istringstream stream(conteudo.toStdString());

    std::vector<array<double,4>> vertices;
    std::vector<std::pair<int,int>> arestas;

    std::string linha;

    while (std::getline(stream, linha)) {
        std::istringstream iss(linha);
        std::string tipo;
        iss >> tipo;

        if (tipo == "v") {
            double x, y, z;
            iss >> x >> y >> z;
            vertices.push_back({x, y, z, 1.0});
        }
        else if (tipo == "f") {
            std::vector<int> indicesFace;
            std::string indice;

            while (iss >> indice) {
                size_t slash = indice.find('/');
                if (slash != std::string::npos)
                    indice = indice.substr(0, slash);

                int idx = std::stoi(indice) - 1;
                if (idx >= 0 && idx < (int)vertices.size())
                    indicesFace.push_back(idx);
            }

            // Criar arestas conectando todos os vértices da face
            for (int i = 0; i < (int)indicesFace.size(); i++) {
                int j = (i+1) % indicesFace.size();
                arestas.emplace_back(indicesFace[i], indicesFace[j]);
            }
        }
    }

    // Criar objeto unico OBJ
    auto obj = std::make_unique<ObjetoOBJ>(
        caminho.toStdString(),
        vertices,
        arestas
    );

    display.adicionarObjeto(std::move(obj));

    qInfo() << "OBJ carregado:"
            << vertices.size() << "vértices,"
            << arestas.size() << "arestas.";
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Visualizador 3D");

    // ----- CENTRAL WIDGET -----
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    // ----- AREA DE DESENHO -----
    areaDesenho = new WidgetDesenho(displayFile, this);
    areaDesenho->setObjetoSelecionado(-1);

    // ----- LISTA LATERAL -----
    listaObjetos = new QListWidget(this);
    connect(listaObjetos, &QListWidget::itemClicked,
            this, &MainWindow::onObjetoSelecionado);

    // ----- LAYOUT HORIZONTAL -----
    QHBoxLayout* layoutPrincipal = new QHBoxLayout();
    layoutPrincipal->addWidget(listaObjetos, 1);
    layoutPrincipal->addWidget(areaDesenho, 4);

    central->setLayout(layoutPrincipal);

    // ----- MENUS -----
    QMenu *menuArquivo = menuBar()->addMenu("Arquivo");

    QAction *acaoCarregarOBJ = new QAction("Carregar OBJ...", this);
    menuArquivo->addAction(acaoCarregarOBJ);

    QAction *acaoRemoverOBJ = new QAction("Remover objeto selecionado", this);
    menuArquivo->addAction(acaoRemoverOBJ);

    connect(acaoRemoverOBJ, &QAction::triggered,
            this, &MainWindow::onRemoverOBJ);

    connect(acaoCarregarOBJ, &QAction::triggered,
            this, &MainWindow::onCarregarOBJ);

    QMenu *menuCamera = menuBar()->addMenu("Câmera");

    QAction *acaoResetCamera = new QAction("Resetar câmera", this);
    menuCamera->addAction(acaoResetCamera);

    connect(acaoResetCamera, &QAction::triggered,
            this, &MainWindow::onResetCamera);

    resize(900, 700);
}

void MainWindow::onCarregarOBJ()
{
    QString caminho = QFileDialog::getOpenFileName(
        this,
        "Selecione o arquivo OBJ",
        "",
        "Arquivos OBJ (*.obj)"
    );

    if (caminho.isEmpty())
        return;

    try {
        carregarOBJ(caminho, *(areaDesenho->visualizador.displayFile));
        listaObjetos->addItem(caminho);
        areaDesenho->update();
    }
    catch (const std::exception &e) {
        QMessageBox::critical(this, "Erro ao carregar", e.what());
    }
}

void MainWindow::onRemoverOBJ()
{
    int idx = areaDesenho->getObjetoSelecionado();
    if (idx < 0) {
        QMessageBox::warning(this, "Remover", "Nenhum objeto selecionado.");
        return;
    }

    // Remove do DisplayFile
    displayFile.objetos.erase(displayFile.objetos.begin() + idx);

    // Remove da lista
    delete listaObjetos->takeItem(idx);

    // Reset seleção
    areaDesenho->setObjetoSelecionado(-1);
    listaObjetos->clearSelection();

    // Atualiza desenho
    areaDesenho->update();
}


void MainWindow::onResetCamera()
{
    areaDesenho->visualizador.window.resetarCamera();
    areaDesenho->update();
}

void MainWindow::onObjetoSelecionado(QListWidgetItem* item)
{
    areaDesenho->setObjetoSelecionado(listaObjetos->row(item));
}

MainWindow::~MainWindow()
{
    delete ui;
}
