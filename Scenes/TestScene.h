#ifndef TESTSCENE_H
#define TESTSCENE_H

#define GLFW_INCLUDE_GLU
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <FTGL/ftgl.h>
#include <glm/glm.hpp>

#include "Scene.h"

struct point3f { float x, y, z; };

class TestScene : public Scene
{
private:
  unsigned map_width, map_height;
  float* map;
  glm::vec3* map_normal;
  point3f current_pos;
  bool key_w, key_a, key_s, key_d, key_space, key_shift, wireframe, lighting;
  GLuint map_vbo, map_ibo, terrain_vert, terrain_frag, terrain_prog;
  size_t n_verticies_map;
  size_t n_indicies_map;
  double dx, dy;
  FTGLPixmapFont font_AverageMono;
public:
  TestScene();
  virtual ~TestScene();

  virtual void init(GLFWwindow* window);
  virtual void key(GLFWwindow* window, int key, int scancode, int action, int mods);
  virtual void mouse(GLFWwindow* window, double x, double y);
  virtual void render(GLFWwindow* window, double delta, int width, int height);
  virtual void destroy(GLFWwindow* window);
};

#endif
