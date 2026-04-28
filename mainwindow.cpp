#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->frame, &MyFrame::objAdicionado,
            this, &MainWindow::receberObjetoAdicionado);
    connect(ui->btnScale, &QPushButton::clicked,
            this, &MainWindow::aplicarEscala);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::receberObjetoAdicionado(QString nome)
{
    ui->comboBoxObj->addItem(nome);
}

void MainWindow::aplicarEscala()
{
    int indiceObjeto = ui->comboBoxObj->currentIndex();
    float escalaX = static_cast<float>(ui->inputScaleX->value());
    float escalaY = static_cast<float>(ui->inputScaleY->value());

    ui->frame->escalarObjeto(indiceObjeto, escalaX, escalaY);
}

void MainWindow::draw()
{
    int x = ui->inputX->value();
    int y = ui->inputY->value();

    QString tipo = ui->comboBox->currentText();

    MyFrame *frame = ui->frame;

    if (tipo == "Ponto")
    {
        QString resultado = tipo + QString::number(contadores[tipo]++);
        frame->adicionarObjeto(new Ponto(resultado, tipo, x, y));
    }

    else if (tipo == "Linha")
    {
        Ponto p1("", "", x, y);
        Ponto p2("", "", x + 50, y + 50);
        QString resultado = tipo + QString::number(contadores[tipo]++);
        frame->adicionarObjeto(new Linha(resultado, tipo, p1, p2));
    }

    else if (tipo == "Triangulo")
    {
        std::list<Ponto*> pts;
        pts.push_back(new Ponto("", "", x, y));
        pts.push_back(new Ponto("", "", x + 50, y));
        pts.push_back(new Ponto("", "", x + 25, y - 50));
        QString resultado = tipo + QString::number(contadores[tipo]++);
        frame->adicionarObjeto(new Poligono(resultado, tipo, pts));
    }

    else if (tipo == "Retangulo")
    {
        std::list<Ponto*> pts;
        pts.push_back(new Ponto("", "", x, y));
        pts.push_back(new Ponto("", "", x + 80, y));
        pts.push_back(new Ponto("", "", x + 80, y + 60));
        pts.push_back(new Ponto("", "", x, y + 60));
        QString resultado = tipo + QString::number(contadores[tipo]++);
        frame->adicionarObjeto(new Poligono(resultado, tipo, pts));
    }

    else if (tipo == "Estrela")
    {
        std::list<Ponto*> pts;

        int r1 = 40, r2 = 20;

        for (int i = 0; i < 10; i++)
        {
            double ang = i * M_PI / 5.0;
            int r = (i % 2 == 0) ? r1 : r2;

            int px = x + r * cos(ang);
            int py = y + r * sin(ang);

            pts.push_back(new Ponto("", "", px, py));
        }
        QString resultado = tipo + QString::number(contadores[tipo]++);
        frame->adicionarObjeto(new Poligono(resultado, tipo, pts));
    }

    else if (tipo == "Casinha")
    {
        // base
        std::list<Ponto*> base;
        base.push_back(new Ponto("", "", x, y));
        base.push_back(new Ponto("", "", x + 80, y));
        base.push_back(new Ponto("", "", x + 80, y + 60));
        base.push_back(new Ponto("", "", x, y + 60));
        QString resultado = tipo + QString::number(contadores[tipo]++);
        frame->adicionarObjeto(new Poligono(resultado, tipo, base));

        // telhado
        std::list<Ponto*> telhado;

        telhado.push_back(new Ponto("", "", x, y));
        telhado.push_back(new Ponto("", "", x + 40, y - 40));
        telhado.push_back(new Ponto("", "", x + 80, y));
        QString resultado2 = tipo + QString::number(contadores[tipo]);
        frame->adicionarObjeto(new Poligono(resultado2, tipo, telhado));
    }
}
