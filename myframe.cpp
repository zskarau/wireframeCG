#include "myframe.h"
#include <QPainter>
#include <QPaintEvent>

MyFrame::MyFrame(QWidget *parent) : QFrame(parent)
{
    linha = QLine(0, 0, 800, 620);
    retangulo = QRect(50, 50, 200, 100);
    flag = false;
    flagRect = false;

    setFocusPolicy(Qt::StrongFocus);
    setFocus();
}

void MyFrame::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter painter(this);
    painter.setPen(Qt::blue);

    if(flag){
        painter.drawLine(linha);
        flag = false;
    }

    if(flagRect){
        painter.setPen(QPen(Qt::black, 3));
        painter.setBrush(Qt::yellow);
        painter.drawRect(retangulo);

        flagRect = false;
    }
}

void MyFrame::keyPressEvent(QKeyEvent *keyEvent)
{
    switch(keyEvent->key()){
        case Qt::Key_W:
            retangulo = QRect(retangulo.x(), retangulo.y() - 10, retangulo.width(), retangulo.height());
            break;
        case Qt::Key_S:
            retangulo = QRect(retangulo.x(), retangulo.y() + 10, retangulo.width(), retangulo.height());
            break;
        case Qt::Key_A:
            retangulo = QRect(retangulo.x() - 10, retangulo.y(), retangulo.width(), retangulo.height());
            break;
        case Qt::Key_D:
            retangulo = QRect(retangulo.x() + 10, retangulo.y(), retangulo.width(), retangulo.height());
            break;
    }

    QRect area = contentsRect(); // contentsRect representa a área de desenho (no caso os limites do frame)

    // Limites horizontais
    if(retangulo.left() < area.left())
        retangulo.moveLeft(area.left());

    if(retangulo.right() > area.right())
        retangulo.moveRight(area.right());

    // Limites verticais
    if(retangulo.top() < area.top())
        retangulo.moveTop(area.top());

    if(retangulo.bottom() > area.bottom())
        retangulo.moveBottom(area.bottom());

    flagRect = true;
    update();
}

void MyFrame::mostrar()
{
    flag = true;
    update(); //update para atualizar o frame
}

void MyFrame::apagar()
{
    flag = false;
    flagRect = false;
    update();
}

void MyFrame::desenharRect()
{
    setFocus();
    flagRect = true;
    update();
}

