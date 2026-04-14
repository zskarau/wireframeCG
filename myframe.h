#ifndef MYFRAME_H
#define MYFRAME_H

#include <QDebug>
#include <QFrame>
#include <QPoint>
#include <QWidget>

class MyPoint{
private:
    int x, y, z;

public:
    MyPoint(int x, int y);

    int getX(){
        return x;
    }

    int getY(){
        return y;
    }

    void desenharPonto(QPainter *painter);
};

class MyLine{
private:
    MyPoint i, f;

public:
    MyLine(MyPoint i, MyPoint f);
    void desenharLinha(QPainter *painter);
};

class MyFrame : public QFrame
{
    Q_OBJECT
public:
    MyFrame(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;

private:
    bool flag;
    MyPoint ponto = MyPoint(100, 100);
    MyLine linha = MyLine(MyPoint(100, 200), MyPoint(400, 200));

public slots:
    void mostrar();
    void apagar();
};

#endif // MYFRAME_H
