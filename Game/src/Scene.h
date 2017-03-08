#pragma once
#include <vector>
class Actor;
class Window;
class Shaders;
class Scene {
  std::vector<Actor *> actors;

public:
  void addActor(Actor *actor);
  void checkCollision(Actor &actor);
  void update(Window &window, Shaders &shaders);
};
