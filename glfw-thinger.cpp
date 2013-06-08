#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define GLFW_INCLUDE_GLU
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Scene.h"
#include "TestScene.h"

static Scene* scene;

static void error(int error, const char* description)
{
  std::cout << description << std::endl;
}

static void key(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  scene->key(key, scancode, action, mods);
}

int main(void)
{
  GLFWwindow* window;
  scene = new TestScene();

  glfwSetErrorCallback(error);
  if (!glfwInit())
    return EXIT_FAILURE;
  if (!(window = glfwCreateWindow(640, 480, "asdf", NULL, NULL)))
  {
    glfwTerminate();
    return EXIT_FAILURE;
  }
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key);
  double last_time = 0.0;

  glewExperimental = GL_TRUE;
  glewInit();
  scene->init();
  while(!glfwWindowShouldClose(window))
  {
    int width, height;
    double time = glfwGetTime(), delta = time - last_time;
    glfwGetFramebufferSize(window, &width, &height);
    scene->render(delta, width, height);
    glfwSwapBuffers(window);
    glfwPollEvents();
    last_time = time;
  }
  scene->destroy();
  glfwDestroyWindow(window);
  glfwTerminate();
  return EXIT_SUCCESS;
}
