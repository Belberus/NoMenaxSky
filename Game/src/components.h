#ifndef COMPONENTS_H_
#define COMPONENTS_H_

#include <functional>
#include <string>
#include <unordered_set>

#include <entityx/entityx.h>
#include <glm/glm.hpp>

struct Health {
  float hp;
  Health(float hp) : hp(hp) {}
};

struct MeleeWeapon {
  float damage;
  bool drawn;
  entityx::Entity owner;
  std::function<std::unordered_set<entityx::Entity>(entityx::EntityManager &)>
      enemies;
};

struct Energy {
  int energy;
  Energy(int e) : energy(e) {}
};

struct KnightAttack {
  enum Orientation { UP, DOWN, RIGHT, LEFT };

  bool is_attacking;
  int damage;
  Orientation orientation;

  KnightAttack(int damage, Orientation orientation)
      : is_attacking(false), damage(damage), orientation(orientation) {}
};

struct MenuImage {};

struct ArrowMenu {
  enum Option { JUGAR, OPCIONES, SALIR };

  Option option;

  ArrowMenu(Option option) : option(option) {}
};

struct ArrowOptions {
  enum Option { MUSIC_VOL, FX_VOL, SALIR };

  Option option;

  ArrowOptions(Option option) : option(option) {}
};

struct Door {
  Door(const std::string &next_door, const std::string &pos)
      : next_door(next_door), pos(pos) {}
  std::string next_door;
  std::string pos;
};

struct Player {
  enum Orientation { UP, DOWN, RIGHT, LEFT };
  Orientation orientation;

  Player(Orientation orientation) : orientation(orientation) {}
};

struct Ghost {
	Ghost(): comportamiento(Comportamiento::FOLLOW), time_passed(0.0) {}
	enum Comportamiento { FOLLOW, DAMAGE_TOP, DAMAGE_BOTTOM, DAMAGE_LEFT, DAMAGE_RIGHT };
	Comportamiento comportamiento;
	static const float kHitDuration;
	float time_passed;
};

struct Turret{};

struct RoomLimit {};

struct Sword {};

struct Legs{};

struct ParentLink {
	entityx::Entity owner;
};

#endif  // COMPONENTS_H_