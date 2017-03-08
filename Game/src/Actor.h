#pragma once
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class GraphicsComponent;
class InputComponent;
class PhysicsComponent;
class Texture;
class Shaders;
class Scene;
class Window;
class Actor {
  glm::vec3 position;
  glm::vec2 size;
  glm::vec2 velocity;
  Texture *texture;
  GraphicsComponent *graphicsComponent;
  InputComponent *inputComponent;
  PhysicsComponent *physicsComponent;

public:
  Actor(glm::vec3 position, glm::vec2 size, Texture *texture,
        GraphicsComponent *graphicsComponent, InputComponent *inputComponent,
        PhysicsComponent *physicsComponent);
  void update(Window &window, Scene &scene, Shaders &shaders);
  void setPosition(glm::vec3 pos);
  glm::vec3 const &getPosition() const;
  void setVelocity(glm::vec2 velocity);
  glm::vec2 const &getVelocity() const;
  Texture const *getTexture() const;
  glm::vec2 const &getSize() const;
  ~Actor();
};

class GraphicsComponent {
public:
  virtual void update(Actor &actor, Shaders &shaders) = 0;
};

class InputComponent {
public:
  virtual void update(Window &window, Actor &actor) = 0;
};

class PhysicsComponent {
protected:
  glm::vec2 position;
  glm::vec2 size;

public:
  PhysicsComponent(glm::vec2 position, glm::vec2 size)
      : position(position), size(size){};
  virtual void update(Actor &actor, Scene &scene) = 0;
};
