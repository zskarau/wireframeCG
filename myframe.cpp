#include "myframe.h"
#include <QPainter>
#include <QPaintEvent>

MyFrame::MyFrame(QWidget *parent) : QFrame(parent)
{}

void MyFrame::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter painter(this);
    painter.setPen(Qt::red);

    for(Objeto* obj : displayFile)
    {
        obj->desenhar(&painter);
    }
}

void MyFrame::adicionarObjeto(Objeto *obj)
{
    displayFile.push_back(obj);
    update();
}

Objeto::Objeto(QString n, QString t) : nome(n), tipo(t){}

Ponto::Ponto(QString n, QString t, int x, int y) : Objeto(n, t), x(x), y(y){}

void Ponto::desenhar(QPainter *painter)
{
    painter->drawPoint(x, y);
}

Linha::Linha(QString n, QString t, Ponto p1, Ponto p2) : Objeto(n, t), p1(p1), p2(p2){}

void Linha::desenhar(QPainter *painter)
{
    painter->drawLine(p1.x, p1.y, p2.x, p2.y);
}

Poligono::Poligono(QString n, QString t, std::list<Ponto *> l) : Objeto(n, t), listaPontos(l){}

void Poligono::desenhar(QPainter *painter)
{
    if (listaPontos.size() < 2) return;

    auto i = listaPontos.begin();
    Ponto* primeiro = *i;
    Ponto* anterior = primeiro;

    ++i;

    for (; i != listaPontos.end(); ++i)
    {
        Ponto* atual = *i;
        painter->drawLine(anterior->x, anterior->y, atual->x, atual->y);
        anterior = atual;
    }

    // Fecha o polígono
    painter->drawLine(anterior->x, anterior->y, primeiro->x, primeiro->y);
}
