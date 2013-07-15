#include "TerrainScene.hpp"

#include "Util.hpp"

#include <tgmath.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

TerrainScene::TerrainScene(GLFWwindow *window)
  : Scene(), current_pos({ 0.0f, 50.0f, 0.0f }),
    key_w(false), key_a(false), key_s(false), key_d(false), key_space(false), key_shift(false), wireframe(false), lighting(true),
    font_AverageMono("media/fonts/AverageMono.ttf"), terrain(3, 8), window(window)
{
}

TerrainScene::~TerrainScene() { }

void TerrainScene::key(int key, int scancode, int action, int mods)
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

void TerrainScene::mouse(double x, double y)
{
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  dx += ((width / 2) - x) / 1000.0f;
  dy += ((height / 2) - y) / 1000.0f;
  glfwSetCursorPos(window, width / 2, height / 2);
}

void TerrainScene::destroy()
{
  glDeleteTextures(1, &tex_dirt_A_diffuse);
  glDeleteTextures(1, &tex_dirt_B_diffuse);
  glDeleteTextures(1, &tex_grass_A_diffuse);
  glDeleteTextures(1, &tex_grass_B_diffuse);
  glDeleteTextures(1, &tex_stone_A_diffuse);
  glDeleteTextures(1, &tex_stone_B_diffuse);
  terrain.Unload();
}

void TerrainScene::init()
{
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  glDepthMask(GL_TRUE);

  glShadeModel(GL_FLAT);
  glEnable(GL_NORMALIZE);

  GLfloat ambient[]  = { 1.0f, 1.0f, 1.0f, 1.0f }; glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
  GLfloat diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f }; glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse);
  GLfloat specular[] = { 0.0f, 0.0f, 0.0f, 1.0f }; glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

  std::string terrain_vert_arr[] = { "media/shaders/terrain.vert.glsl" };
  std::string terrain_frag_arr[] = { "media/shaders/terrain.frag.glsl", "media/shaders/noise2D.glsl" };
  terrain_vert = makeShader(GL_VERTEX_SHADER, 1, terrain_vert_arr);
  terrain_frag = makeShader(GL_FRAGMENT_SHADER, 2, terrain_frag_arr);
  terrain_prog = makeProgram(terrain_vert, terrain_frag);
  glUseProgram(terrain_prog);

  GLint lower_A_diffuse  = glGetUniformLocation(terrain_prog,  "lower_A_diffuse");
  GLint lower_B_diffuse  = glGetUniformLocation(terrain_prog,  "lower_B_diffuse");
  GLint middle_A_diffuse = glGetUniformLocation(terrain_prog, "middle_A_diffuse");
  GLint middle_B_diffuse = glGetUniformLocation(terrain_prog, "middle_B_diffuse");
  GLint upper_A_diffuse  = glGetUniformLocation(terrain_prog,  "upper_A_diffuse");
  GLint upper_B_diffuse  = glGetUniformLocation(terrain_prog,  "upper_B_diffuse");

  tex_dirt_A_diffuse  = loadTexture("media/textures/Dirt_A_Diffuse.tga");
  tex_dirt_B_diffuse  = loadTexture("media/textures/Dirt_B_Diffuse.tga");
  tex_grass_A_diffuse = loadTexture("media/textures/Grass_A_Diffuse.tga");
  tex_grass_B_diffuse = loadTexture("media/textures/Grass_B_Diffuse.tga");
  tex_stone_A_diffuse = loadTexture("media/textures/Stone_A_Diffuse.tga");
  tex_stone_B_diffuse = loadTexture("media/textures/Stone_B_Diffuse.tga");

  glUniform1i(lower_A_diffuse, 0);
  glActiveTexture(GL_TEXTURE0 + 0);
  glBindTexture(GL_TEXTURE_2D, tex_dirt_A_diffuse);

  glUniform1i(lower_B_diffuse, 1);
  glActiveTexture(GL_TEXTURE0 + 1);
  glBindTexture(GL_TEXTURE_2D, tex_dirt_B_diffuse);

  glUniform1i(middle_A_diffuse, 2);
  glActiveTexture(GL_TEXTURE0 + 2);
  glBindTexture(GL_TEXTURE_2D, tex_grass_A_diffuse);

  glUniform1i(middle_B_diffuse, 3);
  glActiveTexture(GL_TEXTURE0 + 3);
  glBindTexture(GL_TEXTURE_2D, tex_grass_B_diffuse);

  glUniform1i(upper_A_diffuse, 4);
  glActiveTexture(GL_TEXTURE0 + 4);
  glBindTexture(GL_TEXTURE_2D, tex_stone_A_diffuse);

  glUniform1i(upper_B_diffuse, 5);
  glActiveTexture(GL_TEXTURE0 + 5);
  glBindTexture(GL_TEXTURE_2D, tex_stone_B_diffuse);

  terrain.Load();
}

void TerrainScene::render(double delta, int width, int height)
{
  if (key_w)
  {
    current_pos.x += sin(dx) * delta * 10.0f;
    current_pos.z += cos(dx) * delta * 10.0f;
  }
  if (key_s)
  {
    current_pos.x -= sin(dx) * delta * 10.0f;
    current_pos.z -= cos(dx) * delta * 10.0f;
  }
  if (key_a)
  {
    current_pos.x += sin(dx + (M_PI / 2.0f)) * delta * 10.0f;
    current_pos.z += cos(dx + (M_PI / 2.0f)) * delta * 10.0f;
  }
  if (key_d)
  {
    current_pos.x -= sin(dx + (M_PI / 2.0f)) * delta * 10.0f;
    current_pos.z -= cos(dx + (M_PI / 2.0f)) * delta * 10.0f;
  }
  if (key_space) current_pos.y += delta * 20.0f;
  if (key_shift) current_pos.y -= delta * 20.0f;

  GLint camera_pos = glGetUniformLocation(terrain_prog, "camera_pos");
  glUniform3f(camera_pos, current_pos.x, current_pos.y, current_pos.z);

  float ar = (float)width / (float)height;

  glUseProgram(terrain_prog);

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
  glColor3d(1.0, 0.0, 0.0);

  glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);

  // TODO: use glm
  point3f dir({ (float)(cos(dy) * sin(dx)), (float)sin(dy), (float)(cos(dy) * cos(dx)) });
  point3f right({ (float)sin(dx - (M_PI / 2.0f)), 0.0f, (float)cos(dx - (M_PI / 2.0f)) });
  point3f up({ (float)((right.y * dir.z) - (right.z * dir.y)), (float)((right.z * dir.x) - (right.x * dir.z)), (float)((right.x * dir.y) - (right.y * dir.x)) });

  glm::mat4 model =
    glm::lookAt(glm::vec3(current_pos.x,         current_pos.y,         current_pos.z),
		glm::vec3(current_pos.x + dir.x, current_pos.y + dir.y, current_pos.z + dir.z),
		glm::vec3(up.x,                  up.y,                  up.z));
  glLightfv(GL_LIGHT0, GL_POSITION, glm::value_ptr(model * glm::vec4(-1, -1, 1, 0)));
  glLoadMatrixf(glm::value_ptr(model));

  if (lighting)
  {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
  }

  terrain.Render();

  glUseProgram(0);
  glDisable(GL_LIGHTING);

  glPixelTransferf(GL_RED_BIAS,  -1.0f);
  glPixelTransferf(GL_GREEN_BIAS, 0.0f);
  glPixelTransferf(GL_BLUE_BIAS, -1.0f);

  font_AverageMono.FaceSize(16);
  font_AverageMono.Render("Test Scene", -1, FTPoint(0.0f, 48.0f));
  font_AverageMono.Render(static_cast<std::ostringstream*>(&(std::ostringstream() << "Frame Time (ms): " << delta * 1000))->str().c_str(), -1, FTPoint(0.0f, 32.0f));
  font_AverageMono.Render(static_cast<std::ostringstream*>(&(std::ostringstream() << "Wireframe      : " << wireframe))->str().c_str(), -1, FTPoint(0.0f, 16.0f));
  font_AverageMono.Render(static_cast<std::ostringstream*>(&(std::ostringstream() << width << "x" << height))->str().c_str(), -1, FTPoint(0.0f, 0.0f));
}
