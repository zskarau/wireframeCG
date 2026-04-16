#ifndef MYFRAME_H
#define MYFRAME_H

#include <QDebug>
#include <QFrame>
#include <QPoint>
#include <QWidget>
#include <QString>
#include <list>
#include <vector>

class Objeto
{
public:
    QString nome, tipo;
    Objeto(QString n, QString t);

    virtual void desenhar(QPainter *painter) = 0;
};

class Matriz : public std::vector<std::vector<float>>{
public:
    Matriz(int l, int c) : vector(l, vector<float>(c)){}

    Matriz operator*(Matriz n){} // operatorOverload
};

class Ponto : public Objeto , public Matriz
{
public:
    Ponto(QString n, QString t,float x, float y):
        Objeto(n,t),Matriz(3,1)
    {
        (*this)[0][0]=x;
        (*this)[1][0]=y;
        (*this)[2][0]=1;

    }
    void desenhar(QPainter *painter) override;

    int getX(){
        return (*this)[0][0];
    }

    int getY(){
        return (*this)[1][0];
    }

};

class Linha : public Objeto
{
public:
    Ponto p1, p2;

    Linha(QString n, QString t, Ponto p1, Ponto p2);
    void desenhar(QPainter *painter) override;
};

class Poligono : public Objeto
{
public:
    Poligono(const QString &n, const QString &t);
    std::list<Ponto *> listaPontos;
    void desenhar(QPainter *painter) override;

    Poligono(QString n, QString t, std::list<Ponto *> l);
};

class MyFrame : public QFrame
{
    Q_OBJECT
public:
    MyFrame(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
    void adicionarObjeto(Objeto *obj);

    std::list<Objeto *> displayFile;
};

#endif // MYFRAME_H
