#pragma once
#include <GL/gl3w.h>
#include <memory>
#include <string>

class Texture {
  int width;
  int height;
  int ch;
  unsigned char *img;
  GLuint id;

public:
  Texture(std::string path);
  Texture(const Texture &tex);
  Texture &operator=(const Texture &tex);
  ~Texture();
  void load();
  int getWidth() const;
  int getHeight() const;
};
