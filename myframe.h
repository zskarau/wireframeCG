#ifndef MYFRAME_H
#define MYFRAME_H

#include <QFrame>
#include <QWidget>

class MyFrame : public QFrame
{
    Q_OBJECT
public:
    MyFrame(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *keyEvent) override;

public slots:
    void mostrar();
    void apagar();
    void desenharRect();

private:
    QLine linha;
    QRect retangulo;
    bool flag;
    bool flagRect;
};

#endif // MYFRAME_H
