#ifndef GLWIDGET_H
#define GLWIDGET_H

// Qt
#include <QOpenGLWindow>
#include <QOpenGLWidget>
#include <QPainter>
#include <QTimer>
#include <QElapsedTimer>
#include <QTime>
#include <QCoreApplication>
#include <QThread>
#include  <QKeyEvent>

// Project
#include "inputManager.h"
#include "dynamics/dynamicsWorld.h"


// forward declare class Scene, so Window can be constructed with the Scene null-pointer
//class AbstractScene;
class Scene;

class Scene;

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
//    GLWidget();
    GLWidget(QWindow *parent = nullptr);
    void setScene(Scene *_scene);

     Scene* scene() const;
     void countFPS();

protected slots:
    void update();
    void loop();
    void processInput();

    void printVersionInformation();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int _w, int _h);

private:
  double lag;
  float fpsRate, fpsCount;
  double previous, current, elapsed, second, render;
  QTimer m_timer;
  QElapsedTimer m_elpasedTimer;
  Scene *m_scene = nullptr;
  inputManager m_inputManger;
};

#endif // GLWIDGET_H
