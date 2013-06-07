#ifndef TESTSCENE_H
#define TESTSCENE_H

#include "Scene.h"

class TestScene : public Scene
{
public:
  TestScene();
  virtual ~TestScene();

  void key(int key, int scancode, int action, int mods);
  void render(double delta, int width, int height);
};

#endif
