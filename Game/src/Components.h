#pragma once
#include <GL/gl3w.h>
#include "Texture.h"
#include <entityx/entityx.h>
#include <glm/glm.hpp>
#include <vector>

struct Position {
  glm::vec3 position;

  Position(glm::vec3 position);
};

struct Physics {
  glm::vec2 velocity;

  Physics(glm::vec2 velocity);
};

struct Health {
  int health;
  Health(int h);
};

struct Energy {
  int energy;
  Energy(int e);
};

struct Body {
  glm::vec2 position;
  glm::vec2 length;

  Body(glm::vec2 position, glm::vec2 length);
};

struct KnightAttack {
  enum Orientation { UP, DOWN, RIGHT, LEFT };

  bool isAttacking;
  int damage;
  Orientation orientation;

  KnightAttack(int damage, Orientation orientation);
};

struct MenuImage {};

struct ArrowMenu {
  enum Option { JUGAR, OPCIONES, SALIR };

  Option option;

  ArrowMenu(Option option);
};

struct Door {
	glm::vec2 position;
  	glm::vec2 length;
  	int numberOfRoom;

  	Door(glm::vec2 position,glm::vec2 length, int numberOfRoom);
};

struct Player {};

struct Ghost {};

struct RoomLimit {};

struct Graphics {
  Texture texture;
  GLuint vao;
  GLuint ebo;
  GLuint buf;
  static GLfloat const quad[16];
  static GLuint const indices[6];
  glm::vec2 size;

  Graphics(Texture texture, glm::vec2 size);
};

struct AnimationClip {
  std::vector<Texture> clip;
  entityx::TimeDelta timePerFrame;

  AnimationClip(std::vector<std::string> str, double timePerFrame);
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
                  std::shared_ptr<AnimationClip> mov_right);
};

struct GhostAnimation {
  std::shared_ptr<AnimationClip> mov_top;
  std::shared_ptr<AnimationClip> mov_down;
  std::shared_ptr<AnimationClip> mov_left;
  std::shared_ptr<AnimationClip> mov_right;
  std::shared_ptr<AnimationClip> which;
  std::vector<std::shared_ptr<Texture>>::size_type index;
  entityx::TimeDelta time;

  GhostAnimation(std::shared_ptr<AnimationClip> mov_top,
                 std::shared_ptr<AnimationClip> mov_down,
                 std::shared_ptr<AnimationClip> mov_left,
                 std::shared_ptr<AnimationClip> mov_right);
};

struct MenuAnimation {
  std::shared_ptr<AnimationClip> menu_animation;
  std::shared_ptr<AnimationClip> which;
  std::vector<std::shared_ptr<Texture>>::size_type index;
  entityx::TimeDelta time;

  MenuAnimation(std::shared_ptr<AnimationClip> menu_animation);
};