#include "Texture.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(std::string path) {
  glGenTextures(1, &id);
  stbi_set_flip_vertically_on_load(1);
  img = stbi_load(path.c_str(), &width, &height, &ch, STBI_rgb_alpha);
  if (!img) {
    std::cout << "Failed to load image: " << path.c_str() << ", because: " 
       << stbi_failure_reason() << std::endl;
  }
}

Texture::Texture(const Texture &tex) {
  id = tex.id;
  ch = tex.ch;
  width = tex.width;
  height = tex.height;
  img = new unsigned char[tex.ch * tex.width * tex.height];
  std::copy(tex.img, tex.img + tex.ch * tex.width * tex.height, img);
}

Texture &Texture::operator=(const Texture &tex) {
  id = tex.id;
  ch = tex.ch;
  width = tex.width;
  height = tex.height;
  img = new unsigned char[tex.ch * tex.width * tex.height];
  std::copy(tex.img, tex.img + tex.ch * tex.width * tex.height, img);
  return *this;
}

void Texture::load() {
  glBindTexture(GL_TEXTURE_2D, id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, (GLvoid *)img);
}
int Texture::getWidth() const { return width; }

int Texture::getHeight() const { return height; }

Texture::~Texture() {
  stbi_image_free(img);
  glDeleteTextures(1, &id);
}
