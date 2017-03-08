#include "ActorFactory.h"
#include "Actor.h"
#include "Texture.h"
#include "Warrior.h"
#include "Zombie.h"

ActorFactory::ActorFactory() {
  createWarrior();
  createZombie();
}

ActorFactory *ActorFactory::get() {
  static ActorFactory factory;
  return &factory;
}

Actor *ActorFactory::getActor(std::string const &name) {
  if (name.compare("warrior") == 0) {
    return actors[name];
  } else if (name.compare("zombie") == 0) {
    return actors[name];
  } else {
    return nullptr;
  }
}

Actor *ActorFactory::createWarrior() {
  Texture *texture = new Texture("assets/ppc_front.png");
  InputComponent *input = new WarriorInputComponent();
  GraphicsComponent *graphics = new WarriorGraphicsComponent();
  PhysicsComponent *physics =
      new WarriorPhysicsComponent(glm::vec2(0, 0), glm::vec2(70, 70));
  Actor *actor = new Actor(glm::vec3(480, 270, 1), glm::vec2(50, 50), texture,
                           graphics, input, physics);
  return actors.insert(std::pair<std::string, Actor *>("warrior", actor))
      .first->second;
}

Actor *ActorFactory::createZombie() {
  Texture *texture = new Texture("assets/sombra.png");
  InputComponent *input = new ZombieInputComponent();
  GraphicsComponent *graphics = new WarriorGraphicsComponent();
  PhysicsComponent *physics =
      new ZombiePhysicsComponent(glm::vec2(0, 0), glm::vec2(70, 70));
  Actor *actor = new Actor(glm::vec3(200, 200, 1), glm::vec2(100, 100), texture,
                           graphics, input, physics);
  return actors.insert(std::pair<std::string, Actor *>("zombie", actor))
      .first->second;
}

void ActorFactory::clear() {
  for (auto a : actors) {
    delete a.second;
  }
}
