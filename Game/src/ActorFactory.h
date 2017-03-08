#pragma once
#include <map>
class Actor;
class ActorFactory {
  std::map<std::string, Actor *> actors;

public:
  ActorFactory();
  static ActorFactory *get();
  Actor *getActor(std::string const &actor);
  void clear();

private:
  Actor *createWarrior();
  Actor *createZombie();
};
