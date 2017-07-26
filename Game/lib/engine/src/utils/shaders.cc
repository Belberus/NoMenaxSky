#include "utils/shaders.h"

#include <iostream>
#include <vector>

GLuint engine::utils::CompileShader(const GLchar *src, GLenum type) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &src, NULL);
  glCompileShader(shader);
  GLint res;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &res);
  if (!res) {
    std::vector<char> compilation_log(512);
    glGetShaderInfoLog(shader, compilation_log.size(), NULL,
                       &compilation_log[0]);
    std::cerr << "Shader compilation error: " << &compilation_log[0] << '\n';
    exit(-1);
  }
  return shader;
}

GLuint engine::utils::LinkProgram(GLuint vertex, GLuint fragment) {
  GLuint program = glCreateProgram();
  glAttachShader(program, vertex);
  glAttachShader(program, fragment);
  glLinkProgram(program);
  GLint res;
  glGetProgramiv(program, GL_LINK_STATUS, &res);
  if (!res) {
    std::vector<char> compilation_log(512);
    glGetProgramInfoLog(program, compilation_log.size(), NULL,
                        &compilation_log[0]);
    std::cerr << "Program linking error: " << &compilation_log[0] << '\n';
    exit(-1);
  }
  return program;
}
