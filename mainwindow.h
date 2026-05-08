#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

class QDoubleSpinBox;
class QSpinBox;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    QMap<QString, int> contadores;

    QDoubleSpinBox *windowScaleX;
    QDoubleSpinBox *windowScaleY;
    QSpinBox *windowTransX;
    QSpinBox *windowTransY;
    QDoubleSpinBox *windowRotAng;
    QSpinBox *viewportXMin;
    QSpinBox *viewportYMin;
    QSpinBox *viewportXMax;
    QSpinBox *viewportYMax;

    void configurarPaineisWindowViewport();
    void sincronizarCamposViewport(const QRectF &viewport);

public slots:
    void draw();
    void aplicarEscala();
    void aplicarRotacao();
    void aplicarTranslacao();
    void aplicarEscalaWindow();
    void aplicarRotacaoWindow();
    void aplicarTranslacaoWindow();
    void aplicarViewport();
    void resetarViewportParaFrame();
    void receberObjetoAdicionado(QString nome);
    void onCentroideToggled(bool checked);
};

#endif // MAINWINDOW_H
