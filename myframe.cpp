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
    for (Ponto *ponto : listaPontos)
    {
        if (ponto != nullptr)
        {
            ponto->aplicarTransformacao(transformacao);
        }
    }
}

bool MyFrame::escalarObjeto(int indice, float escalaX, float escalaY)
{
    auto it = displayFile.begin();
    std::advance(it, indice);

    QPointF centro = (*it)->getCentro();

    Matriz translacaoParaOrigem = Matriz(3, 3).translacao(-centro.x(), -centro.y());
    Matriz matrizEscala = Matriz(3, 3).escala(escalaX, escalaY);
    Matriz translacaoDeVolta = Matriz(3, 3).translacao(centro.x(), centro.y());
    Matriz transformacao = translacaoDeVolta * (matrizEscala * translacaoParaOrigem);

    (*it)->aplicarTransformacao(transformacao);
    update();

    return true;
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

    float somaX = 0.0f;
    float somaY = 0.0f;
    int quantidade = 0;

    for (const Ponto *ponto : listaPontos)
    {
        if (ponto != nullptr)
        {
            somaX += ponto->getXF();
            somaY += ponto->getYF();
            quantidade++;
        }
    }

    if (quantidade == 0)
    {
        return QPointF();
    }

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
