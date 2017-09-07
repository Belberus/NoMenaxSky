#ifndef COMPONENTS_H_
#define COMPONENTS_H_

#include <functional>
#include <string>
#include <unordered_set>

#include <entityx/entityx.h>
#include <glm/glm.hpp>

struct Health {
  float init_hp;
  float hp;
  std::string death_fx;
  Health(float init_hp, float hp, std::string death_fx) : init_hp(init_hp), hp(hp), death_fx(death_fx) {}
};

struct MeleeWeapon {
  float damage;
  bool drawn;
  entityx::Entity owner;
  std::function<std::unordered_set<entityx::Entity>(entityx::EntityManager &)>
      enemies;
};

struct Energy {
  float init_nrg;
  float energy;
  Energy(float energy, float init_nrg) : energy(energy), init_nrg(init_nrg) {}
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
  enum Option { MUSIC_VOL, FX_VOL, SALIR, MODE };

  Option option;

  ArrowOptions(Option option) : option(option) {}
};

struct PauseOptions {
  enum Option { CONTINUAR, SALIR, MUSIC, FX};

  Option option;

  PauseOptions(Option option) : option(option) {}
};

struct GameOptions {
  enum Modo { TWO_D, THREE_D };
  enum Musica { MUSIC_ON, MUSIC_OFF };
  enum Efectos { FX_ON, FX_OFF };

  Modo modo;
  Musica musica;
  Efectos efectos;

  GameOptions(Modo modo, Musica musica, Efectos efectos) 
      : modo(modo), musica(musica), efectos(efectos) {}

};

struct WhatOption {
  int what;

  WhatOption(int what) : what(what) {}
};

struct D2Mode {};

struct D3Mode {};

struct MusicOn {};

struct MusicOff {};

struct FXOn {};

struct FXOff {};

struct Door {
  Door(const std::string &next_door, const std::string &pos)
      : next_door(next_door), pos(pos) {}
  std::string next_door;
  std::string pos;
};

struct BossDoor {
	BossDoor(const std::string &next_door, const std::string &pos)
      : next_door(next_door), pos(pos), level("0") {}
	std::string next_door;
	std::string pos;
  std::string level;
};

struct Shield {
	enum Orientation { UP, DOWN, RIGHT, LEFT };
	Orientation orientation;
	bool active;
	entityx::Entity owner;
	float time_passed;
	Shield(entityx::Entity owner) : owner(owner), orientation(Shield::Orientation::UP), active(false), time_passed(0.0f) {}
};

struct Player {
  enum Orientation { UP, DOWN, RIGHT, LEFT };
  enum State {NORMAL, BLOCKING};
  Orientation orientation;
  State state;
  bool key;
  int levers_activated;

  Player(Orientation orientation) : orientation(orientation), key(true), state(State::NORMAL), levers_activated(2) {}
  // PONER KEY A FALSE CUANDO LANCEMOS EL JUEGO Y LEVERS_ACTIVATED A 0
};

struct Characters{
  enum Role { KNIGHT, WIZARD };
  Role role;

  Characters(Role role) : role(role) {}
};

struct Cursor {};

struct Lancer {
  enum LancerOrientation { UP, DOWN, RIGHT, LEFT};
 
  bool is_attacking;
  float time_passed;

  LancerOrientation orientation;

  Lancer()
      : is_attacking(false), orientation(LancerOrientation::DOWN), time_passed(4000.0f) {}
};

struct LancerHitBox {
  LancerHitBox(float damage, entityx::Entity owner) : damage(damage), owner(owner) {}
  entityx::Entity owner;
  float damage;
};

struct Ghost {
	Ghost(): comportamiento(Comportamiento::FOLLOW), time_passed(0.0) {}
	enum Comportamiento { FOLLOW, DAMAGE_TOP, DAMAGE_BOTTOM, DAMAGE_LEFT, DAMAGE_RIGHT };
	Comportamiento comportamiento;
	static const float kHitDuration;
	float time_passed;
};

struct GhostHitBox{
	GhostHitBox(float damage, entityx::Entity owner) : damage(damage), owner(owner) {}
	entityx::Entity owner;
	float damage;
};

struct Turret{
	Turret(float frecuencia) : time_passed(0.0), frecuencia(frecuencia) {}
	float time_passed;
	float frecuencia;
};

struct RoomLimit {};

struct Sword {};

struct Legs{};

struct TurretLegs{};

struct LancerLegs{};

struct MasiatrixLegs{};

struct LowCollision {};

struct Manueleth {
	Manueleth() : comportamiento(Comportamiento::NORMAL), time_for_shooting(0.0), hits(0) {}
	enum Comportamiento {NORMAL, PUSH, SPECIAL};
	Comportamiento comportamiento;
	float time_for_shooting;
  int hits;
};

struct Masiatrix {
  enum Orientation {TOP, DOWN, LEFT, RIGHT};
  Masiatrix(std::string id, glm::vec3 original_position) : id(id), original_position(original_position), orientation(Orientation::DOWN), is_attacking(false), time_passed_attack(0.0f), time_passed_movement(3000.0f) {}
  Orientation orientation;
  glm::vec3 original_position;
  float time_passed_attack;
  float time_passed_movement;
  bool is_attacking;
  bool rand_initialized;
  std::string id;
};

struct Wizard {
	Wizard() : is_attacking(false) {};
	bool is_attacking;
};

struct Chest {
	Chest(bool key) : key(key) {}
	bool key;
};

struct Lever {
	Lever(std::string id) : id(id), activated(false) {}
	std::string id;
  bool activated;
};

struct Trap {
	enum Orientation { UP, DOWN, RIGHT, LEFT};
	Orientation orientation;
	float time_passed;
	float frecuencia;
	Trap(Orientation orientation, float frecuencia) : orientation(orientation), time_passed(0.0), frecuencia(frecuencia) {}
};

struct EnemyProjectile{
	EnemyProjectile(float damage) : damage(damage){}
	float damage;
};

struct WizardProjectile{
	WizardProjectile(float damage) : damage(damage) {};
	float damage;
};

struct ParentLink {
	entityx::Entity owner;
};

#endif  // COMPONENTS_H_