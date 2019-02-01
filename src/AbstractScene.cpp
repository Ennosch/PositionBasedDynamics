
// Project
#include "AbstractScene.h"
#include <QDebug>

#include <iostream>


void AbstractScene::foo()
{
    qDebug("foo foo ");
}

AbstractScene::AbstractScene(QOpenGLWindow *_window)
{
    m_window = _window;
}

//AbstractScene::AbstractScene(QOpenGLWidget *_widget)
//{
//    m_widget = _widget;
//}

AbstractScene::AbstractScene(GLWidget *_widget)
{
    m_widget = _widget;
}

AbstractScene::~AbstractScene()
{

}

QOpenGLWindow *AbstractScene::window() const
{
    return m_window;
}

GLWidget *AbstractScene::widget() const
{
    return m_widget;
}

QOpenGLContext *AbstractScene::context()
{
    qDebug()<<"CONTEXT QUERY";
  return window() ? window()->context() : 0;
}

const QOpenGLContext *AbstractScene::context() const
{
    qDebug()<<"CONTEXT QUERY 2 ";
  return window() ? window()->context() : 0;
}

void AbstractScene::initialize()
{
  qDebug()<<"init openglfucntions :";
  initializeOpenGLFunctions();
  qDebug()<<"init openglfucntions end";

//  const uint test = &this->glBindTexture;
//  printf("address of function funct() is : %p\n", this->glBindTexture());



}


