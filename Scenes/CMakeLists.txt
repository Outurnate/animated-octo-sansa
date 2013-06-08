add_library(Scenes TestScene.cpp)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${OPENGL_glu_INCLUDE_DIRS})
target_link_libraries(Scenes glfw ${GLFW_LIBRARIES} ${OPENGL_glu_LIBRARY} ${OPENGL_LIBRARIES})
