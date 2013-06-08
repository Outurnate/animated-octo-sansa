#ifndef SCENE_H
#define SCENE_H

class Scene
{
public:
  virtual void init() = 0;
  virtual void key(int key, int scancode, int action, int mods) = 0;
  virtual void render(double delta, int width, int height) = 0;
  virtual void destroy() = 0;
};

#endif
