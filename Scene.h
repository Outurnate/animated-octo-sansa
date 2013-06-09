#ifndef SCENE_H
#define SCENE_H

class Scene
{
public:
  virtual void init(GLFWwindow* window) = 0;
  virtual void key(GLFWwindow* window, int key, int scancode, int action, int mods) = 0;
  virtual void mouse(GLFWwindow* window, double x, double y) = 0;
  virtual void render(GLFWwindow* window, double delta, int width, int height) = 0;
  virtual void destroy(GLFWwindow* window) = 0;
};

#endif
