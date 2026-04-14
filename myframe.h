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
    Objeto(QString n, QString t);

    virtual void desenhar(QPainter *painter) = 0;
};

class Ponto : public Objeto
{
public:
    int x, y;
    Ponto(QString n, QString t, int x, int y);

    void desenhar(QPainter *painter) override;
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
