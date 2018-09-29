
#ifndef WINDOW_H
#define WINDOW_H

// Qt
#include <QOpenGLWindow>
#include <QTimer>
#include <QElapsedTimer>
//#include  <QKeyEvent>

// Project
#include "inputManager.h"
#include "dynamics/dynamicsWorld.h"


// forward declare class Scene, so Window can be constructed with the Scene null-pointer
//class AbstractScene;
class Scene;

class Window : public QOpenGLWindow
{
  Q_OBJECT

public:
  Window(QWindow *parent = nullptr);
  Scene* scene() const;
  void setScene(Scene *_scene);
  Vertex* passSceneData();

protected slots:
    void teardownGL();
    void loop();
    void testerTimer();
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
  Scene *m_scene = nullptr;
  QTimer m_timer;
  QElapsedTimer m_elpasedTimer;
  inputManager m_inputManger;

};

#endif // WINDOW_H
