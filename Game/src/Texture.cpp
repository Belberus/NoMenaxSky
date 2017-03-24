#include "Texture.h"
#include <FreeImage.h>
#include <iostream>

Texture::Texture(std::string path) {
  img = FreeImage_Load(FreeImage_GetFileType(path.c_str()), path.c_str());
  int bitsPerPixel = FreeImage_GetBPP(img);
  FIBITMAP *img32;
  if (bitsPerPixel == 32) {
    img32 = img;
  } else {
    img32 = FreeImage_ConvertTo32Bits(img);
  }

  if (bitsPerPixel != 32) {
    FreeImage_Unload(img);
    img = img32;
  }
  glGenTextures(1, &id);
}

Texture::Texture(const Texture &tex) {
  id = tex.id;
  img = FreeImage_Clone(tex.img);
}

Texture &Texture::operator=(const Texture &rhs) {
  id = rhs.id;
  img = FreeImage_Clone(rhs.img);
  return *this;
}

void Texture::load() {
  glBindTexture(GL_TEXTURE_2D, id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, getWidth(), getHeight(), 0, GL_BGRA,
               GL_UNSIGNED_BYTE, (GLvoid *)FreeImage_GetBits(img));
}
int Texture::getWidth() const { return FreeImage_GetWidth(img); }

int Texture::getHeight() const { return FreeImage_GetHeight(img); }

Texture::~Texture() {
  glDeleteTextures(1, &id);
  FreeImage_Unload(img);
}
