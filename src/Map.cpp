#include "Map.hpp"

Map::Map(unsigned width, unsigned height) :
  typeMap(0), width(width), height(height)
{
  typeMap = new TerrainType[width * height];
}

Map::~Map()
{
  delete[] typeMap;
}

TerrainType& Map::operator()(const unsigned x, const unsigned y)
{
  return typeMap[(y * width) + x];
}

void Map::UpdateTexture(GLuint id)
{
}
