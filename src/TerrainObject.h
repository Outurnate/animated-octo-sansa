#ifndef TERRAINOBJECT_H
#define TERRAINOBJECT_H

#include <vector>

#include "TerrainChunk.h"

class TerrainObject
{
private:
  std::vector<TerrainChunk> chunks;
  float *map;
public:
  TerrainObject(unsigned nc, unsigned ncp); // l/w of chunks, size of chunk as pow of 2
  virtual ~TerrainObject();

  void Load();
  void Unload();
  void Render();
};

#endif
