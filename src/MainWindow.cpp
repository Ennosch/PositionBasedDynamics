#include <QtDebug>

#include "MainWindow.h"


MainWindow::MainWindow()
{

}

MainWindow::~MainWindow()
{

}

MainWindow::MainWindow(GLWidget *_glw)
{

}

void MainWindow::setupUi(GLWidget *_glw)
{
    // create Widget
    mainWidget =  new QWidget;
    controlWidget = new ControlWidget();
    glw = _glw;

//    connect(m_activeObject, SIGNAL(transformChanged(const QMatrix4x4, const QVector3D, const QVector3D, const QVector3D)) ,   this,    SLOT(foo(const QMatrix4x4, const QVector3D, const QVector3D, const QVector3D)));
    connect(glw->m_activeObject, SIGNAL(transformChanged(const QMatrix4x4, const QVector3D, const QVector3D, const QVector3D)), controlWidget, SLOT(setTransform(const QMatrix4x4, const QVector3D, const QVector3D, const QVector3D)));
    connect(controlWidget, SIGNAL(matrixChanged(const QVector3D, const QVector3D, const QVector3D)), glw, SLOT(uiTransformChange(const QVector3D, const QVector3D, const QVector3D)));
    //    connect(controlWidget);

    // layout
    layout.addWidget(glw);
    layout.addWidget(controlWidget);

    mainWidget->setLayout(&layout);

//    mainWidget->s
    setCentralWidget(mainWidget);

//    glw->setParent(mainWidget);
    mainWidget->resize(750,1000);
    glw->resize(900,720);
    this->resize(1000,720);
}
