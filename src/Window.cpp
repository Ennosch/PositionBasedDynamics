
// Project
#include "Window.h"
#include <iostream>

#include <stdio.h>

#include <QDebug>
#include <QKeyEvent>
#include <QString>
#include <QString>


#include "Scene.h"
//#include "AbstractScene.h"
//#include <QOpenGLFunctions>


// now include Scene.h, to define Window::initializeGL() which calls things from the scene()
// class Scene has only been forward declared to this point (?)
// if not #included we get:
//      member access into incomlpete type 'Scene' (whereever scene()->something is called)

void Window::printVersionInformation()
{
    uint test;
    glGenBuffers(100, &test);
//    qDebug()<<"HELLO---";
//    qDebug()<<"qDebug genBuffers "<<(void *)glGenBuffers;
//    std::cout<<"std::cout &glGenBuffers  "<<&glGenBuffers<<std::endl;
//    qDebug()<<"HELLO---2";
//    printf("Window GL glGenBuffers :%p\n", &glGenBuffers);
//    printf("Window GL glGenBuffers :%p\n", glGenBuffers);

//  QString glType;
//  QString glVersion;
//  QString glProfile;

//  // Get Version Information
//  glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
//  glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

//  // Get Profile Information
//#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
//  switch (format().profile())
//  {
//    CASE(NoProfile);
//    CASE(CoreProfile);
//    CASE(CompatibilityProfile);
//  }
//#undef CASE

  // qPrintable() will print our QString w/o quotes around it.
//  qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}


Window::Window(QWindow *parent) : QOpenGLWindow(NoPartialUpdate, parent)
{
  //connect(&m_timer, SIGNAL(timeout()), this, SLOT(update()));

  qDebug()<< format().swapInterval();

  if(format().swapInterval() == -1)
  {
      // V_blank synchronization not available (tearing likely to happen)
      qDebug("Swap Buffers at v_blank not available: refresh at approx 60fps.");
      m_timer.setInterval(17);
  }
  else
  {
      // V_blank synchronization available
      m_timer.setInterval(0);
  }
  //m_timer.setInterval(500);
  m_timer.start();
  //m_elapsTimer.start();
}

Scene *Window::scene() const
{
  return m_scene;
}

void Window::setScene(Scene *_scene)
{
  m_scene = _scene;
}

void Window::initializeGL()
{
  // Init OpenGL Backend  (QOpenGLFunctions)
  if (scene())
    scene()->initialize();

  printVersionInformation();

  // Init QtWindow specific things, connectionQtype
  // sender signal receive, method
  connect(context(), SIGNAL(aboutToBeDestroyed()), this, SLOT(teardownGL()), Qt::DirectConnection);
  connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
}

void Window::paintGL()
{
  if (scene())
    scene()->paint();
}

void Window::resizeGL(int _w, int _h)
{
  qDebug("New window size: %d, %d", _w, _h);
}

void Window::teardownGL()
{
    /*
  // Actually destroy our OpenGL information
  m_object.destroy();
  m_vertex.destroy();
  delete m_program;
  */
}

void Window::update()
{
    inputManager::update();
    // handle key press events
    //qDebug("update");


    if(inputManager::keyPressed(Qt::Key_Up))
    {
        qDebug(" -forward");
        scene()->m_myTransform.translate(0.0, 0.03, 0.0);
    }
    if(inputManager::keyPressed(Qt::Key_Down))
    {
        qDebug(" -down");
        scene()->m_myTransform.translate(0.0, -0.03, 0.0);
    }
    if(inputManager::keyPressed(Qt::Key_Left))
    {
        qDebug(" -left");
        scene()->m_myTransform.translate(0.03, 0.0, 0.0);
    }
    if(inputManager::keyPressed(Qt::Key_Right))
    {
        qDebug(" -right");
        scene()->m_myTransform.translate(-0.03, 0.0, 0.0);
    }

    scene()->update();
    QOpenGLWindow::update();
}

void Window::keyPressEvent(QKeyEvent *event)
{

    if (event->isAutoRepeat())
    {
      event->ignore();
    }
    else
    {
      inputManager::registerKeyPress(event->key());
      //inputManager::foo();
    }

    /*
    // Simple QKeyEvent switch
    if (event->isAutoRepeat())
    {
      qDebug("event isAutoRepeat");
    }
    else
    {
//      qDebug()<< "keyPressed: "<< event->text();
//      qDebug()<< "keyPressed: "<< event->key();
      switch(event->key())
      {
        case Qt::Key_Up: qDebug("up key pressed");
          //scene()->m_transform.translate(1 , 1);
          scene()->m_myTransform.translate(0.0, 0.01, 0.0);
        break;
        case Qt::Key_Left: qDebug("down key pressed");
        break;
      default: break;
      }
    }*/
}

void Window::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
    {
      event->ignore();
    }
    else
    {
      inputManager::registerKeyRelease(event->key());
    }
}

