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
    emit objAdicionado(obj->getNome());
    update();
}

Objeto::Objeto(QString n, QString t) : nome(n), tipo(t){}

Ponto::Ponto(QString n, QString t, float x, float y):
    Objeto(n, t), Matriz(3, 1)
{
    (*this)[0][0] = x;
    (*this)[1][0] = y;
    (*this)[2][0] = 1;
}

void Ponto::desenhar(QPainter *painter)
{
    painter->drawPoint(getX(), getY());
}

Linha::Linha(QString n, QString t, Ponto p1, Ponto p2) : Objeto(n, t), p1(p1), p2(p2){}

void Linha::desenhar(QPainter *painter)
{
    painter->drawLine(p1.getX(), p1.getY(), p2.getX(), p2.getY());
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
        painter->drawLine(anterior->getX(), anterior->getY(), atual->getX(), atual->getY());
        anterior = atual;
    }

    // Fecha o polígono
    painter->drawLine(anterior->getX(), anterior->getY(), primeiro->getX(), primeiro->getY());
}
