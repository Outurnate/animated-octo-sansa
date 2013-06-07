cmake_minimum_required (VERSION 2.6)
project (glfw-thinger)
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
target_link_libraries(glfw-thinger Scenes glfw ${GLFW_LIBRARIES} ${OPENGL_glu_LIBRARY})
