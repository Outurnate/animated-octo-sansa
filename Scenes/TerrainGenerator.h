#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include <glm/gtc/noise.hpp>

void generateTerrain(float *map, unsigned size)
{
  for(unsigned x = 0; x < size; ++x)
    for(unsigned y = 0; y < size; ++y)
      map[(y * size) + x] = (
	glm::simplex(glm::vec2(x / 128.f, y / 128.f))
	) * 64 + 64;
}

#endif
