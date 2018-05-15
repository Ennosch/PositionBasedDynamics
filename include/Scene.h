
#ifndef SCENE_H
#define SCENE_H

// Project
#include "AbstractScene.h"
#include "Window.h"
class Scene : public AbstractScene
{

public:
 Scene(Window *_window);
  ~Scene();
  void initialize();
  void paint();

};

#endif // SCENE_H
