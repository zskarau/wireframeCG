#include "myframe.h"
#include <QPainter>
#include <QPaintEvent>
#include <iterator>

MyFrame::MyFrame(QWidget *parent) : QFrame(parent)
{
    Window *w =
        new Window(
            "window",
            800,
            600
            );

    displayFile.push_front(w);
}

void MyFrame::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter painter(this);

    if(displayFile.empty())
        return;

    Window *window =
        dynamic_cast<Window*>(
            displayFile.front()
            );

    if(!window)
        return;

    Matriz scn =
        window->gerarSCN();

    auto it = displayFile.begin();

    ++it;

    for(; it != displayFile.end(); ++it)
    {
        (*it)->desenhar(
            &painter,
            this,
            scn
            );
    }
}

void MyFrame::adicionarObjeto(Objeto *obj)
{
    displayFile.push_back(obj);
    emit objAdicionado(obj->getNome());
    update();
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

void MyFrame::transladarObjeto(int indice, float dx, float dy)
{
    // Encontra o objeto na lista
    auto objeto = displayFile.begin();
    std::advance(objeto, indice);

    // A translação é direta e não precisa ir para a origem
    Matriz matrizTranslacao = Matriz(3, 3).translacao(dx, dy);

    // Aplica a matriz e atualiza a tela
    (*objeto)->aplicarTransformacao(matrizTranslacao);
    update();
}

void MyFrame::rotacionarObjeto(int indice, float angulo, bool usarCentroide, int dx, int dy)
{
    // Encontra o objeto na lista
    auto objeto = displayFile.begin();
    std::advance(objeto, indice);

    Matriz transformacao = Matriz(3, 3);

    if (usarCentroide)
    {
        // rotacao no centro do objeto
        QPointF centro = (*objeto)->getCentro();

        Matriz translacaoParaOrigem = Matriz(3, 3).translacao(-centro.x(), -centro.y());
        Matriz matrizRotacao = Matriz(3, 3).rotacao(-angulo); // ajuste do sentido
        Matriz translacaoDeVolta = Matriz(3, 3).translacao(centro.x(), centro.y());

        transformacao = translacaoDeVolta * (matrizRotacao * translacaoParaOrigem);
    }
    else
    {
        // rotacao em torno de ponto arbitrario digitado pelo user
        Matriz translacaoParaOrigem = Matriz(3, 3).translacao(-dx, -dy);
        Matriz matrizRotacao = Matriz(3, 3).rotacao(-angulo); // ajuste do sentido
        Matriz translacaoDeVolta = Matriz(3, 3).translacao(dx, dy);

        transformacao = translacaoDeVolta * (matrizRotacao * translacaoParaOrigem);
    }

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

void Ponto::desenhar(QPainter *painter,
                     MyFrame *frame,
                     const Matriz &scn)
{
    Matriz mundo =
        transformacaoObjeto * (*this);

    Matriz normalizado =
        scn * mundo;

    QPointF vp =
        frame->viewportTransform(
            normalizado[0][0],
            normalizado[1][0]
            );

    painter->drawPoint(vp);
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

void Linha::desenhar(QPainter *painter,
                     MyFrame *frame,
                     const Matriz &scn)
{
    Matriz m1 =
        scn * (transformacaoObjeto * p1);

    Matriz m2 =
        scn * (transformacaoObjeto * p2);

    QPointF v1 =
        frame->viewportTransform(
            m1[0][0],
            m1[1][0]
            );

    QPointF v2 =
        frame->viewportTransform(
            m2[0][0],
            m2[1][0]
            );

    painter->drawLine(v1, v2);
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

Retangulo::Retangulo(QString n)
    : Objeto(n, "ret")
{
    pontosFixos.push_back(Ponto("", "", 1, 1));
    pontosFixos.push_back(Ponto("", "", 1, 15));
    pontosFixos.push_back(Ponto("", "", 15, 15));
    pontosFixos.push_back(Ponto("", "", 15, 1));

    transformacaoObj = Matriz::identidade();
}

Window::Window(QString n, float l, float a)
    : Retangulo(n)
{
    tipo = "window";

    largura = l;
    altura = a;
}

Matriz Window::gerarSCN()
{
    float cx = transformacaoObjeto[0][2];
    float cy = transformacaoObjeto[1][2];

    float angulo = atan2(
                       transformacaoObjeto[1][0],
                       transformacaoObjeto[0][0]
                       ) * 180.0 / M_PI;

    Matriz T =
        Matriz::translacao(-cx, -cy);

    Matriz R =
        Matriz::rotacao(-angulo);

    Matriz S =
        Matriz::escala(
            2.0f / largura,
            2.0f / altura
            );

    return S * R * T;
}

QPointF MyFrame::viewportTransform(float x,
                                   float y)
{
    float xv = ((x + 1.0f) / 2.0f)
    * width();

    float yv = ((1.0f - y) / 2.0f)
               * height();

    return QPointF(xv, yv);
}