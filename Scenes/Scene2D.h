#ifndef SCENE2D_H
#define SCENE2D_H

#define GLFW_INCLUDE_GLU
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Scene.h"

class Scene2D : public Scene
{
public:
  Scene2D();
  virtual ~Scene2D();

  virtual void init(GLFWwindow* window);
  virtual void key(GLFWwindow* window, int key, int scancode, int action, int mods);
  virtual void mouse(GLFWwindow* window, double x, double y);
  virtual void render(GLFWwindow* window, double delta, int width, int height);
  virtual void destroy(GLFWwindow* window);
};

#endif
