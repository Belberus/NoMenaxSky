#include "Actor.h"
#include "Scene.h"
#include "Window.h"
#include "Texture.h"

Actor::Actor(glm::vec3 position, glm::vec2 size, Texture *texture,
             GraphicsComponent *graphicsComponent,
             InputComponent *inputComponent, PhysicsComponent *physicsComponent)
    : position(position), size(size), texture(texture),
      graphicsComponent(graphicsComponent), inputComponent(inputComponent),
      physicsComponent(physicsComponent) {}

void Actor::update(Window &window, Scene &scene, Shaders &shaders) {
  inputComponent->update(window, *this);
  physicsComponent->update(*this, scene);
  graphicsComponent->update(*this, shaders);
}

void Actor::setPosition(glm::vec3 pos) { position = pos; }

glm::vec3 const &Actor::getPosition() const { return position; }

void Actor::setVelocity(glm::vec2 v) { velocity = v; }

glm::vec2 const &Actor::getVelocity() const { return velocity; }

Texture const *Actor::getTexture() const { return texture; }

glm::vec2 const &Actor::getSize() const { return size; }

Actor::~Actor() {
  delete graphicsComponent;
  delete inputComponent;
  delete physicsComponent;
  delete texture;
}
