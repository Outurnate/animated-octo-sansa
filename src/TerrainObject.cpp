#include "TerrainObject.h"

#include <glm/gtc/noise.hpp>
#include <string.h>

TerrainObject::TerrainObject(unsigned nc, unsigned ncp)
{
  unsigned chunk_size = pow(2, ncp);
  unsigned size = chunk_size * nc;
  float *map = new float[size * size];
  for(unsigned x = 0; x < size; ++x)
    for(unsigned y = 0; y < size; ++y)
      map[(y * size) + x] = 1/*(
	glm::simplex(glm::vec2(x / 128.f, y / 128.f))
	) * 64 + 64*/;
  for(unsigned x = 0; x < size; x += chunk_size)
    for(unsigned y = 0; y < size; y += chunk_size)
    {
      //float *nmap = new float[chunk_size * chunk_size];
      //for (unsigned i = 0; i < chunk_size; ++i)
      //memcpy(nmap, map/* + (i * chunk_size)*/, chunk_size*chunk_size);
      chunks.push_back(TerrainChunk(chunk_size, x, y, map));
      //delete[] nmap;
    }
  delete[] map;
}

TerrainObject::~TerrainObject()
{
}

void TerrainObject::Load()
{
  for (unsigned i = 0; i < chunks.size(); ++i)
    chunks[i].Load();
}

void TerrainObject::Unload()
{
  for (unsigned i = 0; i < chunks.size(); ++i)
    chunks[i].Unload();
}

void TerrainObject::Render()
{
  for (unsigned i = 0; i < chunks.size(); ++i)
    chunks[i].Render();
}
