cmake_minimum_required (VERSION 2.6)
project (glfw-thinger)
include (FindGLEW.cmake)
set (glfw-thinger_VERSION_MAJOR 1)
set (glfw-thinger_VERSION_MINOR 0)
configure_file (
  "${PROJECT_SOURCE_DIR}/Config.h.in"
  "${PROJECT_BINARY_DIR}/Config.h"
)
include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/Scenes")
add_subdirectory(Scenes)
add_executable(glfw-thinger glfw-thinger.cpp)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${OPENGL_glu_INCLUDE_DIRS} ${GLEW_INCLUDE_PATH})
target_link_libraries(glfw-thinger Scenes glfw ${GLFW_LIBRARIES} ${OPENGL_glu_LIBRARY} ${OPENGL_LIBRARIES} ${GLEW_LIBRARY})
