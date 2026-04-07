#include "myframe.h"
#include <QPainter>
#include <QPaintEvent>

MyFrame::MyFrame(QWidget *parent) : QFrame(parent)
{
    flag = false;
}

void MyFrame::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter painter(this);
    painter.setPen(Qt::yellow);

    ponto.desenharPonto(&painter);
    if(flag){
        linha.desenharLinha(&painter);
    }
}

void MyFrame::mostrar()
{
    flag = true;
    update();
}

void MyFrame::apagar()
{
    flag = false;
    update();
}

MyPoint::MyPoint(int x, int y) : x(x), y(y){}

void MyPoint::desenharPonto(QPainter *painter)
{
    painter->drawPoint(x, y);
}

MyLine::MyLine(MyPoint i, MyPoint f) : i(i), f(f){}

void MyLine::desenharLinha(QPainter *painter)
{
    painter->drawLine(i.getX(), i.getY(), f.getX(), f.getY());
}