#ifndef MYFRAME_H
#define MYFRAME_H

#include <QFrame>
#include <QPointF>
#include <QRectF>
#include <QString>
#include <cmath>
#include <list>
#include <stdexcept>
#include <vector>

class QPainter;

class Matriz : public std::vector<std::vector<float>>
{
public:
    Matriz(int l, int c)
        : std::vector<std::vector<float>>(l, std::vector<float>(c, 0.0f))
    {
    }

    Matriz operator*(const Matriz &m) const;

    Matriz escala(float x, float y);
    Matriz translacao(float dx, float dy);
    Matriz rotacao(float angulo);
    Matriz identidade();
};

class MyFrame;

class Objeto
{
public:
    QString nome;
    QString tipo;

    Objeto(QString n, QString t);
    virtual ~Objeto() = default;

    virtual void desenhar(QPainter *painter, MyFrame *frame, const Matriz &scn) = 0;
    virtual void aplicarTransformacao(const Matriz &transformacao) = 0;
    virtual QPointF getCentro() const = 0;

    QString getNome() const;
};

class Ponto : public Objeto, public Matriz
{
public:
    Ponto(QString n, QString t, float x, float y);

    void desenhar(QPainter *painter, MyFrame *frame, const Matriz &scn) override;
    void aplicarTransformacao(const Matriz &transformacao) override;
    QPointF getCentro() const override;

    float getXF() const;
    float getYF() const;
};

class Linha : public Objeto
{
public:
    Ponto p1;
    Ponto p2;

    Linha(QString n, QString t, const Ponto &p1, const Ponto &p2);

    void desenhar(QPainter *painter, MyFrame *frame, const Matriz &scn) override;
    void aplicarTransformacao(const Matriz &transformacao) override;
    QPointF getCentro() const override;
};

class Retangulo : public Objeto
{
public:
    std::vector<Ponto> pontos;

    Retangulo(QString n, QString t, float x, float y, float largura, float altura);
    Retangulo(QString n, QString t, const std::vector<Ponto> &pontosRetangulo);

    void desenhar(QPainter *painter, MyFrame *frame, const Matriz &scn) override;
    void aplicarTransformacao(const Matriz &transformacao) override;
    QPointF getCentro() const override;

protected:
    float larguraAtual() const;
    float alturaAtual() const;
    QPointF vupAtual() const;
};

class Window : public Retangulo
{
public:
    Window(QString n, float largura, float altura);

    Matriz gerarSCN() const;
};

class MyFrame : public QFrame
{
    Q_OBJECT

public:
    explicit MyFrame(QWidget *parent = nullptr);
    ~MyFrame() override;

    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void adicionarObjeto(Objeto *obj);
    void carregarCenaTeste();

    void escalarObjeto(int indice, float escalaX, float escalaY);
    void transladarObjeto(int indice, float dx, float dy);
    void rotacionarObjeto(int indice, float angulo, bool usarCentroide, int dx, int dy);

    void escalarWindow(float escalaX, float escalaY);
    void transladarWindow(float dx, float dy);
    void rotacionarWindow(float angulo);

    void definirViewport(float xMin, float yMin, float xMax, float yMax);
    void resetarViewportParaFrame();
    QPointF viewportTransform(float x, float y) const;
    QRectF getViewportRect() const;
    Window *getWindow() const;
    int quantidadeObjetos() const;

    std::list<Objeto *> displayFile;

signals:
    void objAdicionado(QString nome);
    void viewportAlterada(QRectF viewport);

private:
    float viewportXMin;
    float viewportYMin;
    float viewportXMax;
    float viewportYMax;
    bool viewportAcompanhaFrame;

    void limparObjetosDoMundo();
    void aplicarTransformacaoComposta(Objeto *obj, const Matriz &transformacao);
    Objeto *getObjetoPorIndice(int indice) const;
    QRectF viewportPadraoDoFrame() const;
};

#endif // MYFRAME_H
