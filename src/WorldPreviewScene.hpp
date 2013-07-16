#ifndef WORLDPREVIEWSCENE_H
#define WORLDPREVIEWSCENE_H

#include "Scene.hpp"

class WorldPreviewScene : public Scene
{
private:
  unsigned iwidth, iheight;
  bool dirty;
public:
  WorldPreviewScene();
  virtual ~WorldPreviewScene();

  virtual void init();
  virtual void key(int key, int scancode, int action, int mods);
  virtual void mouse(double x, double y);
  virtual void render(double delta, int width, int height);
  virtual void destroy();
};

#endif
