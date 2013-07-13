#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define GLFW_INCLUDE_GLU
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <IL/il.h>

#include "Scene.h"
#include "TerrainScene.h"

static Scene* scene;

static void error(int error, const char* description)
{
  std::cout << description << std::endl;
}

static void key(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  scene->key(window, key, scancode, action, mods);
}

static void mouse(GLFWwindow* window, double x, double y)
{
  scene->mouse(window, x, y);
}

int main(void)
{
  GLFWwindow* window;
  scene = new TerrainScene();

  glfwSetErrorCallback(error);
  if (!glfwInit())
    return EXIT_FAILURE;
  //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  if (!(window = glfwCreateWindow(640, 480, "asdf", NULL, NULL)))
  {
    glfwTerminate();
    return EXIT_FAILURE;
  }
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key);
  glfwSetCursorPosCallback(window, mouse);

  double last_time = 0.0;

  glewExperimental = GL_TRUE;
  glewInit();
  if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
  {
    glfwTerminate();
    return EXIT_FAILURE;
  }
  ilInit();
  scene->init(window);
  while(!glfwWindowShouldClose(window))
  {
    int width, height;
    double time = glfwGetTime(), delta = time - last_time;
    glfwGetFramebufferSize(window, &width, &height);
    scene->render(window, delta, width, height);
    glfwSwapBuffers(window);
    glfwPollEvents();
    last_time = time;
  }
  scene->destroy(window);
  glfwDestroyWindow(window);
  glfwTerminate();
  return EXIT_SUCCESS;
}
