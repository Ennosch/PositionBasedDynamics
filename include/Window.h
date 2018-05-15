
#ifndef WINDOW_H
#define WINDOW_H

// Qt
#include <QOpenGLWindow>
#include <QTimer>
#include <QDebug>

// Project
#include "AbstractScene.h"

// forward declare class Scene, to be Window can be constructed with the Scene null-pointer
class Scene;

class Window : public QOpenGLWindow
{

public:
  Window(QWindow *parent = 0);
  Scene* scene() const;
  void setScene(Scene *_scene);

protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int _w, int _h);

private:
  //make m_scene ptr of type *Scene because, to be able to access member of the actual scene.
  Scene *m_scene = nullptr;
  QTimer m_timer;

};

#endif // WINDOW_H
