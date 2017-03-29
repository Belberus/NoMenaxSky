#include "Shaders.h"
#include <iostream>

Shaders::Shaders() {
  static const GLchar *vertexShaderSource[] = {
      "#version 330 core\n"
      "layout (location = 0) in vec4 position;\n"
      "out vec2 TexCoord;\n"
      "uniform mat4 model;\n"
      "uniform mat4 projection;\n"
      "void main(void)\n"
      "{\n"
      "gl_Position = projection * model * vec4(position.xy, 0.0, 1.0);\n"
      "TexCoord = position.zw;\n"
      "}\n"};

  static const GLchar *fragmentShaderSource[] = {
      "#version 330 core\n"
      "out vec4 color;\n"
      "in vec2 TexCoord;\n"
      "uniform sampler2D ourTexture;\n"
      "void main(void)\n"
      "{\n"
      " color = texture(ourTexture, TexCoord);\n"
      "}\n"};

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Shaders::useProgram() { glUseProgram(program); }

GLuint Shaders::getProgram() { return program; }

Shaders::~Shaders() {}
