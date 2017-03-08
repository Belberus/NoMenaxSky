#pragma once
#include <GL/gl3w.h>
#include "Actor.h"
#include <glm/glm.hpp>
class Shaders;
class Window;
class Scene;
class WarriorGraphicsComponent : public GraphicsComponent {
  GLuint vao;
  GLuint ebo;
  GLuint buf;
  GLfloat quad[16] = {
    // Vertices	// Texture coordinates
    0.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f
  };
  GLuint indices[6] = {0, 1, 2, 0, 3, 2};

public:
  WarriorGraphicsComponent();
  void update(Actor &actor, Shaders &shaders);
  ~WarriorGraphicsComponent();
};

class WarriorInputComponent : public InputComponent {
public:
  void update(Window &window, Actor &actor);
};

class WarriorPhysicsComponent : public PhysicsComponent {
public:
  WarriorPhysicsComponent(glm::vec2 position, glm::vec2 size);
  void update(Actor &actor, Scene &scene);
};
