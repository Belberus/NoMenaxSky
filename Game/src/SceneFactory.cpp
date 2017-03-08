#include "SceneFactory.h"
#include "ActorFactory.h"
#include "Scene.h"
SceneFactory::SceneFactory() { createScene1(); }

SceneFactory *SceneFactory::get() {
  static SceneFactory factory;
  return &factory;
}

Scene *SceneFactory::getScene(std::string const &scene) {
  if (scene.compare("scene1") == 0) {
    return scenes[scene];
  } else {
    return nullptr;
  }
}

Scene *SceneFactory::createScene1() {
  Scene *scene = new Scene();
  scene->addActor(ActorFactory::get()->getActor("warrior"));
  scene->addActor(ActorFactory::get()->getActor("zombie"));
  return scenes.insert(std::pair<std::string, Scene *>("scene1", scene))
      .first->second;
}

void SceneFactory::clear() {
  ActorFactory::get()->clear();
  for (auto s : scenes) {
    delete s.second;
  }
}
