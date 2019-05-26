#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
    connect(ui->vsFOV, &QSlider::valueChanged, ui->sbFOV, &QSpinBox::setValue);
    connect(ui->sbFOV, QOverload<int>::of(&QSpinBox::valueChanged), ui->vsFOV, &QSlider::setValue);
    connect(ui->vsAngle, &QSlider::valueChanged, ui->sbAngle, &QSpinBox::setValue);
    connect(ui->sbAngle, QOverload<int>::of(&QSpinBox::valueChanged), ui->vsAngle, &QSlider::setValue);

    // set methoden
    connect(ui->vsFOV, &QSlider::valueChanged, ui->openGLWidget, &MyGLWidget::setFOV);
    connect(ui->vsAngle, &QSlider::valueChanged, ui->openGLWidget, &MyGLWidget::setAngle);
    connect(ui->rbOrthogonal, &QRadioButton::toggled, ui->openGLWidget, &MyGLWidget::setProjectionMode);
    connect(ui->hsRotationA, &QSlider::valueChanged, ui->openGLWidget, &MyGLWidget::setRotationA);
    connect(ui->hsRotationB, &QSlider::valueChanged, ui->openGLWidget, &MyGLWidget::setRotationB);
    connect(ui->hsRotationC, &QSlider::valueChanged, ui->openGLWidget, &MyGLWidget::setRotationC);
    connect(ui->dsbNear, QOverload<double>::of(&QDoubleSpinBox::valueChanged), ui->openGLWidget, &MyGLWidget::setNear);
    connect(ui->dsbFar, QOverload<double>::of(&QDoubleSpinBox::valueChanged), ui->openGLWidget, &MyGLWidget::setFar);
    connect(ui->rbReset, &QPushButton::clicked, this, reset);

    connect(ui->openGLWidget, &MyGLWidget::farValueChanged, ui->dsbFar, &QDoubleSpinBox::setValue);
    connect(ui->openGLWidget, &MyGLWidget::nearValueChanged, ui->dsbNear, &QDoubleSpinBox::setValue);

    // Lambda Methoden
    connect(ui->vsFOV, &QSlider::valueChanged, [=] (){qInfo() << ui->openGLWidget->getFOV();});
    connect(ui->vsAngle, &QSlider::valueChanged, [=] (){qInfo() << ui->openGLWidget->getAngle();});
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::reset(){
    ui->vsFOV->setValue(45);
    ui->vsAngle->setValue(0);
    ui->rbOrthogonal->click();
    ui->hsRotationA->setValue(0);
    ui->hsRotationB->setValue(0);
    ui->hsRotationC->setValue(0);
    ui->dsbNear->setValue(0);
    ui->dsbFar->setValue(2);
}
