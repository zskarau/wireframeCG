#ifndef MYFRAME_H
#define MYFRAME_H

#include <QDebug>
#include <QFrame>
#include <QPoint>
#include <QWidget>
#include <QString>
#include <list>

class Objeto
{
public:
    QString nome, tipo;
    Objeto(QString n, QString t) : nome(n), tipo(t){}

    virtual void desenhar(QPainter *painter) = 0;
};

class Ponto : Objeto
{
public:
    int x, y;
    Ponto(QString n, QString t, int x, int y) : Objeto(n, t), x(x), y(y){}

    void desenhar(QPainter *painter) override;
};

class Linha : Objeto
{
public:
    Ponto p1, p2;

    Linha(QString n, QString t, Ponto p1, Ponto p2) : Objeto(n, t), p1(p1), p2(p2){}
    void desenhar(QPainter *painter) override;
};

class MyFrame : public QFrame
{
    Q_OBJECT
public:
    MyFrame(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;

    list<Objeto *> displayFile;

private:
    bool flag;
    MyPoint ponto = MyPoint(100, 100);
    MyLine linha = MyLine(MyPoint(100, 200), MyPoint(400, 200));

public slots:
    void mostrar();
    void apagar();
};

#endif // MYFRAME_H
