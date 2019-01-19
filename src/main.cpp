// STL
#include <iostream>
#include <vector>

// Qt
#include <QApplication>
#include <QPropertyAnimation>

#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QOpenGLWidget>


#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QMainWindow>

// Project
#include "Window.h"
#include "GLWidget.h"
#include "Scene.h"
#include "AbstractScene.h"
#include "dynamics/dynamicsWorld.h"


QString getString(QString Filename)
{
    QFile mFile(Filename);
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug()<< "could not open file for writing";
        return 0;
    }
    QTextStream in (&mFile);
    QString mText = in.readAll();
    //qDebug() << mText;
    mFile.close();
    return mText;
}


void Read(QString Filename)
{
    QFile mFile(Filename);
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug()<< "could not open file for writing";
        return;
    }
    QTextStream in (&mFile);
    QString mText = in.readAll();
    qDebug() << mText;
    mFile.close();
}


class MainWindow : public QMainWindow
{
public:
    MainWindow() : QMainWindow()
    {
        qDebug()<<"MainWindow ctor";
        layoutPtr = new QHBoxLayout;
        button = new QPushButton;
        glw = new GLWidget;


          QSurfaceFormat fmt;
          fmt.setMinorVersion(3);
          fmt.setMajorVersion(3);
          fmt.setSamples(4);
          fmt.setSwapInterval(1);
          fmt.setRenderableType(QSurfaceFormat::OpenGL);
          fmt.setProfile(QSurfaceFormat::CoreProfile);


          // needed for widgets (?)
          QSurfaceFormat::setDefaultFormat(fmt);
          glw->setFormat(fmt);

          scene = new Scene(glw);
          glw->setScene(scene);
          glw->resize(720, 720);

        layoutPtr->addWidget(glw);

        qDebug()<<"hello QtCreator ?";
        layoutPtr->addWidget(button);

        cw = new QWidget();
        cw->setLayout(layoutPtr);
        setCentralWidget(cw);
    }

private:
    QWidget *cw;
    QHBoxLayout *layoutPtr;
    QPushButton *button;
    GLWidget *glw;
    Scene *scene;
};


int main(int argc, char *argv[])
{

  QApplication app(argc, argv);
//  Window window;
//  GLWidget glw;

//  QSurfaceFormat fmt;
//  fmt.setMinorVersion(3);
//  fmt.setMajorVersion(3);
//  fmt.setSamples(4);
//  fmt.setSwapInterval(1);
//  fmt.setRenderableType(QSurfaceFormat::OpenGL);
//  fmt.setProfile(QSurfaceFormat::CoreProfile);


//  // needed for widgets (?)
//  QSurfaceFormat::setDefaultFormat(fmt);

////  window.setFormat(fmt);
//  glw.setFormat(fmt);

////  Scene scene(&window);
//  Scene scene2(&glw);

////  window.setScene(&scene);
//  glw.setScene(&scene2);

//// just init crashes !
//// scene.initialize();
////  window.resize(720, 720);
////  window.show();

//  glw.resize(720,720);
//  glw.show();

  MainWindow window;
  window.show();

  return app.exec();
}


