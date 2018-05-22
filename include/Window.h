
#ifndef WINDOW_H
#define WINDOW_H

// Qt
#include <QOpenGLWindow>
#include <QTimer>
#include <QElapsedTimer>
//#include  <QKeyEvent>

// Project
#include "AbstractScene.h"
#include "inputManager.h"

// forward declare class Scene, to be Window can be constructed with the Scene null-pointer
class Scene;

class Window : public QOpenGLWindow
{
  Q_OBJECT

public:
  Window(QWindow *parent = 0);
  Scene* scene() const;
  void setScene(Scene *_scene);

protected slots:
    void teardownGL();
    void update();
    void printVersionInformation();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int _w, int _h);

private:
  //make m_scene ptr of type *Scene because, to be able to access member of the actual scene.
  Scene *m_scene = nullptr;
  QTimer m_timer;
  inputManager m_inputManger;

};

#endif // WINDOW_H
