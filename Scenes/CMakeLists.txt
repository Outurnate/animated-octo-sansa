ADD_DEFINITIONS(-std=c++0x)
add_library(Scenes TestScene.cpp Scene2D.cpp)
target_link_libraries(Scenes glfw ${FTGL_LIBRARIES} ${FREETYPE_LIBRARIES} ${GLFW_LIBRARIES} ${OPENGL_glu_LIBRARY} ${OPENGL_LIBRARIES} ${ASSIMP_LIBRARIES})
