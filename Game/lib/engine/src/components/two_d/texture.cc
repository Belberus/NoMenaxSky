#include "engine/components/two_d/texture.h"

#include <iostream>

#include <GL/gl3w.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// TODO: if the number of texture exceeds 16 some problem may arise
engine::components::two_d::Texture::Texture(const std::string &path)
    : width_(0),
      height_(0),
      ch_(0),
      img_data_(nullptr, [](unsigned char *) {}),
      texture_id_(0),
      texture_unit_id_(0) {
  stbi_set_flip_vertically_on_load(1);
  auto deleter = [](unsigned char *img_ptr) -> void {
    stbi_image_free(img_ptr);
  };
  img_data_ = std::unique_ptr<unsigned char, void (*)(unsigned char *)>(
      stbi_load(path.c_str(), &width_, &height_, &ch_, STBI_rgb_alpha),
      deleter);
  if (!img_data_) {
    std::cerr << "Failed to load image: " << path.c_str()
              << ", because: " << stbi_failure_reason() << '\n';
    exit(-1);
  }
  texture_unit_id_ = GL_TEXTURE0 + next_texture_unit_;
  next_texture_unit_ =
      (next_texture_unit_ + 1) % GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS;
  glActiveTexture(texture_unit_id_);
  glGenTextures(1, &texture_id_);
  glBindTexture(GL_TEXTURE_2D, texture_id_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, (GLvoid *)img_data_.get());
}

int engine::components::two_d::Texture::GetWidth() const { return width_; }

int engine::components::two_d::Texture::GetHeight() const { return height_; }

engine::components::two_d::Texture::~Texture() {
  glDeleteTextures(1, &texture_id_);
}

int engine::components::two_d::Texture::next_texture_unit_ = 0;
