#include "include/GLWidget.h"


#include "Scene.h"


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


//GLWidget::GLWidget()
//{

//}

GLWidget::GLWidget(QWindow *parent) : QOpenGLWidget()
{
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(loop()));
    m_timer.setInterval(0);
    m_elpasedTimer.start();
    second = 0;
    previous = m_elpasedTimer.elapsed();
    lag = 0.0;

    m_timer.start();
}

void GLWidget::setScene(Scene *_scene)
{
    m_scene = _scene;
}

Scene *GLWidget::scene() const
{
    return m_scene;
}

void GLWidget::update()
{
    QOpenGLWidget::update();
}

void GLWidget::loop()
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
        QOpenGLWidget::update();
    }
}

void GLWidget::countFPS()
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

void GLWidget::processInput()
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
}

void GLWidget::printVersionInformation()
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

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<"widget:";
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
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
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

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    inputManager::registerMousePress(event->button());
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    inputManager::registerMouseRelease(event->button());
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    QVector3D camTranslation = scene()->m_arcCamera.translation();
    float t = event->pixelDelta().y();

    QVector3D nextCamTranslation = camTranslation + (QVector3D(0,0,1) * t * 0.05);
    scene()->m_arcCamera.setTranslation(nextCamTranslation);
}


void GLWidget::initializeGL()
{
    qDebug()<<"init w";
    if(scene())
        scene()->initialize();
}

void GLWidget::paintGL()
{
    if (scene())
        scene()->paint();
}

void GLWidget::resizeGL(int _w, int _h)
{
    qDebug()<<"resize w"<<_w<<_h;
    if (scene())
    {
//        qDebug()<<"resize 2 w"<<_w<<_h;
//      scene()->resize(_w, _h);
//      qDebug()<<"resize  3 w"<<_w<<_h;
    }
}
