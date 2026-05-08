#include "myframe.h"

#include <QPaintEvent>
#include <QPainter>
#include <QPolygonF>
#include <QResizeEvent>
#include <algorithm>
#include <iterator>
#include <utility>

namespace
{
constexpr float PI = 3.14159265358979323846f;

float distanciaEntre(const QPointF &a, const QPointF &b)
{
    return std::hypot(static_cast<float>(b.x() - a.x()),
                      static_cast<float>(b.y() - a.y()));
}
}

Matriz Matriz::operator*(const Matriz &m) const
{
    const int qtdLinA = static_cast<int>(size());
    const int qtdColA = static_cast<int>(at(0).size());
    const int qtdLinB = static_cast<int>(m.size());
    const int qtdColB = static_cast<int>(m.at(0).size());

    if (qtdColA != qtdLinB)
    {
        throw std::runtime_error("Dimensoes invalidas para multiplicacao");
    }

    Matriz r(qtdLinA, qtdColB);

    for (int i = 0; i < qtdLinA; ++i)
    {
        for (int j = 0; j < qtdColB; ++j)
        {
            for (int k = 0; k < qtdColA; ++k)
            {
                r[i][j] += (*this)[i][k] * m[k][j];
            }
        }
    }

    return r;
}

Matriz Matriz::escala(float x, float y)
{
    Matriz m(3, 3);

    m[0][0] = x;    m[0][1] = 0.0f; m[0][2] = 0.0f;
    m[1][0] = 0.0f; m[1][1] = y;    m[1][2] = 0.0f;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;

    return m;
}

Matriz Matriz::translacao(float dx, float dy)
{
    Matriz m(3, 3);

    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = dx;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = dy;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;

    return m;
}

Matriz Matriz::rotacao(float angulo)
{
    Matriz m(3, 3);
    const float anguloRad = angulo * PI / 180.0f;
    const float c = std::cos(anguloRad);
    const float s = std::sin(anguloRad);

    m[0][0] = c;    m[0][1] = -s;   m[0][2] = 0.0f;
    m[1][0] = s;    m[1][1] = c;    m[1][2] = 0.0f;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;

    return m;
}

Matriz Matriz::identidade()
{
    Matriz m(3, 3);

    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;

    return m;
}

Objeto::Objeto(QString n, QString t)
    : nome(std::move(n))
    , tipo(std::move(t))
{
}

QString Objeto::getNome() const
{
    return nome;
}

Ponto::Ponto(QString n, QString t, float x, float y)
    : Objeto(std::move(n), std::move(t))
    , Matriz(3, 1)
{
    (*this)[0][0] = x;
    (*this)[1][0] = y;
    (*this)[2][0] = 1.0f;
}

void Ponto::desenhar(QPainter *painter, MyFrame *frame, const Matriz &scn)
{
    const Matriz normalizado = scn * static_cast<const Matriz &>(*this);
    const QPointF vp = frame->viewportTransform(normalizado[0][0], normalizado[1][0]);

    painter->drawPoint(vp);
}

void Ponto::aplicarTransformacao(const Matriz &transformacao)
{
    const Matriz resultado = transformacao * static_cast<const Matriz &>(*this);

    (*this)[0][0] = resultado[0][0];
    (*this)[1][0] = resultado[1][0];
    (*this)[2][0] = resultado[2][0];
}

QPointF Ponto::getCentro() const
{
    return QPointF(getXF(), getYF());
}

float Ponto::getXF() const
{
    return (*this)[0][0];
}

float Ponto::getYF() const
{
    return (*this)[1][0];
}

Linha::Linha(QString n, QString t, const Ponto &p1Inicial, const Ponto &p2Inicial)
    : Objeto(std::move(n), std::move(t))
    , p1(p1Inicial)
    , p2(p2Inicial)
{
}

void Linha::desenhar(QPainter *painter, MyFrame *frame, const Matriz &scn)
{
    const Matriz m1 = scn * static_cast<const Matriz &>(p1);
    const Matriz m2 = scn * static_cast<const Matriz &>(p2);

    const QPointF v1 = frame->viewportTransform(m1[0][0], m1[1][0]);
    const QPointF v2 = frame->viewportTransform(m2[0][0], m2[1][0]);

    painter->drawLine(v1, v2);
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

Retangulo::Retangulo(QString n, QString t, float x, float y, float largura, float altura)
    : Objeto(std::move(n), std::move(t))
{
    pontos.reserve(4);
    pontos.emplace_back("", "", x, y);
    pontos.emplace_back("", "", x + largura, y);
    pontos.emplace_back("", "", x + largura, y + altura);
    pontos.emplace_back("", "", x, y + altura);
}

Retangulo::Retangulo(QString n, QString t, const std::vector<Ponto> &pontosRetangulo)
    : Objeto(std::move(n), std::move(t))
    , pontos(pontosRetangulo)
{
}

void Retangulo::desenhar(QPainter *painter, MyFrame *frame, const Matriz &scn)
{
    if (pontos.empty())
    {
        return;
    }

    QPolygonF poligono;
    poligono.reserve(static_cast<int>(pontos.size()));

    for (const Ponto &ponto : pontos)
    {
        const Matriz normalizado = scn * static_cast<const Matriz &>(ponto);
        poligono << frame->viewportTransform(normalizado[0][0], normalizado[1][0]);
    }

    painter->drawPolygon(poligono);
}

void Retangulo::aplicarTransformacao(const Matriz &transformacao)
{
    for (Ponto &ponto : pontos)
    {
        ponto.aplicarTransformacao(transformacao);
    }
}

QPointF Retangulo::getCentro() const
{
    float somaX = 0.0f;
    float somaY = 0.0f;

    for (const Ponto &ponto : pontos)
    {
        somaX += ponto.getXF();
        somaY += ponto.getYF();
    }

    const float divisor = pontos.empty() ? 1.0f : static_cast<float>(pontos.size());
    return QPointF(somaX / divisor, somaY / divisor);
}

float Retangulo::larguraAtual() const
{
    return distanciaEntre(pontos[0].getCentro(), pontos[1].getCentro());
}

float Retangulo::alturaAtual() const
{
    return distanciaEntre(pontos[0].getCentro(), pontos[3].getCentro());
}

QPointF Retangulo::vupAtual() const
{
    return pontos[3].getCentro() - pontos[0].getCentro();
}

Window::Window(QString n, float largura, float altura)
    : Retangulo(std::move(n), "Window", -largura / 2.0f, -altura / 2.0f, largura, altura)
{
}

Matriz Window::gerarSCN() const
{
    const QPointF wc = getCentro();
    const QPointF vup = vupAtual();
    const float anguloVupComY = std::atan2(vup.x(), vup.y()) * 180.0f / PI;
    const float larguraNormalizacao = larguraAtual();
    const float alturaNormalizacao = alturaAtual();

    const Matriz translacaoOrigem = Matriz(3, 3).translacao(-wc.x(), -wc.y());
    const Matriz alinhamentoVup = Matriz(3, 3).rotacao(-anguloVupComY);
    const Matriz normalizacao = Matriz(3, 3).escala(2.0f / larguraNormalizacao,
                                                    2.0f / alturaNormalizacao);

    return normalizacao * alinhamentoVup * translacaoOrigem;
}

MyFrame::MyFrame(QWidget *parent)
    : QFrame(parent)
    , viewportXMin(0.0f)
    , viewportYMin(0.0f)
    , viewportXMax(0.0f)
    , viewportYMax(0.0f)
    , viewportAcompanhaFrame(true)
{
    displayFile.push_front(new Window("window", 800.0f, 600.0f));
}

MyFrame::~MyFrame()
{
    for (Objeto *objeto : displayFile)
    {
        delete objeto;
    }
}

void MyFrame::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    if (viewportAcompanhaFrame && getViewportRect().isEmpty())
    {
        resetarViewportParaFrame();
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen viewportPen(Qt::darkGray);
    viewportPen.setStyle(Qt::DashLine);
    painter.setPen(viewportPen);
    painter.drawRect(getViewportRect());
    painter.setPen(Qt::black);

    Window *window = getWindow();
    if (!window)
    {
        return;
    }

    const Matriz scn = window->gerarSCN();

    painter.save();
    painter.setClipRect(getViewportRect());

    auto it = displayFile.begin();
    if (it != displayFile.end())
    {
        ++it;
    }

    for (; it != displayFile.end(); ++it)
    {
        (*it)->desenhar(&painter, this, scn);
    }

    painter.restore();
}

void MyFrame::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);

    if (viewportAcompanhaFrame)
    {
        const QRectF viewport = viewportPadraoDoFrame();
        viewportXMin = static_cast<float>(viewport.left());
        viewportYMin = static_cast<float>(viewport.top());
        viewportXMax = static_cast<float>(viewport.right());
        viewportYMax = static_cast<float>(viewport.bottom());
        emit viewportAlterada(getViewportRect());
    }
}

void MyFrame::adicionarObjeto(Objeto *obj)
{
    if (!obj)
    {
        return;
    }

    displayFile.push_back(obj);
    emit objAdicionado(obj->getNome());
    update();
}

void MyFrame::carregarCenaTeste()
{
    limparObjetosDoMundo();

    adicionarObjeto(new Retangulo("RetanguloMundo1", "Retangulo", 720.0f, 520.0f, 180.0f, 110.0f));
    adicionarObjeto(new Retangulo("RetanguloMundo2", "Retangulo", 1040.0f, 680.0f, 140.0f, 220.0f));
    adicionarObjeto(new Retangulo("RetanguloMundo3", "Retangulo", 1290.0f, 560.0f, 210.0f, 130.0f));

    adicionarObjeto(new Linha("LinhaMundo1",
                              "Linha",
                              Ponto("", "", 650.0f, 480.0f),
                              Ponto("", "", 1490.0f, 480.0f)));
    adicionarObjeto(new Linha("LinhaMundo2",
                              "Linha",
                              Ponto("", "", 830.0f, 430.0f),
                              Ponto("", "", 830.0f, 980.0f)));
    adicionarObjeto(new Linha("LinhaMundo3",
                              "Linha",
                              Ponto("", "", 760.0f, 930.0f),
                              Ponto("", "", 1520.0f, 700.0f)));
}

void MyFrame::escalarObjeto(int indice, float escalaX, float escalaY)
{
    Objeto *objeto = getObjetoPorIndice(indice);
    if (!objeto)
    {
        return;
    }

    const QPointF centro = objeto->getCentro();
    const Matriz translacaoParaOrigem = Matriz(3, 3).translacao(-centro.x(), -centro.y());
    const Matriz matrizEscala = Matriz(3, 3).escala(escalaX, escalaY);
    const Matriz translacaoDeVolta = Matriz(3, 3).translacao(centro.x(), centro.y());

    aplicarTransformacaoComposta(objeto,
                                 translacaoDeVolta * matrizEscala * translacaoParaOrigem);
}

void MyFrame::transladarObjeto(int indice, float dx, float dy)
{
    Objeto *objeto = getObjetoPorIndice(indice);
    if (!objeto)
    {
        return;
    }

    aplicarTransformacaoComposta(objeto, Matriz(3, 3).translacao(dx, dy));
}

void MyFrame::rotacionarObjeto(int indice, float angulo, bool usarCentroide, int dx, int dy)
{
    Objeto *objeto = getObjetoPorIndice(indice);
    if (!objeto)
    {
        return;
    }

    const QPointF centroRotacao = usarCentroide ? objeto->getCentro()
                                                : QPointF(dx, dy);

    const Matriz translacaoParaOrigem = Matriz(3, 3).translacao(-centroRotacao.x(),
                                                                -centroRotacao.y());
    const Matriz matrizRotacao = Matriz(3, 3).rotacao(angulo);
    const Matriz translacaoDeVolta = Matriz(3, 3).translacao(centroRotacao.x(),
                                                             centroRotacao.y());

    aplicarTransformacaoComposta(objeto,
                                 translacaoDeVolta * matrizRotacao * translacaoParaOrigem);
}

void MyFrame::escalarWindow(float escalaX, float escalaY)
{
    Window *window = getWindow();
    if (!window)
    {
        return;
    }

    const QPointF centro = window->getCentro();
    const Matriz translacaoParaOrigem = Matriz(3, 3).translacao(-centro.x(), -centro.y());
    const Matriz matrizEscala = Matriz(3, 3).escala(escalaX, escalaY);
    const Matriz translacaoDeVolta = Matriz(3, 3).translacao(centro.x(), centro.y());

    aplicarTransformacaoComposta(window,
                                 translacaoDeVolta * matrizEscala * translacaoParaOrigem);
}

void MyFrame::transladarWindow(float dx, float dy)
{
    Window *window = getWindow();
    if (!window)
    {
        return;
    }

    aplicarTransformacaoComposta(window, Matriz(3, 3).translacao(dx, dy));
}

void MyFrame::rotacionarWindow(float angulo)
{
    Window *window = getWindow();
    if (!window)
    {
        return;
    }

    const QPointF centro = window->getCentro();
    const Matriz translacaoParaOrigem = Matriz(3, 3).translacao(-centro.x(), -centro.y());
    const Matriz matrizRotacao = Matriz(3, 3).rotacao(angulo);
    const Matriz translacaoDeVolta = Matriz(3, 3).translacao(centro.x(), centro.y());

    aplicarTransformacaoComposta(window,
                                 translacaoDeVolta * matrizRotacao * translacaoParaOrigem);
}

void MyFrame::definirViewport(float xMin, float yMin, float xMax, float yMax)
{
    if (xMax <= xMin || yMax <= yMin)
    {
        return;
    }

    viewportXMin = xMin;
    viewportYMin = yMin;
    viewportXMax = xMax;
    viewportYMax = yMax;

    const QRectF viewportFrame = viewportPadraoDoFrame();
    viewportAcompanhaFrame =
        xMin == static_cast<float>(viewportFrame.left()) &&
        yMin == static_cast<float>(viewportFrame.top()) &&
        xMax == static_cast<float>(viewportFrame.right()) &&
        yMax == static_cast<float>(viewportFrame.bottom());

    emit viewportAlterada(getViewportRect());
    update();
}

void MyFrame::resetarViewportParaFrame()
{
    const QRectF viewport = viewportPadraoDoFrame();

    viewportXMin = static_cast<float>(viewport.left());
    viewportYMin = static_cast<float>(viewport.top());
    viewportXMax = static_cast<float>(viewport.right());
    viewportYMax = static_cast<float>(viewport.bottom());
    viewportAcompanhaFrame = true;

    emit viewportAlterada(getViewportRect());
    update();
}

QPointF MyFrame::viewportTransform(float x, float y) const
{
    const QRectF viewport = getViewportRect();
    const float larguraViewport = static_cast<float>(viewport.width());
    const float alturaViewport = static_cast<float>(viewport.height());

    const float xv = static_cast<float>(viewport.left()) + ((x + 1.0f) * 0.5f) * larguraViewport;
    const float yv = static_cast<float>(viewport.top()) + ((1.0f - y) * 0.5f) * alturaViewport;

    return QPointF(xv, yv);
}

QRectF MyFrame::getViewportRect() const
{
    return QRectF(QPointF(viewportXMin, viewportYMin),
                  QPointF(viewportXMax, viewportYMax)).normalized();
}

Window *MyFrame::getWindow() const
{
    if (displayFile.empty())
    {
        return nullptr;
    }

    return dynamic_cast<Window *>(displayFile.front());
}

int MyFrame::quantidadeObjetos() const
{
    return std::max(0, static_cast<int>(displayFile.size()) - 1);
}

void MyFrame::limparObjetosDoMundo()
{
    while (displayFile.size() > 1)
    {
        delete displayFile.back();
        displayFile.pop_back();
    }
}

void MyFrame::aplicarTransformacaoComposta(Objeto *obj, const Matriz &transformacao)
{
    if (!obj)
    {
        return;
    }

    obj->aplicarTransformacao(transformacao);
    update();
}

Objeto *MyFrame::getObjetoPorIndice(int indice) const
{
    if (indice < 0 || indice >= quantidadeObjetos())
    {
        return nullptr;
    }

    auto it = displayFile.begin();
    std::advance(it, indice + 1);
    return (it != displayFile.end()) ? *it : nullptr;
}

QRectF MyFrame::viewportPadraoDoFrame() const
{
    return QRectF(0.0,
                  0.0,
                  static_cast<qreal>(std::max(1, width() - 1)),
                  static_cast<qreal>(std::max(1, height() - 1)));
}
