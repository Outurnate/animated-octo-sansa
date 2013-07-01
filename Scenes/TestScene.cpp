#include "TestScene.h"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <tgmath.h>
#include <glm/gtc/noise.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <IL/il.h>

static inline void showShaderLog(GLuint object, PFNGLGETSHADERIVPROC glGet__iv, PFNGLGETSHADERINFOLOGPROC glGet__InfoLog)
{
    GLint length;
    char *log;

    glGet__iv(object, GL_INFO_LOG_LENGTH, &length);
    log = (char*)malloc(length);
    glGet__InfoLog(object, length, NULL, log);
    fprintf(stderr, "%s", log);
    free(log);
}

static inline GLuint makeShader(GLenum type, unsigned num, std::string fnames[])
{
  std::string contents;
  for (unsigned i = 0; i < num; ++i)
  {
    std::ifstream in(fnames[i], std::ios::in | std::ios::binary);
    if (in)
    {
      in.seekg(0, std::ios::end);
      size_t fsize = in.tellg();
      size_t osize = contents.size();
      contents.resize(osize + fsize);
      in.seekg(0, std::ios::beg);
      in.read(&contents[0] + osize, fsize);
      in.close();
    }
  }
  std::ofstream tempfile("temp.glsl");
  if (tempfile.is_open())
    tempfile << contents;
  tempfile.close();
  GLuint shader;
  GLint sourceLength = contents.length(), status;
  const char *source = contents.c_str();
  shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, &sourceLength);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (!status)
  {
    fprintf(stderr, "Failed to compile %s:\n", fnames[0].c_str());
    showShaderLog(shader, glGetShaderiv, glGetShaderInfoLog);
    glDeleteShader(shader);
    return 0;
  }
  return shader;
}

static inline GLuint makeProgram(GLuint vert, GLuint frag)
{
  GLint status;
  GLuint program = glCreateProgram();
  glAttachShader(program, vert);
  glAttachShader(program, frag);
  glLinkProgram(program);
  glGetProgramiv(program, GL_COMPILE_STATUS, &status);
  if (!status)
  {
    fprintf(stderr, "Failed to compile :\n");
    showShaderLog(program, glGetProgramiv, glGetProgramInfoLog);
    glDeleteProgram(program);
    return 0;
  }
  return program;
}

static inline GLuint loadTexture(const char* fname, bool alpha = false)
{
  GLuint tex;
  ILuint texil;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  ilGenImages(1, &texil);
  ilBindImage(texil);
  if (ilLoadImage((const ILstring)fname)
      && ilConvertImage(alpha ? IL_RGBA : IL_RGB, IL_UNSIGNED_BYTE))
    gluBuild2DMipmaps(GL_TEXTURE_2D, ilGetInteger(IL_IMAGE_BPP),
		      ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
		      ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
  else
    fprintf(stderr, "Failed to load texture: %s\n", fname);
  ilDeleteImages(1, &texil);

  return tex;
}

TestScene::TestScene()
  : Scene(), map_width(256), map_height(256), map(new float[map_width * map_height]), map_normal(new glm::vec3[map_width * map_height * 3]),
    current_pos({ 1.5f, 100.0f, 6.0f }),
    key_w(false), key_a(false), key_s(false), key_d(false), key_space(false), key_shift(false), wireframe(false), lighting(true),
    n_verticies_map(map_width * map_height * 10), n_indicies_map(map_width * map_height * 6), font_AverageMono("AverageMono.ttf")
{
  for(unsigned x = 0; x < map_width; ++x)
    for(unsigned y = 0; y < map_height; ++y)
      map[(y * map_width) + x] = (glm::simplex(glm::vec4(x / 64.0f, y / 64.0f, 0.5f, 0.5f)) * 64.0f) + 64.0f;
  unsigned i = 0;
  for(unsigned x = 0; x < map_width; ++x)
    for(unsigned y = 0; y < map_height; ++y)
      map_normal[i++] = glm::normalize(glm::cross(
      ((x == 0)                ? glm::vec3(x, map[(y * map_width) + x], y) : glm::vec3(x - 1, map[(y       * map_width) + x - 1], y)) -
      ((x == (map_width  - 1)) ? glm::vec3(x, map[(y * map_width) + x], y) : glm::vec3(x + 1, map[(y       * map_width) + x + 1], y)),
      ((y == 0)                ? glm::vec3(x, map[(y * map_width) + x], y) : glm::vec3(x,     map[((y - 1) * map_width) + x],    y - 1)) -
      ((y == (map_height - 1)) ? glm::vec3(x, map[(y * map_width) + x], y) : glm::vec3(x,     map[((y + 1) * map_width) + x],    y + 1))));
}

TestScene::~TestScene() { }

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
  glDeleteTextures(1, &tex_dirt_A_diffuse);
  glDeleteTextures(1, &tex_dirt_B_diffuse);
  glDeleteTextures(1, &tex_grass_A_diffuse);
  glDeleteTextures(1, &tex_grass_B_diffuse);
  glDeleteTextures(1, &tex_stone_A_diffuse);
  glDeleteTextures(1, &tex_stone_B_diffuse);
}

void TestScene::init(GLFWwindow* window)
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
  GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
  GLfloat position[] = { 100.0f, 100.0f, 100.0f, 0.0f }; glLightfv(GL_LIGHT0, GL_POSITION, position);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

  GLfloat verticies_map[n_verticies_map];
  GLushort indicies_map[n_indicies_map];

  std::string terrain_vert_arr[] = { "Media/Shaders/basic.vert" };
  std::string terrain_frag_arr[] = { "Media/Shaders/basic.frag", "Media/Shaders/noise2D.glsl" };
  terrain_vert = makeShader(GL_VERTEX_SHADER, 1, terrain_vert_arr);
  terrain_frag = makeShader(GL_FRAGMENT_SHADER, 2, terrain_frag_arr);
  terrain_prog = makeProgram(terrain_vert, terrain_frag);
  glUseProgram(terrain_prog);

  GLint lower_A_diffuse    = glGetUniformLocation(terrain_prog, "lower_A_diffuse");
  GLint lower_B_diffuse    = glGetUniformLocation(terrain_prog, "lower_B_diffuse");
  GLint middle_A_diffuse = glGetUniformLocation(terrain_prog, "middle_A_diffuse");
  GLint middle_B_diffuse = glGetUniformLocation(terrain_prog, "middle_B_diffuse");
  GLint upper_A_diffuse    = glGetUniformLocation(terrain_prog, "upper_A_diffuse");
  GLint upper_B_diffuse    = glGetUniformLocation(terrain_prog, "upper_B_diffuse");

  tex_dirt_A_diffuse  = loadTexture("Media/Textures/Dirt_A_Diffuse.tga");
  tex_dirt_B_diffuse  = loadTexture("Media/Textures/Dirt_B_Diffuse.tga");
  tex_grass_A_diffuse = loadTexture("Media/Textures/Grass_A_Diffuse.tga");
  tex_grass_B_diffuse = loadTexture("Media/Textures/Grass_B_Diffuse.tga");
  tex_stone_A_diffuse = loadTexture("Media/Textures/Stone_A_Diffuse.tga");
  tex_stone_B_diffuse = loadTexture("Media/Textures/Stone_B_Diffuse.tga");

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

  unsigned i = 0;
  for(unsigned x = 0; x < map_width; ++x)
    for(unsigned y = 0; y < map_height; ++y)
    {
      verticies_map[i++] = x; // X
      verticies_map[i++] = map[(y * map_width) + x]; // Y
      verticies_map[i++] = y; // Z
      verticies_map[i++] = map_normal[(y * map_width) + x].x; // NX
      verticies_map[i++] = map_normal[(y * map_width) + x].y; // NY
      verticies_map[i++] = map_normal[(y * map_width) + x].z; // NZ
      verticies_map[i++] = 0; // R
      verticies_map[i++] = 0; // G
      verticies_map[i++] = 0; // B
      verticies_map[i++] = 0; // A
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
  glVertexPointer(3, GL_FLOAT, 10 * sizeof(GLfloat), (GLvoid*)0);
  glNormalPointer(GL_FLOAT, 10 * sizeof(GLfloat), (GLvoid*)3);

  glGenBuffers(1, &map_ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, map_ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies_map), indicies_map, GL_STATIC_DRAW);
}

void TestScene::render(GLFWwindow* window, double delta, int width, int height)
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
  GLfloat fmodel[16] =
    {
      model[0][0],
      model[0][1],
      model[0][2],
      model[0][3],

      model[1][0],
      model[1][1],
      model[1][2],
      model[1][3],

      model[2][0],
      model[2][1],
      model[2][2],
      model[2][3],

      model[3][0],
      model[3][1],
      model[3][2],
      model[3][3]
    };
  /*for (unsigned i = 0; i < 16; ++i)
    printf("%d,", fmodel[i]);*/
  glMultMatrixf(fmodel);

  if (lighting)
  {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
  }

  glEnableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, map_vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, map_ibo);
  glDrawElements(GL_TRIANGLES, n_indicies_map, GL_UNSIGNED_SHORT, NULL);
  glDisableClientState(GL_VERTEX_ARRAY);

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
