#include "WorldPreviewScene.hpp"

#define GLFW_INCLUDE_GLU
#include <GL/glew.h>
#include <GLFW/glfw3.h>

WorldPreviewScene::WorldPreviewScene()
  : iwidth(256), iheight(256), dirty(true), map(iwidth, iheight)
{
}

WorldPreviewScene::~WorldPreviewScene()
{
}

void WorldPreviewScene::init()
{
  GLuint tex;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void WorldPreviewScene::key(int key, int scancode, int action, int mods)
{
}

void WorldPreviewScene::mouse(double x, double y)
{
}

void WorldPreviewScene::render(double delta, int width, int height)
{
  unsigned cwidth  = (width < height) ? width :  (iwidth  * ((double)height / (double)iheight)),
           cheight = (width > height) ? height : (iheight * ((double)width  / (double)iwidth));

  glViewport((width / 2) - (cwidth / 2), (height / 2) - (cheight / 2), cwidth, cheight);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, iwidth, iheight, 0, 0, 1);
  glMatrixMode(GL_MODELVIEW);
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3d(1.0, 0.0, 0.0);

  glBegin(GL_TRIANGLES);
  glVertex2f(0.0f, 0.0f);
  glVertex2f(iwidth, 0.0f);
  glVertex2f(iwidth, iheight);
  glVertex2f(0.0f, 0.0f);
  glVertex2f(0.0f, iheight);
  glVertex2f(iwidth, iheight);
  glEnd();
}

void WorldPreviewScene::destroy()
{
}
