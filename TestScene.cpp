#include "TestScene.h"

TestScene::TestScene()
  : Scene()
{
}

TestScene::~TestScene()
{
}

void TestScene::init()
{
}

void TestScene::key(int key, int scancode, int action, int mods)
{
}

void TestScene::render(double delta, int width, int height)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3d(1.0, 1.0, 1.0);
  glLoadIdentity();
  glTranslate(-1.5f, 0.0f, -6.0f);
  glBegin(GL_TRIANGLES);
  glVertex3f( 0.0f,  1.0f, 0.0f);
  glVertex3f(-1.0f, -1.0f, 0.0f);
  glVertex3f( 1.0f, -1.0f, 0.0f);
  glEnd();
}
