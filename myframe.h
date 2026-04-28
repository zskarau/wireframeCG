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

    // operatorOverload
    Matriz operator *(Matriz &m){
        // construtor recebe primeiro o vetor de linhas, cada uma contem um vetor de colunas

        int qtdLinA = this->size(); // size conta quantas linhas
        int qtdColA = this->at(0).size(); // at(0) referencia pra primeira linha (size conta quantas colunas nessa linha)

        int qtdLinB = m.size();
        int qtdColB = m.at(0).size();

        // qtd de colunas da matriz A = qtd de linhas da matriz B
        if(qtdColA != qtdLinB)
            throw std::runtime_error("Dimensoes invalidas para multiplicacao");

        // matriz de resultados
        Matriz r(qtdLinA, qtdColB);

        for(int i = 0; i < qtdLinA; i++)
        {
            for(int j = 0; j < qtdColB; j++)
            {
                r[i][j] = 0;

                for(int k = 0; k < qtdColA; k++)
                {
                    r[i][j] += (*this)[i][k] * m[k][j];
                }
            }
        }
        return r;
    }
};

class Ponto : public Objeto , public Matriz
{
public:
    Ponto(QString n, QString t, float x, float y);
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
