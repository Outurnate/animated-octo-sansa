#ifndef MAP_H
#define MAP_H

enum class TerrainType : unsigned char
{
  // 0000BBAA
  // A - generation type
  // B - terrain flags
  // 0 - Unused
  Plains    = 0x0 << 0,
  Mountains = 0x1 << 0,
  Ocean     = 0x2 << 0,
  UNUSED00  = 0x3 << 0,
  Sand      = 0x0 << 2,
  Path      = 0x1 << 2,
  UNUSED01  = 0x2 << 2,
  UNUSED02  = 0x3 << 2
};

class Map
{
private:
  TerrainType *typeMap;
  unsigned width, height;
public:
  Map(unsigned width, unsigned height);
  virtual ~Map();

  void UpdateTexture(GLuint id);

  TerrainType& operator()(const unsigned x, const unsigned y);
};

#endif
