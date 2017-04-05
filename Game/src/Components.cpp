#include "Components.h"

Position::Position(glm::vec3 position) : position(position) {}

Physics::Physics(glm::vec2 velocity) : velocity(velocity) {}

Health::Health(int h) : health(h){};

Energy::Energy(int e) : energy(e){};

Body::Body(glm::vec2 position, glm::vec2 length)
    : position(position), length(length) {}

Door::Door(glm::vec2 position, glm::vec2 length , int numberOfRoom, Place place)
    : position(position), length(length), numberOfRoom(numberOfRoom), place(place){}

KnightAttack::KnightAttack(int damage, Orientation orientation)
    : isAttacking(false), damage(damage), orientation(orientation) {}

Graphics::Graphics(Texture texture, glm::vec2 size)
    : texture(texture), size(size) {
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &ebo);
  glGenBuffers(1, &buf);
}

Player::Player(Orientation orientation) : orientation(orientation) {}

GLfloat const Graphics::quad[16] = {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                                    1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                                    0.0f, 1.0f, 0.0f, 1.0f};

GLuint const Graphics::indices[6] = {0, 1, 2, 0, 3, 2};

AnimationClip::AnimationClip(std::vector<std::string> str, double timePerFrame)
    : timePerFrame(timePerFrame) {
  for (auto s : str) {
    clip.emplace_back(s);
  }
}

KnightAnimation::KnightAnimation(std::shared_ptr<AnimationClip> atk_n_top,
                                 std::shared_ptr<AnimationClip> atk_n_down,
                                 std::shared_ptr<AnimationClip> atk_n_left,
                                 std::shared_ptr<AnimationClip> atk_n_right,
                                 std::shared_ptr<AnimationClip> mov_top,
                                 std::shared_ptr<AnimationClip> mov_down,
                                 std::shared_ptr<AnimationClip> mov_left,
                                 std::shared_ptr<AnimationClip> mov_right)
    : atk_n_top(atk_n_top), atk_n_down(atk_n_down), atk_n_left(atk_n_left),
      atk_n_right(atk_n_right), mov_top(mov_top), mov_down(mov_down),
      mov_left(mov_left), mov_right(mov_right), which(nullptr), index(0),
      time(0) {}

GhostAnimation::GhostAnimation(std::shared_ptr<AnimationClip> mov_top,
                               std::shared_ptr<AnimationClip> mov_down,
                               std::shared_ptr<AnimationClip> mov_left,
                               std::shared_ptr<AnimationClip> mov_right)
    : mov_top(mov_top), mov_down(mov_down), mov_left(mov_left),
      mov_right(mov_right), which(nullptr), index(0), time(0) {}

MenuAnimation::MenuAnimation(std::shared_ptr<AnimationClip> menu_animation):
      menu_animation(menu_animation),which(nullptr), index(0), time(0) {}

ArrowMenu::ArrowMenu(Option option): option(option) {}

ArrowOptions::ArrowOptions(Option option): option(option) {}