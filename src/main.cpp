
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

QSurfaceFormat createGLFormat()
{
    QSurfaceFormat fmt;
    fmt.setMinorVersion(3);
    fmt.setMajorVersion(3);
    fmt.setSamples(4);
    fmt.setSwapInterval(1);
    fmt.setRenderableType(QSurfaceFormat::OpenGL);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    // default format need when using QWidgets (?)
    QSurfaceFormat::setDefaultFormat(fmt);
    return fmt;
}

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  MainWindow mainWindow;
  GLWidget glw;
  Scene scene(&glw);
//  DynamicsWorld dynamics;

  QFont myFont;
  myFont.setPointSize(12);
  app.setFont(myFont);

  QSurfaceFormat fmt = createGLFormat();
  glw.setFormat(fmt);

  glw.setScene(&scene);
//  scene.setDynamicsWorld(&dynamics);

  mainWindow.setGLController(&glw);
//  mainWindow.setDynamicsController(dynamics.controller());
  mainWindow.setupUi();
  mainWindow.show();

  return app.exec();
}


