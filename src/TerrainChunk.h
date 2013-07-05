#ifndef TERRAINCHUNK_H
#define TERRAINCHUNK_H

#define GLFW_INCLUDE_GLU
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class TerrainChunk
{
private:
  float *map;
  unsigned size, xoff, yoff;
  GLuint map_vbo, map_ibo;
  size_t n_verticies_map;
  size_t n_indicies_map;
public:
  TerrainChunk(unsigned size, unsigned xoff, unsigned yoff);
  virtual ~TerrainChunk();

  void Load();
  void Unload();
  void Render();
};

#endif
