#include "TerrainChunk.h"

#include "TerrainGenerator.h"

TerrainChunk::TerrainChunk(unsigned size, unsigned xoff, unsigned yoff)
  : size(size), xoff(xoff), yoff(yoff), map(new float[size * size]), n_verticies_map(size * size * 10), n_indicies_map(size * size * 6)
{
  generateTerrain(map, size);
}

TerrainChunk::~TerrainChunk()
{
}

void TerrainChunk::Load()
{
  GLfloat verticies_map[n_verticies_map];
  GLushort indicies_map[n_indicies_map];

  unsigned i = 0;
  for(unsigned x = 0; x < size; ++x)
    for(unsigned y = 0; y < size; ++y)
    {
      glm::vec3 n = glm::normalize(glm::cross(
        ((x == 0)          ? glm::vec3(x, map[(y * size) + x], y) : glm::vec3(x - 1, map[(y       * size) + x - 1], y)) -
        ((x == (size - 1)) ? glm::vec3(x, map[(y * size) + x], y) : glm::vec3(x + 1, map[(y       * size) + x + 1], y)),
        ((y == 0)          ? glm::vec3(x, map[(y * size) + x], y) : glm::vec3(x,     map[((y - 1) * size) + x],     y - 1)) -
        ((y == (size - 1)) ? glm::vec3(x, map[(y * size) + x], y) : glm::vec3(x,     map[((y + 1) * size) + x],     y + 1))));
      verticies_map[i++] = x + xoff; // X
      verticies_map[i++] = map[(y * size) + x]; // Y
      verticies_map[i++] = y + yoff; // Z
      verticies_map[i++] = n.x; // NX
      verticies_map[i++] = n.y; // NY
      verticies_map[i++] = n.z; // NZ
      verticies_map[i++] = 0; // R
      verticies_map[i++] = 0; // G
      verticies_map[i++] = 0; // B
      verticies_map[i++] = 0; // A
    }

  i = 0;
  for(unsigned x = 0; x < size - 1; ++x)
    for(unsigned y = 0; y < size - 1; ++y)
    {
      indicies_map[i++] = (y * size) + x;
      indicies_map[i++] = (y * size) + x + 1;
      indicies_map[i++] = (y * size) + x + size + 1;
      indicies_map[i++] = (y * size) + x + size + 1;
      indicies_map[i++] = (y * size) + x + size;
      indicies_map[i++] = (y * size) + x;
    }

  glGenBuffers(1, &map_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, map_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticies_map), verticies_map, GL_STATIC_DRAW);
  glVertexPointer(3, GL_FLOAT, 10 * sizeof(GLfloat), (GLvoid*)0);
  glNormalPointer(GL_FLOAT, 10 * sizeof(GLfloat), (GLvoid*)3);

  glGenBuffers(1, &map_ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, map_ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies_map), indicies_map, GL_STATIC_DRAW);
}

void TerrainChunk::Unload()
{
  glDeleteBuffers(1, &map_vbo);
  glDeleteBuffers(1, &map_ibo);
}

void TerrainChunk::Render()
{
  glEnableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, map_vbo);
  glVertexPointer(3, GL_FLOAT, 10 * sizeof(GLfloat), (GLvoid*)0);
  glNormalPointer(GL_FLOAT, 10 * sizeof(GLfloat), (GLvoid*)3);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, map_ibo);
  glDrawElements(GL_TRIANGLES, n_indicies_map, GL_UNSIGNED_SHORT, NULL);
  glDisableClientState(GL_VERTEX_ARRAY);
}
