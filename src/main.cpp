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


int main(int argc, char *argv[])
{

  QApplication app(argc, argv);
  Window window;
  GLWidget glw;

  QSurfaceFormat fmt;
  fmt.setMinorVersion(3);
  fmt.setMajorVersion(3);
  fmt.setSamples(4);
  fmt.setSwapInterval(1);
  fmt.setRenderableType(QSurfaceFormat::OpenGL);
  fmt.setProfile(QSurfaceFormat::CoreProfile);

  // needed for widgets (?)
  QSurfaceFormat::setDefaultFormat(fmt);

  window.setFormat(fmt);
  glw.setFormat(fmt);

  Scene scene(&window);
  Scene scene2(&glw);

  window.setScene(&scene);
  glw.setScene(&scene2);



// just init crashes !
// scene.initialize();
  window.resize(720, 720);
  window.show();

  glw.resize(720,720);
  glw.show();



  return app.exec();
}


