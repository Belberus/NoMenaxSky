#pragma once
#include <map>
class Scene;
class SceneFactory {
  std::map<std::string, Scene *> scenes;

public:
  SceneFactory();
  static SceneFactory *get();
  Scene *getScene(std::string const &scene);
  void clear();

private:
  Scene *createScene1();
};
