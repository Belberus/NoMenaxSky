#include "Actor.h"
#include <glm/glm.hpp>
class Window;
class Actor;
class Scene;

class ZombieInputComponent : public InputComponent {
public:
  void update(Window &window, Actor &actor) { ; }
};

class ZombiePhysicsComponent : public PhysicsComponent {
public:
  ZombiePhysicsComponent(glm::vec2 position, glm::vec2 size)
      : PhysicsComponent(position, size) {}
  void update(Actor &actor, Scene &scene) { ; }
};
