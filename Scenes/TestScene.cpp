#include "TestScene.h"

#include <sstream>
#include <string.h>
#include <iostream>
#include <tgmath.h>
#include <glm/gtc/noise.hpp>

TestScene::TestScene()
  : Scene(), map_width(128), map_height(128), map(new float[map_width * map_height]), map_normal(new glm::vec3[map_width * map_height * 3]),
    current_pos({ 1.5f, 100.0f, 6.0f }),
    key_w(false), key_a(false), key_s(false), key_d(false), key_space(false), key_shift(false), wireframe(false), lighting(true),
    n_verticies_map(map_width * map_height * 12), n_indicies_map(map_width * map_height * 6), font_AverageMono("AverageMono.ttf")
{
  for(unsigned x = 0; x < map_width; ++x)
    for(unsigned y = 0; y < map_height; ++y)
      map[(y * map_width) + x] = (glm::simplex(glm::vec4(x / 8.0f, y / 8.0f, 0.5f, 0.5f)) * 10.0f) + (glm::simplex(glm::vec4(x / 32.0f, y / 32.0f, 0.5f, 0.5f)) * 100.0f);
  unsigned i = 0;
  for(unsigned x = 0; x < map_width; ++x)
    for(unsigned y = 0; y < map_height; ++y)
      map_normal[i++] = glm::normalize(glm::cross(
      ((x == 0)                ? glm::vec3(x, map[(y * map_width) + x], y) : glm::vec3(x - 1, map[(y       * map_width) + x - 1], y)) -
      ((x == (map_width  - 1)) ? glm::vec3(x, map[(y * map_width) + x], y) : glm::vec3(x + 1, map[(y       * map_width) + x + 1], y)),
      ((y == 0)                ? glm::vec3(x, map[(y * map_width) + x], y) : glm::vec3(x,     map[((y - 1) * map_width) + x],    y - 1)) -
      ((y == (map_height - 1)) ? glm::vec3(x, map[(y * map_width) + x], y) : glm::vec3(x,     map[((y + 1) * map_width) + x],    y + 1))));
}

TestScene::~TestScene()
{
}


void TestScene::key(GLFWwindow* window, int key, int scancode, int action, int mods)
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
    case '2':
      lighting = !lighting;
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

void TestScene::mouse(GLFWwindow* window, double x, double y)
{
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  dx += ((width / 2) - x) / 1000.0f;
  dy += ((height / 2) - y) / 1000.0f;
  glfwSetCursorPos(window, width / 2, height / 2);
}

void TestScene::destroy(GLFWwindow* window)
{
  glDeleteBuffers(1, &map_vbo);
  glDeleteBuffers(1, &map_ibo);
}

void TestScene::init(GLFWwindow* window)
{
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  glShadeModel(GL_FLAT);
  glEnable(GL_NORMALIZE);

  GLfloat ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f }; glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
  GLfloat diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f }; glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse);
  GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
  GLfloat position[] = { 100.0f, 100.0f, 0.0f, 1.0f }; glLightfv(GL_LIGHT0, GL_POSITION, position);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

  GLfloat verticies_map[n_verticies_map];
  GLushort indicies_map[n_indicies_map];

  // TODO: mixmap

  unsigned i = 0;
  for(unsigned x = 0; x < map_width; ++x)
    for(unsigned y = 0; y < map_height; ++y)
    {
      verticies_map[i++] = x * 10;
      verticies_map[i++] = map[(y * map_width) + x];
      verticies_map[i++] = y * 10;
      verticies_map[i++] = map_normal[(y * map_width) + x].x;
      verticies_map[i++] = map_normal[(y * map_width) + x].y;
      verticies_map[i++] = map_normal[(y * map_width) + x].z;
      verticies_map[i++] = 0; // R
      verticies_map[i++] = 0; // G
      verticies_map[i++] = 0; // B
      verticies_map[i++] = 0; // A
      verticies_map[i++] = 0; // S
      verticies_map[i++] = 0; // T
    }

  i = 0;
  for(unsigned x = 0; x < map_width - 1; ++x)
    for(unsigned y = 0; y < map_height - 1; ++y)
    {
      indicies_map[i++] = (y * map_height) + x;
      indicies_map[i++] = (y * map_height) + x + 1;
      indicies_map[i++] = (y * map_height) + x + map_height + 1;
      indicies_map[i++] = (y * map_height) + x + map_height + 1;
      indicies_map[i++] = (y * map_height) + x + map_height;
      indicies_map[i++] = (y * map_height) + x;
    }

  glGenBuffers(1, &map_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, map_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticies_map), verticies_map, GL_STATIC_DRAW);
  glVertexPointer(3, GL_FLOAT, 12 * sizeof(GLfloat), (GLvoid*)0);
  glNormalPointer(GL_FLOAT, 12 * sizeof(GLfloat), (GLvoid*)3);

  glGenBuffers(1, &map_ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, map_ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies_map), indicies_map, GL_STATIC_DRAW);
}

void TestScene::render(GLFWwindow* window, double delta, int width, int height)
{
  if (key_w)
  {
    current_pos.x += sin(dx) * delta * 100.0f;
    current_pos.z += cos(dx) * delta * 100.0f;
  }
  if (key_s)
  {
    current_pos.x -= sin(dx) * delta * 100.0f;
    current_pos.z -= cos(dx) * delta * 100.0f;
  }
  if (key_a)
  {
    current_pos.x += sin(dx + (M_PI / 2.0f)) * delta * 100.0f;
    current_pos.z += cos(dx + (M_PI / 2.0f)) * delta * 100.0f;
  }
  if (key_d)
  {
    current_pos.x -= sin(dx + (M_PI / 2.0f)) * delta * 100.0f;
    current_pos.z -= cos(dx + (M_PI / 2.0f)) * delta * 100.0f;
  }
  if (key_space) current_pos.y += delta * 200.0f;
  if (key_shift) current_pos.y -= delta * 200.0f;

  float ar = (float)width / (float)height;

  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glFrustum(-ar, ar, -1.0, 1.0, 2.0, 5120.0);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3d(1.0, 1.0, 0.0);

  glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);

  // TODO: use glm
  point3f dir({ (float)(cos(dy) * sin(dx)), (float)sin(dy), (float)(cos(dy) * cos(dx)) });
  point3f right({ (float)sin(dx - (M_PI / 2.0f)), 0.0f, (float)cos(dx - (M_PI / 2.0f)) });
  point3f up({ (float)((right.y * dir.z) - (right.z * dir.y)), (float)((right.z * dir.x) - (right.x * dir.z)), (float)((right.x * dir.y) - (right.y * dir.x)) });

  gluLookAt(current_pos.x,         current_pos.y,         current_pos.z,
	    current_pos.x + dir.x, current_pos.y + dir.y, current_pos.z + dir.z,
	    up.x,                  up.y,                  up.z);

  if (lighting)
  {
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHT0);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  }

  glEnableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, map_vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, map_ibo);
  glDrawElements(GL_TRIANGLES, n_indicies_map, GL_UNSIGNED_SHORT, NULL);
  glDisableClientState(GL_VERTEX_ARRAY);

  glDisable(GL_COLOR_MATERIAL);
  glDisable(GL_LIGHTING);

  glPixelTransferf(GL_RED_BIAS,  -1.0f);
  glPixelTransferf(GL_GREEN_BIAS, 0.0f);
  glPixelTransferf(GL_BLUE_BIAS, -1.0f);

  font_AverageMono.FaceSize(16);
  font_AverageMono.Render("Test Scene", -1, FTPoint(0.0f, 32.0f));
  font_AverageMono.Render(static_cast<std::ostringstream*>(&(std::ostringstream() << "Wireframe : " << wireframe))->str().c_str(), -1, FTPoint(0.0f, 16.0f));
  font_AverageMono.Render(static_cast<std::ostringstream*>(&(std::ostringstream() << width << "x" << height))->str().c_str(), -1, FTPoint(0.0f, 0.0f));
}
