#ifndef TESTSCENE_H
#define TESTSCENE_H

#inlcude "Scene.h"

class TestScene : Scene
{
public:
  Scene();
  virtual ~Scene();

  void key(int key, int scancode, int action, int mods) = 0;
  void render(double delta, int width, int height) = 0;
};

#endif
