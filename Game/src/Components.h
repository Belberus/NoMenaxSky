#pragma once
#include <GL/gl3w.h>
#include <entityx/entityx.h>
#include <glm/glm.hpp>
#include <vector>
#include "Texture.h"

struct Position {
  glm::vec3 position;

  Position(glm::vec3 position) : position(position) {}
};

struct Physics {
  glm::vec2 velocity;

  Physics(glm::vec2 velocity) : velocity(velocity) {}
};

struct Body {
  glm::vec2 position;
  glm::vec2 length;

  Body(glm::vec2 position, glm::vec2 length)
      : position(position), length(length) {}
};

struct KnightAttack {
  enum Orientation { UP, DOWN, RIGHT, LEFT };

  bool isAttacking;
  int damage;
  Orientation orientation;

  KnightAttack(int damage, Orientation orientation)
      : isAttacking(false), damage(damage), orientation(orientation) {}
};

struct Player {};

struct Graphics {
  Texture texture;
  GLuint vao;
  GLuint ebo;
  GLuint buf;
  GLfloat quad[16] = {  // Vertices	// Texture coordinates
      0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
      1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f};
  GLuint indices[6] = {0, 1, 2, 0, 3, 2};
  glm::vec2 size;

  Graphics(Texture texture, GLuint vao, GLuint ebo, GLuint buf, glm::vec2 size)
      : texture(texture), vao(vao), ebo(ebo), buf(buf), size(size) {}
};

struct AnimationClip {
  std::vector<Texture> clip;
  entityx::TimeDelta timePerFrame;

  AnimationClip(std::vector<std::string> str, double timePerFrame)
      : timePerFrame(timePerFrame) {
    for (auto s : str) {
      clip.emplace_back(s);
    }
  }
};

struct KnightAnimation {
  std::shared_ptr<AnimationClip> atk_n_top;
  std::shared_ptr<AnimationClip> atk_n_down;
  std::shared_ptr<AnimationClip> atk_n_left;
  std::shared_ptr<AnimationClip> atk_n_right;
  std::shared_ptr<AnimationClip> mov_top;
  std::shared_ptr<AnimationClip> mov_down;
  std::shared_ptr<AnimationClip> mov_left;
  std::shared_ptr<AnimationClip> mov_right;
  std::shared_ptr<AnimationClip> which;
  std::vector<std::shared_ptr<Texture>>::size_type index;
  entityx::TimeDelta time;

  KnightAnimation(std::shared_ptr<AnimationClip> atk_n_top,
                  std::shared_ptr<AnimationClip> atk_n_down,
                  std::shared_ptr<AnimationClip> atk_n_left,
                  std::shared_ptr<AnimationClip> atk_n_right,
                  std::shared_ptr<AnimationClip> mov_top,
                  std::shared_ptr<AnimationClip> mov_down,
                  std::shared_ptr<AnimationClip> mov_left,
                  std::shared_ptr<AnimationClip> mov_right)
      : atk_n_top(atk_n_top),
        atk_n_down(atk_n_down),
        atk_n_left(atk_n_left),
        atk_n_right(atk_n_right),
        mov_top(mov_top),
        mov_down(mov_down),
        mov_left(mov_left),
        mov_right(mov_right),
        which(nullptr),
        index(0),
        time(0) {}
};
