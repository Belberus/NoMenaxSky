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

struct Health {
  int health;
  Health(int h):health(h){};
};

struct Energy {
  int energy;
  Energy(int e):energy(e){};
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

struct Ghost {};

struct Graphics {
  Texture texture;
  GLuint vao;
  GLuint ebo;
  GLuint buf;
  static GLfloat const quad[16];
  static GLuint const indices[6];
  glm::vec2 size;

  Graphics(Texture texture, glm::vec2 size) : texture(texture), size(size) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &ebo);
    glGenBuffers(1, &buf);
  }
};

GLfloat const Graphics::quad[16] = {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                                    1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                                    0.0f, 1.0f, 0.0f, 1.0f};

GLuint const Graphics::indices[6] = {0, 1, 2, 0, 3, 2};

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

struct GhostAnimation {
  std::shared_ptr<AnimationClip> mov_top;
  std::shared_ptr<AnimationClip> mov_down;
  std::shared_ptr<AnimationClip> mov_left;
  std::shared_ptr<AnimationClip> mov_right;
  std::shared_ptr<AnimationClip> which;
  std::vector<std::shared_ptr<Texture>>::size_type index;
  entityx::TimeDelta time;

  GhostAnimation( std::shared_ptr<AnimationClip> mov_top,
                  std::shared_ptr<AnimationClip> mov_down,
                  std::shared_ptr<AnimationClip> mov_left,
                  std::shared_ptr<AnimationClip> mov_right)
      : mov_top(mov_top),
        mov_down(mov_down),
        mov_left(mov_left),
        mov_right(mov_right),
        which(nullptr),
        index(0),
        time(0) {}
};
