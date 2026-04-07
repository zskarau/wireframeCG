#include "myframe.h"
#include <QPainter>
#include <QPaintEvent>

MyFrame::MyFrame(QWidget *parent) : QFrame(parent)
{}

void MyFrame::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter painter(this);
    painter.setPen(Qt::black);

    ponto.desenharPonto(&painter);
    linha.desenharLinha(&painter);
}

MyPoint::MyPoint(int x, int y) : x(x), y(y){}

void MyPoint::desenharPonto(QPainter *painter)
{
    for(int i = 0; i < 10; i++){
        painter->drawPoint(x + i, y);
    }
}

MyLine::MyLine(MyPoint i, MyPoint f) : i(i), f(f){}

void MyLine::desenharLinha(QPainter *painter)
{
    painter->drawLine(i, f);
}