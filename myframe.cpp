#include "myframe.h"
#include <QPainter>
#include <QPaintEvent>
#include <iterator>

MyFrame::MyFrame(QWidget *parent) : QFrame(parent)
{}

void MyFrame::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter painter(this);
    painter.setPen(Qt::red);

    for(Objeto* obj : displayFile)
    {
        obj->desenhar(&painter);
    }
}

void MyFrame::adicionarObjeto(Objeto *obj)
{
    displayFile.push_back(obj);
    emit objAdicionado(obj->getNome());
    update();
}

void Poligono::aplicarTransformacao(const Matriz &transformacao)
{
    // para todos os pontos do poligono aplique a transformacao
    for (Ponto *ponto : listaPontos)
    {
        ponto->aplicarTransformacao(transformacao);
    }
}

void MyFrame::escalarObjeto(int indice, float escalaX, float escalaY)
{
    // encontrar o objeto que foi solicitado para transformacao (indice do comboBox é o mesmo do df)
    // auto it para detectar qual o tipo do objeto a ser tratado
    auto objeto = displayFile.begin();
    std::advance(objeto, indice);

    QPointF centro = (*objeto)->getCentro();

    // levar o centro do objeto para a origem, aplicar escala e voltar para o ponto original
    Matriz translacaoParaOrigem = Matriz(3, 3).translacao(-centro.x(), -centro.y());
    Matriz matrizEscala = Matriz(3, 3).escala(escalaX, escalaY);
    Matriz translacaoDeVolta = Matriz(3, 3).translacao(centro.x(), centro.y());
    // operacoes a serem feitas primeiro são colocadas por ultimo
    // exemplo de composicao de matrizes com operatorOverload
    Matriz transformacao = translacaoDeVolta * (matrizEscala * translacaoParaOrigem);

    // aplicar transformacao ao poligono, linha ou ponto
    (*objeto)->aplicarTransformacao(transformacao);
    update();
}

Objeto::Objeto(QString n, QString t) : nome(n), tipo(t){}

Ponto::Ponto(QString n, QString t, float x, float y):
    Objeto(n, t), Matriz(3, 1)
{
    (*this)[0][0] = x;
    (*this)[1][0] = y;
    (*this)[2][0] = 1;
}

void Ponto::desenhar(QPainter *painter)
{
    painter->drawPoint(getX(), getY());
}

void Ponto::aplicarTransformacao(const Matriz &transformacao)
{
    // transformacao em um unico ponto
    Matriz pontoAtual = *this;
    Matriz resultado = transformacao * pontoAtual;

    (*this)[0][0] = resultado[0][0];
    (*this)[1][0] = resultado[1][0];
    (*this)[2][0] = resultado[2][0];
}

QPointF Ponto::getCentro() const
{
    return QPointF(getXF(), getYF());
}

Linha::Linha(QString n, QString t, Ponto p1, Ponto p2) : Objeto(n, t), p1(p1), p2(p2){}

void Linha::desenhar(QPainter *painter)
{
    painter->drawLine(p1.getX(), p1.getY(), p2.getX(), p2.getY());
}

void Linha::aplicarTransformacao(const Matriz &transformacao)
{
    // aplicar transformacao nos dois pontos que compoem a linha
    p1.aplicarTransformacao(transformacao);
    p2.aplicarTransformacao(transformacao);
}

QPointF Linha::getCentro() const
{
    return QPointF((p1.getXF() + p2.getXF()) / 2.0f,
                   (p1.getYF() + p2.getYF()) / 2.0f);
}

Poligono::Poligono(QString n, QString t, std::list<Ponto *> l) : Objeto(n, t), listaPontos(l){}

QPointF Poligono::getCentro() const
{
    if (listaPontos.empty())
    {
        return QPointF();
    }

    // fazer media aritmetica dos vertices
    float somaX = 0.0;
    float somaY = 0.0;
    int quantidade = 0;

    // percorre todos os pontos da lista de pontos do poligono
    for (const Ponto *ponto : listaPontos)
    {
        somaX += ponto->getXF();
        somaY += ponto->getYF();
        quantidade++;
    }

    // centro do objeto feito com a media dos pontos
    return QPointF(somaX / quantidade, somaY / quantidade);
}

void Poligono::desenhar(QPainter *painter)
{
    if (listaPontos.size() < 2) return;

    auto i = listaPontos.begin();
    Ponto* primeiro = *i;
    Ponto* anterior = primeiro;

    ++i;

    for (; i != listaPontos.end(); ++i)
    {
        Ponto* atual = *i;
        painter->drawLine(anterior->getX(), anterior->getY(), atual->getX(), atual->getY());
        anterior = atual;
    }

    // Fecha o polígono
    painter->drawLine(anterior->getX(), anterior->getY(), primeiro->getX(), primeiro->getY());
}

// casinha tratada como objeto unico (uniao da base com telhado)
Casinha::Casinha(QString n, QString t, Poligono *b, Poligono *t2)
    : Objeto(n, t), base(b), telhado(t2) {}

void Casinha::desenhar(QPainter *painter)
{
    base->desenhar(painter);
    telhado->desenhar(painter);
}

void Casinha::aplicarTransformacao(const Matriz &transformacao)
{
    base->aplicarTransformacao(transformacao);
    telhado->aplicarTransformacao(transformacao);
}

QPointF Casinha::getCentro() const
{
    QPointF c1 = base->getCentro();
    QPointF c2 = telhado->getCentro();

    return QPointF((c1.x() + c2.x()) / 2.0,
                   (c1.y() + c2.y()) / 2.0);
}
