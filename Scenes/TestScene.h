#ifndef TESTSCENE_H
#define TESTSCENE_H

#define GLFW_INCLUDE_GLU
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <FTGL/ftgl.h>
#include <glm/glm.hpp>
#include <vector>

#include "Scene.h"
#include "TerrainChunk.h"

struct point3f { float x, y, z; };

class TestScene : public Scene
{
private:
  float* map;
  point3f current_pos;
  bool key_w, key_a, key_s, key_d, key_space, key_shift, wireframe, lighting;
  GLuint terrain_vert, terrain_frag, terrain_prog, tex_grass_A_diffuse, tex_grass_B_diffuse, tex_dirt_A_diffuse, tex_dirt_B_diffuse, tex_stone_A_diffuse, tex_stone_B_diffuse;
  double dx, dy;
  FTGLPixmapFont font_AverageMono;
  std::vector<TerrainChunk> chunks;
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
