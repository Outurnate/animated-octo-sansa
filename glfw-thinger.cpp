#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glfw.h>

int main(void)
{
  if (!glfwInit())
    return EXIT_FAILURE;
  if (!glfwOpenWindow(640, 480, 8, 8, 8, 0, 24, 0, GLFW_WINDOW))
    return EXIT_FAILURE;
  while (glfwGetWindowParam(GLFW_OPENED))
  {
    glfwSwapBuffers();
  }
  glfwTerminate();
  return EXIT_SUCCESS;
}
