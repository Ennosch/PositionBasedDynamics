
// Project
#include "Window.h"
#include <iostream>
#include <algorithm>

#include <QDebug>
#include <QKeyEvent>
#include <QString>
#include <QString>
#include <QCursor>

#include <typeinfo>


// now include Scene.h, to define Window::initializeGL() which calls things from the scene()
// class Scene has only been forward declared to this point (?)
// if not #included we get:
//      member access into incomlpete type 'Scene' (whereever scene()->something is called)

#include "Scene.h"

Window::Window(QWindow *parent) : QOpenGLWindow(NoPartialUpdate, parent)
{
  //connect(&m_timer, SIGNAL(timeout()), this, SLOT(update()));

  //qDebug()<< format().swapInterval();

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
  if (scene())
    scene()->resize(_w, _h);
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
    // update() registers keys, add/remove from inputManager containers

    // convert Mouse window coordinates to have origin (0,0) at center of image
    QPoint _localMousePos = this->mapFromGlobal(QCursor::pos());
    _localMousePos.setX(_localMousePos.x() - (this->width()/2));
    _localMousePos.setY((_localMousePos.y() - (this->height()/2)) * -1);


    inputManager::update(_localMousePos);

    // handle key press events

    //if(inputManager::keyPressed(Qt::Key_Alt ) && inputManager::buttonPressed(Qt::LeftButton))
    // Handle Mouse button states
    // LeftButton
    if(inputManager::buttonTriggered(Qt::LeftButton))
    {
        m_inputManger.setMouseTriggeredPosition();
        scene()->m_arcCamera.arcBallStart();
    }

    if(inputManager::buttonPressed(Qt::LeftButton))
    {
        int _radius = std::min(this->width(), this->height()) / 2;
        scene()->m_arcCamera.rotateArcBall(m_inputManger.mousePosition(), m_inputManger.mouseTriggeredPosition(), _radius);
    }
    if(inputManager::buttonReleased(Qt::LeftButton))
    {

    }

    // RightButton
    if(inputManager::buttonTriggered(Qt::RightButton))
    {

    }

    if(inputManager::buttonPressed(Qt::RightButton))
    {

    }
    if(inputManager::buttonReleased(Qt::RightButton))
    {

    // MiddleButton
    }
    if(inputManager::buttonTriggered(Qt::MidButton))
    {

    }

    if(inputManager::buttonPressed(Qt::MidButton))
    {

    }
    if(inputManager::buttonReleased(Qt::MidButton))
    {

    }

    // handle Keyboard key states
    if(inputManager::keyPressed(Qt::Key_Up))
    {
        auto a = scene()->getShapeFromIndex(0);
        a->translate(QVector3D(0,0.4,0));
    }

    if(inputManager::keyPressed(Qt::Key_Down))
    {
        auto a = scene()->getShapeFromIndex(0);
        a->translate(QVector3D(0,-0.4,0));
    }
    if(inputManager::keyPressed(Qt::Key_Left))
    {
        auto a = scene()->getShapeFromIndex(0);
        a->translate(QVector3D(0,0,0.4));
    }
    if(inputManager::keyPressed(Qt::Key_Right))
    {
        auto a = scene()->getShapeFromIndex(0);
        a->translate(QVector3D(0,0,-0.4));
    }
    if(inputManager::keyPressed(Qt::Key_0))
    {
        auto a = scene()->getShapeFromIndex(0);
        a->translate(QVector3D(-0.4,0,0));
    }
    if(inputManager::keyPressed(Qt::Key_1))
    {
        auto a = scene()->getShapeFromIndex(0);
        a->translate(QVector3D(0.4,0,0));
    }
    if(inputManager::keyPressed(Qt::Key_W))
    {
        //scene()->m_camera.translate(0.0, 0.0, 0.03);
        // QQuaternion::fromAxisAndAngle(_axis, _angle);
//        QQuaternion _a = QQuaternion::fromAxisAndAngle(QVector3D(1,0,0), 10.0);
//        scene()->m_arcCamera.rotate(_a);
    }
    if(inputManager::keyPressed(Qt::Key_S))
    {
//        QQuaternion _a = QQuaternion::fromAxisAndAngle(QVector3D(1,0,0), -10.0);
//        scene()->m_arcCamera.rotate(_a);
    }
    if(inputManager::keyPressed(Qt::Key_A))
    {
        //scene()->m_arcCamera.rotateAroundPoint_G(-15.0, QVector3D(0,1,0));

    }
    if(inputManager::keyPressed(Qt::Key_D))
    {
        //scene()->m_arcCamera.rotateAroundPoint_G(15.0, QVector3D(0,1,0));
    }
    if(inputManager::keyPressed(Qt::Key_Q))
    {
    }
    if(inputManager::keyPressed(Qt::Key_A))
    {
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
      qDebug()<<event;
      inputManager::registerKeyPress(event->key());
      //inputManager::foo();
    }


    // Simple QKeyEvent switch
    if (event->isAutoRepeat())
    {
      qDebug("event isAutoRepeat");
    }
    else
    {
//      qDebug()<< "keyPressedT: "<< event->text();
//      qDebug()<< "keyPressedK: "<< event->key();
      switch(event->key())
      {
        case Qt::Key_Up:
          //scene()->m_arcCamera.rotateAroundPoint(15.0, scene()->m_arcCamera.right());
            break;
        case Qt::Key_Down:
          //scene()->m_arcCamera.rotateAroundPoint(-15.0, scene()->m_arcCamera.right());
          break;
        case Qt::Key_Left:
          //scene()->m_arcCamera.rotateAroundPoint(-15.0, QVector3D(0,1,0));
            break;
        case Qt::Key_Right:
          //scene()->m_arcCamera.rotateAroundPoint(15.0, QVector3D(0,1,0));
            break;
      case Qt::Key_J:
          scene()->m_arcCamera.rotate(QQuaternion::fromAxisAndAngle(QVector3D(1,0,0), 10.0));
          //scene()->m_arcCamera.rotateAroundPoint(-15.0, QVector3D(0,1,0));
          break;
      case Qt::Key_K:
           scene()->m_arcCamera.rotate(QQuaternion::fromAxisAndAngle(QVector3D(1,0,0), -10.0));
          //scene()->m_arcCamera.rotateAroundPoint(15.0, QVector3D(0,1,0));
          break;
      case Qt::Key_I:
           scene()->m_arcCamera.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,1,0), 10.0));
          //scene()->m_arcCamera.rotateAroundPoint(15.0, QVector3D(0,1,0));
          break;
      case Qt::Key_M:
           scene()->m_arcCamera.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,1,0), -10.0));
          //scene()->m_arcCamera.rotateAroundPoint(15.0, QVector3D(0,1,0));
          break;
      default: break;
      }
    }
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

void Window::mousePressEvent(QMouseEvent *event)
{
  qDebug()<<"event:"<< event;
  inputManager::registerMousePress(event->button());
}

void Window::mouseReleaseEvent(QMouseEvent *event)
{
  inputManager::registerMouseRelease(event->button());
}

void Window::printVersionInformation()
{
  QString glType;
  QString glVersion;
  QString glProfile;

  // Get Version Information
  glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
  glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

  // Get Profile Information
#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
  switch (format().profile())
  {
    CASE(NoProfile);
    CASE(CoreProfile);
    CASE(CompatibilityProfile);
  }
#undef CASE

  // qPrintable() will print our QString w/o quotes around it.
  qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}
