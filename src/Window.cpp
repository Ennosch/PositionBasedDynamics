#include <typeinfo>

// STL
#include <iostream>
#include <algorithm>

// QT
#include <QDebug>
#include <QKeyEvent>
#include <QString>
#include <QString>
#include <QCursor>

// Project
#include "utils.h"


// now include Scene.h, to define Window::initializeGL() which calls things from the scene()
// class Scene has only been forward declared to this point (?)
// if not #included we get:
//      member access into incomlpete type 'Scene' (whereever scene()->something is called)

#include "Scene.h"

Window::Window(QWindow *parent) : QOpenGLWindow(NoPartialUpdate, parent)
{
  //        sender      signal          reviecer    member
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(loop()));
//    connect(&m_timer, SIGNAL(timeout()), this, SLOT(update()));
//    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));

//  if(format().swapInterval() == -1)
//  {
//      // V_blank synchronization not available (tearing likely to happen)
//      qDebug("Swap Buffers at v_blank not available: refresh at approx 60fps.");
//      m_timer.setInterval(17);
//  }
//  else
//  {
//      // V_blank synchronization available
//      m_timer.setInterval(0);
//  }
//  m_timer.setInterval();
  m_timer.setInterval(0);
  m_elpasedTimer.start();
  second = 0;
  previous = m_elpasedTimer.elapsed();
  lag = 0.0;

  m_timer.start();
}

//Window::~Window()
//{
//    qDebug()<<"bye1";
////    m_elpasedTimer.~QElapsedTimer();
//    QOpenGLWindow::~QOpenGLWindow();
//    qDebug()<<"bye";
//}


void Window::processInput()
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
//        scene()->m_arcCamera.rotateArcBall(m_inputManger.mousePosition(), m_inputManger.mouseTriggeredPosition(), _radius);
        scene()->m_arcCamera.orbit(m_inputManger.mousePosition(), m_inputManger.mouseTriggeredPosition());
//        scene()->m_arcCamera.orbit(m_inputManger.mousePosition(), m_inputManger.mouseTriggeredPosition());
//        scene()->m_arcCamera.correction2(m_inputManger.mousePosition(), m_inputManger.mouseTriggeredPosition());
    }

    // RightButton
    if(inputManager::buttonTriggered(Qt::RightButton))
    {
        scene()->m_arcCamera.trackStart(m_inputManger.mousePosition());
    }

    if(inputManager::buttonPressed(Qt::RightButton))
    {
        scene()->m_arcCamera.dolly(m_inputManger.mousePosition());
    }

    if(inputManager::buttonTriggered(Qt::MidButton))
    {
        scene()->m_arcCamera.trackStart(m_inputManger.mousePosition());
    }

    if(inputManager::buttonPressed(Qt::MidButton))
    {
        scene()->m_arcCamera.track(m_inputManger.mousePosition());
    }

    // handle Keyboard key states
    if(inputManager::keyPressed(Qt::Key_Up))
    {
//          scene()->m_arcCamera.translate(QVector3D(0,0,1));
    }

    if(inputManager::keyPressed(Qt::Key_Down))
    {

    }
    if(inputManager::keyPressed(Qt::Key_Left))
    {
        qDebug()<<"WHER IS: "<<scene()->dynamicsWorld()->m_Particles[0]->x;
    }
    if(inputManager::keyPressed(Qt::Key_Right))
    {
//        scene()->dynamicsWorld()->m_Particles[0]->x = QVector3D(0,12.5,0);
    }
    if(inputManager::keyPressed(Qt::Key_7))
    {
        scene()->m_SceneObjects[2]->rotate(QQuaternion::fromAxisAndAngle(1,0,0,15));
    }
    if(inputManager::keyPressed(Qt::Key_8))
    {
        scene()->m_SceneObjects[2]->rotate(QQuaternion::fromAxisAndAngle(1,0,0,-15));
    }
    if(inputManager::keyPressed(Qt::Key_4))
    {
        scene()->m_SceneObjects[2]->rotate(QQuaternion::fromAxisAndAngle(0,1,0,15));
    }
    if(inputManager::keyPressed(Qt::Key_5))
    {
        scene()->m_SceneObjects[2]->rotate(QQuaternion::fromAxisAndAngle(0,1,0,-15));
    }
    if(inputManager::keyPressed(Qt::Key_1))
    {
        scene()->m_SceneObjects[2]->translate(QVector3D(0,0,0.5));
    }
    if(inputManager::keyPressed(Qt::Key_2))
    {
        scene()->m_SceneObjects[2]->translate(QVector3D(0,0,-0.5));
    }
    if(inputManager::keyPressed(Qt::Key_0))
    {

    }
    if(inputManager::keyPressed(Qt::Key_W))
    {
        scene()->m_SceneObjects[0]->translate(QVector3D(0,0.1,0));
    }
    if(inputManager::keyPressed(Qt::Key_S))
    {
        scene()->m_SceneObjects[0]->translate(QVector3D(0,-0.1,0));
    }
    if(inputManager::keyPressed(Qt::Key_A))
    {
        scene()->m_SceneObjects[0]->translate(QVector3D(-0.1,0,0));
    }
    if(inputManager::keyPressed(Qt::Key_D))
    {
       scene()->m_SceneObjects[0]->translate(QVector3D(0.1,0,0));
    }
    if(inputManager::keyPressed(Qt::Key_Q))
    {
//        scene()->m_SceneObjects[0]->translate(QVector3D(0,0,0.1));
    }
    if(inputManager::keyPressed(Qt::Key_E))
    {
        scene()->m_SceneObjects[0]->translate(QVector3D(0,0,-0.1));
    }
}

Scene *Window::scene() const
{
  return m_scene;
}

void Window::setScene(Scene *_scene)
{
    m_scene = _scene;
}

Vertex* Window::passSceneData()
{
    // Loop over scene's ModelMap
    ModelMap::const_iterator it = scene()->m_ModelPool.begin();
    while(it != scene()->m_ModelPool.end())
    {
//        std::cout<<it->first << ": "<<it->second<< std::endl;
        ModelPtr model = it->second;
        for(int i = 0; i < model->getNumShapes() ; i++ )
        {
            ShapePtr shape = model->getShape(i);
            Vertex *shapeData = shape->data();
            int vertCount = shape->getNumVertices();
            for(int i = 0; i < vertCount; i++)
            {
                //qDebug()<<shapeData[i].Position;
            }
        }
        it++;
    }
}

void Window::paintEvent(QPaintEvent *event)
{
        paintGL();
        QPainter painter(this);

        QString fps = QString::number(fpsRate) + " fps";
        QString a = QString::number(second);
        QString b = QString::number(render);
        painter.drawText(QRect(5, 5, 100, 50), fps);
        painter.drawText(QRect(5, 19, 100, 50), a);
        painter.drawText(QRect(5, 33, 100, 50), b);
}

void Window::initializeGL()
{

    qDebug("calling init Wind");
//    qDebug()<<context();
  // Init OpenGL Backend  (QOpenGLFunctions)
  if (scene())
    scene()->initialize();

  printVersionInformation();

  // Init QtWindow specific things, connectionQtype
  // sender signal receive, method
  connect(context(), SIGNAL(aboutToBeDestroyed()), this, SLOT(teardownGL()), Qt::DirectConnection);

  // QOpenGLWindow::frameSwapped();
  // This signal is emitted after the potentially blocking buffer swap has been done. Applications
  // that wish to continuously repaint synchronized to the vertical refresh, should issue an update()
  // upon this signal. This allows for a much smoother experience compared to the traditional usage of timers.
//  connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
  //--- WIP ----
  // also pass DynamicsWorld to the scene
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


void Window::countFPS()
{
    current = m_elpasedTimer.elapsed();
    elapsed = current - previous;

    previous = current;

    lag += elapsed;

    if(second < 1000)
    {
        second += elapsed;
        fpsCount += 1;
    }
    else
    {
//        qDebug()<<"calc"<<elapsed;
        fpsRate = fpsCount;
        fpsCount = 0;
        second = 0;
    }

}

void Window::update()
{
    QOpenGLWindow::update();
}

void Window::loop()
{
    countFPS();

    processInput();

    int i = 0;

    while(lag >= 10)
    {
        lag -= 10;
        i++;
    }

    QThread::msleep(16);

    // physics
//    scene()->dynamicsWorld()->update();

    if(render < 16)
    {
        render += elapsed;
    }
    else
    {
        render = 0;
        QOpenGLWindow::update();
    }
}

void Window::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<"window:";
    printVersionInformation();
    if (event->isAutoRepeat())
    {
      event->ignore();
    }
    else
    {
//      qDebug()<<event;
      inputManager::registerKeyPress(event->key());
      //inputManager::foo();
    }


    // Simple QKeyEvent switch
    if (event->isAutoRepeat())
    {
//      qDebug("event isAutoRepeat");
    }
    else
    {
//      qDebug()<< "keyPressedT: "<< event->text();
//      qDebug()<< "keyPressedK: "<< event->key();
      switch(event->key())
      {
        case Qt::Key_Up:
//          scene()->m_arcCamera.rotate(15,1,0,0);
//            scene()->m_arcCamera.setTranslation(QVector3D(0,0,12));
//            scene()->m_SceneObjects[1]->translate(QVector3D(0,0.5,0));
          scene()->dynamicsWorld()->info();
            break;
        case Qt::Key_Down:
          scene()->m_SceneObjects[1]->translate(QVector3D(0,-0.5,0));
          break;
        case Qt::Key_Left:
//          scene()->m_arcCamera.rotate(-15,0,1,0);
//          scene()->m_SceneObjects[1]->translate(QVector3D(0.5,0,0));
            break;
        case Qt::Key_Right:
          scene()->dynamicsWorld()->update();

      case Qt::Key_J:
//          scene()->m_arcCamera.rotate(15,0,0,1);
          //scene()->m_arcCamera.rotateAroundPoint(-15.0, QVector3D(0,1,0));
          break;
      case Qt::Key_K:
//           scene()->m_arcCamera.rotate(QQuaternion::fromAxisAndAngle(QVector3D(1,0,0), -10.0));
          //scene()->m_arcCamera.rotateAroundPoint(15.0, QVector3D(0,1,0));
          scene()->m_arcCamera.rotate(-15,0,0,1);
          break;
      case Qt::Key_I:
//           scene()->m_arcCamera.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,1,0), 10.0));
          //scene()->m_arcCamera.rotateAroundPoint(15.0, QVector3D(0,1,0));
          scene()->m_arcCamera.info();
//          qDebug()<<"transform[0]: "<<scene()->m_SceneObjects[0]->m_Transform.toMatrix()<<"transform[1]: "<<scene()->m_SceneObjects[1]->m_Transform.toMatrix();;
          break;
      case Qt::Key_M:
          //scene()->m_arcCamera.rotateAroundPoint(15.0, QVector3D(0,1,0));
          break;
      case Qt::Key_N:
          //scene()->m_arcCamera.rotateAroundPoint(15.0, QVector3D(0,1,0));
          break;
      case Qt::Key_F:
           scene()->m_arcCamera.reset(m_inputManger.mousePosition());
          break;

      case Qt::Key_7:
//           scene()->m_arcCamera.translatePivot(QVector3D(1,0,0));
//           scene()->m_arcCamera.translate(QVector3D(1,0,0));
//             scene()->m_SceneObjects[0]->translate(QVector3D(1,0,0));
          break;

      case Qt::Key_8:
//           scene()->m_arcCamera.translatePivot(QVector3D(-1,0,0));
//           scene()->m_arcCamera.translate(QVector3D(-1,0,0));
          break;
      case Qt::Key_4:
//          scene()->m_arcCamera.translatePivot(QVector3D(0,1,0));
//           scene()->m_arcCamera.translate(QVector3D(0,1,0));
        break;
      case Qt::Key_5:
//           scene()->m_arcCamera.translatePivot(QVector3D(0,-1,0));
//           scene()->m_arcCamera.translate(QVector3D(0,-1,0));
        break;
      case Qt::Key_1:
//           scene()->m_arcCamera.translatePivot(QVector3D(0,0,1));
//           scene()->m_arcCamera.translate(QVector3D(0,0,1));
        break;
      case Qt::Key_2:
//           scene()->m_arcCamera.translatePivot(QVector3D(0,0,-1));
//           scene()->m_arcCamera.translate(QVector3D(0,0,-1));
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
  inputManager::registerMousePress(event->button());
  QPointF pos = event->pos();

  float pixelNDCx = (pos.x() + 0.5) / this->width();
  float pixelNDCy = (pos.y() + 0.5) / this->height();

  float pixelScreenX = 2 * pixelNDCx - 1;
  float pixelScreenY = 1 - 2 * pixelNDCy;

  scene()->pickObject(pixelScreenX, pixelScreenY);
//  scene()->rayIt(pixelScreenX, pixelScreenY);
}

void Window::mouseReleaseEvent(QMouseEvent *event)
{
  inputManager::registerMouseRelease(event->button());
}

void Window::wheelEvent(QWheelEvent *event)
{
    QVector3D camTranslation = scene()->m_arcCamera.translation();
    float t = event->pixelDelta().y();

    QVector3D nextCamTranslation = camTranslation + (QVector3D(0,0,1) * t * 0.05);
    scene()->m_arcCamera.setTranslation(nextCamTranslation);
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

#include "moc_Window.cpp"


