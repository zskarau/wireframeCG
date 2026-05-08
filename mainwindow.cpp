#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDockWidget>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLabel>
#include <QMetaObject>
#include <QPushButton>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QStatusBar>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , windowScaleX(nullptr)
    , windowScaleY(nullptr)
    , windowTransX(nullptr)
    , windowTransY(nullptr)
    , windowRotAng(nullptr)
    , viewportXMin(nullptr)
    , viewportYMin(nullptr)
    , viewportXMax(nullptr)
    , viewportYMax(nullptr)
{
    ui->setupUi(this);

    ui->comboBox->clear();
    ui->comboBox->addItem("Ponto");
    ui->comboBox->addItem("Linha");
    ui->comboBox->addItem("Retangulo");
    ui->comboBox->setEnabled(false);

    ui->inputX->setMinimum(-1000);
    ui->inputY->setMinimum(-1000);
    ui->inputRotX->setMinimum(-1000);
    ui->inputRotY->setMinimum(-1000);
    ui->inputX->setEnabled(false);
    ui->inputY->setEnabled(false);
    ui->btnDraw->setText("Recarregar cena");

    connect(ui->frame, &MyFrame::objAdicionado,
            this, &MainWindow::receberObjetoAdicionado);
    connect(ui->btnScale, &QPushButton::clicked,
            this, &MainWindow::aplicarEscala);
    connect(ui->btnTrans, &QPushButton::clicked,
            this, &MainWindow::aplicarTranslacao);
    connect(ui->btnRotation, &QPushButton::clicked,
            this, &MainWindow::aplicarRotacao);
    connect(ui->checkBox, &QCheckBox::toggled,
            this, &MainWindow::onCentroideToggled);
    connect(ui->frame, &MyFrame::viewportAlterada,
            this, &MainWindow::sincronizarCamposViewport);

    configurarPaineisWindowViewport();
    QMetaObject::invokeMethod(this,
                              [this]() { resetarViewportParaFrame(); },
                              Qt::QueuedConnection);
    draw();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::configurarPaineisWindowViewport()
{
    auto *windowDock = new QDockWidget("Window", this);
    auto *windowWidget = new QWidget(windowDock);
    auto *windowLayout = new QVBoxLayout(windowWidget);
    auto *windowForm = new QFormLayout();

    windowScaleX = new QDoubleSpinBox(windowWidget);
    windowScaleY = new QDoubleSpinBox(windowWidget);
    windowTransX = new QSpinBox(windowWidget);
    windowTransY = new QSpinBox(windowWidget);
    windowRotAng = new QDoubleSpinBox(windowWidget);

    windowScaleX->setRange(0.1, 4.0);
    windowScaleY->setRange(0.1, 4.0);
    windowScaleX->setSingleStep(0.1);
    windowScaleY->setSingleStep(0.1);
    windowScaleX->setValue(1.0);
    windowScaleY->setValue(1.0);

    windowTransX->setRange(-1000, 1000);
    windowTransY->setRange(-1000, 1000);

    windowRotAng->setRange(-360.0, 360.0);
    windowRotAng->setSingleStep(5.0);

    windowForm->addRow("Escala X", windowScaleX);
    windowForm->addRow("Escala Y", windowScaleY);
    windowForm->addRow("Translacao X", windowTransX);
    windowForm->addRow("Translacao Y", windowTransY);
    windowForm->addRow("Rotacao", windowRotAng);

    auto *btnWindowScale = new QPushButton("Aplicar escala", windowWidget);
    auto *btnWindowTranslate = new QPushButton("Aplicar translacao", windowWidget);
    auto *btnWindowRotate = new QPushButton("Aplicar rotacao", windowWidget);

    windowLayout->addWidget(new QLabel("Transformacoes da window", windowWidget));
    windowLayout->addLayout(windowForm);
    windowLayout->addWidget(btnWindowScale);
    windowLayout->addWidget(btnWindowTranslate);
    windowLayout->addWidget(btnWindowRotate);
    windowLayout->addStretch();

    connect(btnWindowScale, &QPushButton::clicked,
            this, &MainWindow::aplicarEscalaWindow);
    connect(btnWindowTranslate, &QPushButton::clicked,
            this, &MainWindow::aplicarTranslacaoWindow);
    connect(btnWindowRotate, &QPushButton::clicked,
            this, &MainWindow::aplicarRotacaoWindow);

    windowDock->setWidget(windowWidget);
    addDockWidget(Qt::RightDockWidgetArea, windowDock);
    splitDockWidget(ui->dockWidget_2, windowDock, Qt::Vertical);

    auto *viewportDock = new QDockWidget("Viewport", this);
    auto *viewportWidget = new QWidget(viewportDock);
    auto *viewportLayout = new QVBoxLayout(viewportWidget);
    auto *viewportForm = new QFormLayout();

    viewportXMin = new QSpinBox(viewportWidget);
    viewportYMin = new QSpinBox(viewportWidget);
    viewportXMax = new QSpinBox(viewportWidget);
    viewportYMax = new QSpinBox(viewportWidget);

    viewportXMin->setRange(-1000, 4000);
    viewportYMin->setRange(-1000, 4000);
    viewportXMax->setRange(-1000, 4000);
    viewportYMax->setRange(-1000, 4000);

    viewportForm->addRow("X min", viewportXMin);
    viewportForm->addRow("Y min", viewportYMin);
    viewportForm->addRow("X max", viewportXMax);
    viewportForm->addRow("Y max", viewportYMax);

    auto *btnViewportReset = new QPushButton("Usar frame inteiro", viewportWidget);

    viewportLayout->addWidget(new QLabel("Viewport em pixels", viewportWidget));
    viewportLayout->addLayout(viewportForm);
    viewportLayout->addWidget(btnViewportReset);
    viewportLayout->addStretch();

    connect(viewportXMin, &QSpinBox::valueChanged,
            this, &MainWindow::aplicarViewport);
    connect(viewportYMin, &QSpinBox::valueChanged,
            this, &MainWindow::aplicarViewport);
    connect(viewportXMax, &QSpinBox::valueChanged,
            this, &MainWindow::aplicarViewport);
    connect(viewportYMax, &QSpinBox::valueChanged,
            this, &MainWindow::aplicarViewport);
    connect(btnViewportReset, &QPushButton::clicked,
            this, &MainWindow::resetarViewportParaFrame);

    viewportDock->setWidget(viewportWidget);
    addDockWidget(Qt::RightDockWidgetArea, viewportDock);
    splitDockWidget(windowDock, viewportDock, Qt::Vertical);
}

void MainWindow::receberObjetoAdicionado(QString nome)
{
    ui->comboBoxObj->addItem(nome);
}

void MainWindow::onCentroideToggled(bool checked)
{
    ui->inputRotX->setEnabled(!checked);
    ui->inputRotY->setEnabled(!checked);
}

void MainWindow::aplicarEscala()
{
    const int indiceObjeto = ui->comboBoxObj->currentIndex();
    if (indiceObjeto < 0)
    {
        statusBar()->showMessage("Selecione um objeto para escalar.", 3000);
        return;
    }

    const float escalaX = static_cast<float>(ui->inputScaleX->value());
    const float escalaY = static_cast<float>(ui->inputScaleY->value());

    ui->frame->escalarObjeto(indiceObjeto, escalaX, escalaY);
}

void MainWindow::aplicarTranslacao()
{
    const int indiceObjeto = ui->comboBoxObj->currentIndex();
    if (indiceObjeto < 0)
    {
        statusBar()->showMessage("Selecione um objeto para transladar.", 3000);
        return;
    }

    const int transX = ui->inputTransX->value();
    const int transY = ui->inputTransY->value();

    ui->frame->transladarObjeto(indiceObjeto, transX, transY);
}

void MainWindow::aplicarRotacao()
{
    const int indiceObjeto = ui->comboBoxObj->currentIndex();
    if (indiceObjeto < 0)
    {
        statusBar()->showMessage("Selecione um objeto para rotacionar.", 3000);
        return;
    }

    const float angRot = static_cast<float>(ui->inputRotAng->value());
    const int inputRotX = ui->inputRotX->value();
    const int inputRotY = ui->inputRotY->value();
    const bool usarCentroide = ui->checkBox->isChecked();

    ui->frame->rotacionarObjeto(indiceObjeto, angRot, usarCentroide, inputRotX, inputRotY);
}

void MainWindow::aplicarEscalaWindow()
{
    ui->frame->escalarWindow(static_cast<float>(windowScaleX->value()),
                             static_cast<float>(windowScaleY->value()));
}

void MainWindow::aplicarTranslacaoWindow()
{
    ui->frame->transladarWindow(static_cast<float>(windowTransX->value()),
                                static_cast<float>(windowTransY->value()));
}

void MainWindow::aplicarRotacaoWindow()
{
    ui->frame->rotacionarWindow(static_cast<float>(windowRotAng->value()));
}

void MainWindow::aplicarViewport()
{
    const int xMin = viewportXMin->value();
    const int yMin = viewportYMin->value();
    const int xMax = viewportXMax->value();
    const int yMax = viewportYMax->value();

    if (xMax <= xMin || yMax <= yMin)
    {
        return;
    }

    ui->frame->definirViewport(static_cast<float>(xMin),
                               static_cast<float>(yMin),
                               static_cast<float>(xMax),
                               static_cast<float>(yMax));
}

void MainWindow::resetarViewportParaFrame()
{
    ui->frame->resetarViewportParaFrame();
    statusBar()->showMessage("Viewport ajustada para ocupar todo o frame.", 3000);
}

void MainWindow::sincronizarCamposViewport(const QRectF &viewport)
{
    if (!viewportXMin || !viewportYMin || !viewportXMax || !viewportYMax)
    {
        return;
    }

    const QSignalBlocker bloqueioXMin(viewportXMin);
    const QSignalBlocker bloqueioYMin(viewportYMin);
    const QSignalBlocker bloqueioXMax(viewportXMax);
    const QSignalBlocker bloqueioYMax(viewportYMax);

    viewportXMin->setValue(static_cast<int>(viewport.left()));
    viewportYMin->setValue(static_cast<int>(viewport.top()));
    viewportXMax->setValue(static_cast<int>(viewport.right()));
    viewportYMax->setValue(static_cast<int>(viewport.bottom()));
}

void MainWindow::draw()
{
    ui->comboBoxObj->clear();
    ui->frame->carregarCenaTeste();
    statusBar()->showMessage("Cena fixa carregada em coordenadas de mundo. Window e viewport podem ser ajustadas.", 5000);
}
