#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

    // mapa de contadores para identificar objetos do mesmo tipo com ids diferentes
    QMap<QString, int> contadores;

public slots:
    void draw();
    void aplicarEscala();
    void receberObjetoAdicionado(QString nome);
};
#endif // MAINWINDOW_H
