
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
#include "parameters.h"

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
  mainWindow.resize(1280*1.5, 720*1.5);
  mainWindow.show();

  return app.exec();
}



/******************************************************************************
* FILE: omp_hello.c
* DESCRIPTION:
*   OpenMP Example - Hello World - C/C++ Version
*   In this simple example, the master thread forks a parallel region.
*   All threads in the team obtain their unique thread number and print it.
*   The master thread only prints the total number of threads.  Two OpenMP
*   library routines are used to obtain the number of threads and each
*   thread's number.
* AUTHOR: Blaise Barney  5/99
* LAST REVISED: 04/06/05
******************************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

//int main (int argc, char *argv[])
//{
//int nthreads, tid;

///* Fork a team of threads giving them their own copies of variables */
//#pragma omp parallel private(nthreads, tid)
//  {

//  /* Obtain thread number */
//  tid = omp_get_thread_num();
//  printf("Hello World from thread = %d\n", tid);

//  /* Only master thread does this */
//  if (tid == 0)
//    {
//    nthreads = omp_get_num_threads();
//    printf("Number of threads = %d\n", nthreads);
//    }

//  }  /* All threads join master thread and disband */

//}

/**/

