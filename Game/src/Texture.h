#pragma once
#include <GL/gl3w.h>
#include <FreeImage.h>
#include <string>

class Texture {
  FIBITMAP *img;
  GLuint id;

public:
  Texture(std::string path);
  ~Texture();
  void load() const;
  int getWidth() const;
  int getHeight() const;
};
