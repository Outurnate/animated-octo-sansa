#ifndef UTILH
#define UTILH

#define GLFW_INCLUDE_GLU
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <IL/il.h>

inline void showShaderLog(GLuint object, PFNGLGETSHADERIVPROC glGet__iv, PFNGLGETSHADERINFOLOGPROC glGet__InfoLog)
{
    GLint length;
    char *log;

    glGet__iv(object, GL_INFO_LOG_LENGTH, &length);
    log = (char*)malloc(length);
    glGet__InfoLog(object, length, NULL, log);
    fprintf(stderr, "%s", log);
    free(log);
}

inline GLuint makeShader(GLenum type, unsigned num, std::string fnames[])
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

inline GLuint makeProgram(GLuint vert, GLuint frag)
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

inline GLuint loadTexture(const char* fname, bool alpha = false)
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

#endif
