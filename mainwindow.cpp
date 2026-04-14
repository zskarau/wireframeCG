#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::draw()
{
    int x = ui->inputX->value();
    int y = ui->inputY->value();

    QString tipo = ui->comboBox->currentText();

    MyFrame *frame = ui->frame;

    if (tipo == "Ponto")
    {
        frame->adicionarObjeto(new Ponto("p", "ponto", x, y));
    }

    else if (tipo == "Linha")
    {
        Ponto p1("", "", x, y);
        Ponto p2("", "", x + 50, y + 50);

        frame->adicionarObjeto(new Linha("l", "linha", p1, p2));
    }

    else if (tipo == "Triangulo")
    {
        std::list<Ponto*> pts;

        pts.push_back(new Ponto("", "", x, y));
        pts.push_back(new Ponto("", "", x + 50, y));
        pts.push_back(new Ponto("", "", x + 25, y - 50));

        frame->adicionarObjeto(new Poligono("tri", "poligono", pts));
    }

    else if (tipo == "Retangulo")
    {
        std::list<Ponto*> pts;

        pts.push_back(new Ponto("", "", x, y));
        pts.push_back(new Ponto("", "", x + 80, y));
        pts.push_back(new Ponto("", "", x + 80, y + 60));
        pts.push_back(new Ponto("", "", x, y + 60));

        frame->adicionarObjeto(new Poligono("ret", "poligono", pts));
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

        frame->adicionarObjeto(new Poligono("estrela", "poligono", pts));
    }

    else if (tipo == "Casinha")
    {
        // base
        std::list<Ponto*> base;

        base.push_back(new Ponto("", "", x, y));
        base.push_back(new Ponto("", "", x + 80, y));
        base.push_back(new Ponto("", "", x + 80, y + 60));
        base.push_back(new Ponto("", "", x, y + 60));

        frame->adicionarObjeto(new Poligono("base", "poligono", base));

        // telhado
        std::list<Ponto*> telhado;

        telhado.push_back(new Ponto("", "", x, y));
        telhado.push_back(new Ponto("", "", x + 40, y - 40));
        telhado.push_back(new Ponto("", "", x + 80, y));

        frame->adicionarObjeto(new Poligono("telhado", "poligono", telhado));
    }
}