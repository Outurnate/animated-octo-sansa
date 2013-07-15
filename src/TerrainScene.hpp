#ifndef TERRAINSCENE_H
#define TERRAINSCENE_H

#define GLFW_INCLUDE_GLU
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <FTGL/ftgl.h>
#include <glm/glm.hpp>

#include "Scene.hpp"
#include "TerrainObject.hpp"

struct point3f { float x, y, z; };

class TerrainScene : public Scene
{
private:
  float* map;
  point3f current_pos;
  bool key_w, key_a, key_s, key_d, key_space, key_shift, wireframe, lighting;
  GLuint terrain_vert, terrain_frag, terrain_prog, tex_grass_A_diffuse, tex_grass_B_diffuse, tex_dirt_A_diffuse, tex_dirt_B_diffuse, tex_stone_A_diffuse, tex_stone_B_diffuse;
  double dx, dy;
  FTGLPixmapFont font_AverageMono;
  TerrainObject terrain;
  GLFWwindow *window;
public:
  TerrainScene(GLFWwindow *window);
  virtual ~TerrainScene();

  virtual void init();
  virtual void key(int key, int scancode, int action, int mods);
  virtual void mouse(double x, double y);
  virtual void render(double delta, int width, int height);
  virtual void destroy();
};

#endif
