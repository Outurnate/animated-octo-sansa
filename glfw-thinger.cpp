#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glfw.h>

static void error(int error, const char* description)
{
}

static void key(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(void)
{
  GLFWwindow* window;

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
  while(!glfwWindowShouldClose(window))
  {
    int width, height;
    double time = glfwGetTime(), delta = time - last_time;
    glfwGetFramebufferSize(window, &width, &height);
    //render
    glfwSwapBuffers(window);
    glfwPollEvents();
    last_time = time;
  }
  glfwDestroyWindow(window);
  glfwTerminate();
  return EXIT_SUCCESS;
}
