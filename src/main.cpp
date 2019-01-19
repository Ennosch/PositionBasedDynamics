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
#include <QFont>

// Project
#include "Window.h"
#include "GLWidget.h"
#include "Scene.h"
#include "AbstractScene.h"
#include "dynamics/dynamicsWorld.h"

#include "ControlWidget.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  MainWindow mainWindow;
  GLWidget glw;

  QFont myFont;
  myFont.setPointSize(12);
  app.setFont(myFont);

  QSurfaceFormat fmt;
  fmt.setMinorVersion(3);
  fmt.setMajorVersion(3);
  fmt.setSamples(4);
  fmt.setSwapInterval(1);
  fmt.setRenderableType(QSurfaceFormat::OpenGL);
  fmt.setProfile(QSurfaceFormat::CoreProfile);
  // default format need when using QWidgets (?)
  QSurfaceFormat::setDefaultFormat(fmt);
  glw.setFormat(fmt);

  Scene scene(&glw);
  glw.setScene(&scene);

  mainWindow.setupUi(&glw);
  mainWindow.show();

  return app.exec();
}


