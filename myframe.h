#ifndef MYFRAME_H
#define MYFRAME_H

#include <iostream>
#include <ostream>
#include <QFrame>
#include <QPoint>
#include <QWidget>

class MyPoint : public QPoint{
private:
    int x, y;

public:
    MyPoint(int x, int y);

    get_x(){
        return x;
    }

    get_y(){
        return y;
    }

    void desenharPonto(QPainter *painter);
};

class MyLine{
private:
    MyPoint i, f;

public:
    MyLine(MyPoint i, MyPoint f);
    std::cout << i.get_x() << i.get_y();
    void desenharLinha(QPainter *painter);
};

class MyFrame : public QFrame
{
    Q_OBJECT
public:
    MyFrame(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;

private:
    MyPoint ponto = MyPoint(100, 100);
    MyLine linha = MyLine(MyPoint(100, 200), MyPoint(400, 200));
};

#endif // MYFRAME_H
