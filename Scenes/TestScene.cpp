#include "TestScene.h"

#include <string.h>
#include <iostream>

TestScene::TestScene()
  : Scene(), map_width(256), map_height(256), map(new float[map_width * map_height]), current_pos({ -1.5f, -1.0f, -6.0f }),
    key_w(false), key_a(false), key_s(false), key_d(false), key_space(false), key_shift(false), wireframe(false),
    n_verticies_map(map_width * map_height * 3), n_indicies_map(map_width * map_height * 6)
{
  memset(map, 0.0f, map_width * map_height);
}

TestScene::~TestScene()
{
}


void TestScene::key(int key, int scancode, int action, int mods)
{
  if (action == GLFW_PRESS)
    switch(key)
    {
    case 'W':
      key_w = true;
      break;
    case 'A':
      key_a = true;
      break;
    case 'S':
      key_s = true;
      break;
    case 'D':
      key_d = true;
      break;
    case GLFW_KEY_SPACE:
      key_space = true;
      break;
    case GLFW_KEY_LEFT_SHIFT:
      key_shift = true;
      break;
    case '1':
      wireframe = !wireframe;
      break;
    }
  else if (action == GLFW_RELEASE)
    switch(key)
    {
    case 'W':
      key_w = false;
      break;
    case 'A':
      key_a = false;
      break;
    case 'S':
      key_s = false;
      break;
    case 'D':
      key_d = false;
      break;
    case GLFW_KEY_SPACE:
      key_space = false;
      break;
    case GLFW_KEY_LEFT_SHIFT:
      key_shift = false;
      break;
    }
}

void TestScene::destroy()
{
  glDeleteBuffers(1, &map_vbo);
  glDeleteBuffers(1, &map_ibo);
}

void TestScene::init()
{
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  GLfloat verticies_map[n_verticies_map];
  GLushort indicies_map[n_indicies_map];

  unsigned i = 0;
  for(unsigned x = 0; x < map_width; ++x)
    for(unsigned y = 0; y < map_height; ++y)
    {
      verticies_map[i++] = x * 10;
      verticies_map[i++] = map[(y * map_width) + x];
      verticies_map[i++] = y * 10;
    }

  i = 0;
  for(unsigned x = 0; x < map_width - 1; ++x)
    for(unsigned y = 0; y < map_height - 1; ++y)
    {
      indicies_map[i++] = (y * map_height) + x;
      indicies_map[i++] = (y * map_height) + x + 1;
      indicies_map[i++] = (y * map_height) + x + 257;
      indicies_map[i++] = (y * map_height) + x + 257;
      indicies_map[i++] = (y * map_height) + x + 256;
      indicies_map[i++] = (y * map_height) + x;
    }

  glGenBuffers(1, &map_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, map_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticies_map), verticies_map, GL_STATIC_DRAW);
  glVertexPointer(3, GL_FLOAT, 0, (GLvoid*)0);

  glGenBuffers(1, &map_ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, map_ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies_map), indicies_map, GL_STATIC_DRAW);
}

void TestScene::render(double delta, int width, int height)
{
  if (key_w)     current_pos.z += delta * 50.0f;
  if (key_s)     current_pos.z -= delta * 50.0f;
  if (key_a)     current_pos.x += delta * 50.0f;
  if (key_d)     current_pos.x -= delta * 50.0f;
  if (key_space) current_pos.y -= delta * 5.0f;
  if (key_shift) current_pos.y += delta * 5.0f;

  float ar = (float)width / (float)height;

  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-ar, ar, -1.0, 1.0, 2.0, 500.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3d(1.0, 1.0, 1.0);
  glLoadIdentity();

  glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);

  glTranslatef(current_pos.x, current_pos.y, current_pos.z);

  glEnableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, map_vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, map_ibo);
  glDrawElements(GL_TRIANGLES, n_indicies_map, GL_UNSIGNED_SHORT, NULL);
  glDisableClientState(GL_VERTEX_ARRAY);
}
