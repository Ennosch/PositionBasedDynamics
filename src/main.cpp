////////////////////////////////////////////////////////////////////////////////
/// @file main.cpp
/// @author Ramon Blanquer
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////

// Qt
#include <QGuiApplication>
#include <QPropertyAnimation>

#include <QFile>
#include <Qdebug>
#include <QTextStream>


// Project
#include "Scene.h"
#include "Window.h"
#include "vertex.h"
#include "AbstractScene.h"

#include <iostream>

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

  QGuiApplication app(argc, argv);

  Window window;
  QSurfaceFormat fmt;
  fmt.setMinorVersion(3);
  fmt.setMajorVersion(3);
  fmt.setSamples(16);
  fmt.setSwapInterval(1);
  
  fmt.setRenderableType(QSurfaceFormat::OpenGL);
  fmt.setProfile(QSurfaceFormat::CoreProfile);
  window.setFormat(fmt);


  //AbstractScene aScene(&window);

  Scene scene(&window);
  window.setScene(&scene);

  window.resize(720, 720);
  window.show();

  QString mFilename = "/Users/ennoschlunder/Dev/QtOpenGLBoilerplate/test.txt";
  QString shader = ":/shader/simple.vert";

  //:/shader/simple.vert
  //:/shaders/simple.vert
  //Write(mFilename);
  //Read(shader);

  return app.exec();

}
