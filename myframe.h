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

    QString getNome(){
        return nome;
    }
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

    Matriz escala(float x, float y){
        Matriz m(3, 3);

        m[0][0] = x; m[0][1] = 0; m[0][2] = 0;
        m[1][0] = 0; m[1][1] = y; m[1][2] = 0;
        m[2][0] = 0; m[2][1] = 0; m[2][2] = 1;

        return m;
    }

    Matriz translacao(float dx, float dy){
        Matriz m(3, 3);

        m[0][0] = 1; m[0][1] = 0; m[0][2] = dx;
        m[1][0] = 0; m[1][1] = 1; m[1][2] = dy;
        m[2][0] = 0; m[2][1] = 0; m[2][2] = 1;

        return m;
    }

    Matriz rotacao(float angulo){
        Matriz m(3, 3);

        angulo = angulo * M_PI / 180;
        m[0][0] = cos(angulo); m[0][1] = -sin(angulo); m[0][2] = 0;
        m[1][0] = sin(angulo); m[1][1] = cos(angulo); m[1][2] = 0;
        m[2][0] = 0; m[2][1] = 0; m[2][2] = 1;

        return m;
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

signals:
    void objAdicionado(QString nome);
};

#endif // MYFRAME_H
