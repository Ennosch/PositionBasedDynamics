////////////////////////////////////////////////////////////////////////////////
/// @file main.cpp
/// @author Ramon Blanquer
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////

// Qt
#include <QGuiApplication>
#include <QPropertyAnimation>

#include <QFile>
#include <QDebug>

// Project
#include "Scene.h"
#include "Window.h"

int main(int argc, char *argv[])
{

  /*
  QString path = qApp->applicationDirPath();

  //path.append("./resources/shaders/simple.vert");
  path.append("./test.txt");
  QFile fileUrl(path);
  QString a = fileUrl.readAll();

  if( fileUrl.open(QIODevice::ReadWrite | QIODevice::Text) ) {
  QTextStream out(&fileUrl);

  //qDebug()<<a<<endl;

  }*/

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

  Scene scene(&window);
  window.setScene(&scene);

  window.resize(720, 720);
  window.show();

  return app.exec();
}
