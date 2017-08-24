#include "entity_factory.h"

#include <string>

#include <engine/components/common/camera.h>
#include <engine/components/common/physics.h>
#include <engine/components/common/transform.h>
#include <engine/components/two_d/aabb_collider.h>
#include <engine/components/two_d/color_animation.h>
#include <engine/components/two_d/sprite_animation.h>
#include <engine/core/engine.h>
#include <engine/core/resource_manager.h>

#include "components.h"

using namespace engine::components::common;
using namespace engine::components::two_d;
using namespace engine::core;
using namespace engine::utils;

std::vector<entityx::Entity> EntityFactory::MakeKnight(
    entityx::EntityManager &entities, const glm::vec3 &position) {
  std::vector<entityx::Entity> entities_created;
  auto player = entities.create();
  player.assign<Physics>(glm::vec3(0, 0, 0));
  player.assign<Transform>(position);
  player.assign<Player>(Player::Orientation::DOWN);
  player.assign<AABBCollider>(glm::vec2(0, 0), glm::vec2(8, 8));
  player.assign<KnightAttack>(100, KnightAttack::Orientation::UP);
  player.assign<Health>(100.0f);

  std::vector<engine::utils::Rectangle> moving_bottom;
  moving_bottom.emplace_back(glm::vec2(3, 137), glm::vec2(15, 14));
  std::vector<engine::utils::Rectangle> moving_top;
  moving_top.emplace_back(glm::vec2(22, 137), glm::vec2(15, 14));
  std::vector<engine::utils::Rectangle> moving_right;
  moving_right.emplace_back(glm::vec2(41, 137), glm::vec2(15, 14));
  std::vector<engine::utils::Rectangle> moving_left;
  moving_left.emplace_back(glm::vec2(60, 137), glm::vec2(15, 14));

  std::vector<engine::utils::Rectangle> defend_bottom;
  defend_bottom.emplace_back(glm::vec2(3, 118), glm::vec2(15, 14));
  std::vector<engine::utils::Rectangle> defend_top;
  defend_top.emplace_back(glm::vec2(22, 118), glm::vec2(15, 14));
  std::vector<engine::utils::Rectangle> defend_right;
  defend_right.emplace_back(glm::vec2(41, 118), glm::vec2(15, 14));
  std::vector<engine::utils::Rectangle> defend_left;
  defend_left.emplace_back(glm::vec2(60, 118), glm::vec2(15, 14));

  std::vector<engine::utils::Rectangle> attack_bottom;
  attack_bottom.emplace_back(glm::vec2(3, 99), glm::vec2(15, 14));
  attack_bottom.emplace_back(glm::vec2(22, 99), glm::vec2(15, 14));
  attack_bottom.emplace_back(glm::vec2(41, 99), glm::vec2(15, 14));

  std::vector<engine::utils::Rectangle> attack_top;
  attack_top.emplace_back(glm::vec2(3., 80), glm::vec2(15, 14));
  attack_top.emplace_back(glm::vec2(22, 80), glm::vec2(15, 14));
  attack_top.emplace_back(glm::vec2(41, 80), glm::vec2(15, 14));

  std::vector<engine::utils::Rectangle> attack_right;
  attack_right.emplace_back(glm::vec2(3., 61), glm::vec2(15, 14));
  attack_right.emplace_back(glm::vec2(22, 61), glm::vec2(15, 14));
  attack_right.emplace_back(glm::vec2(41, 61), glm::vec2(15, 14));

  std::vector<engine::utils::Rectangle> attack_left;
  attack_left.emplace_back(glm::vec2(3., 42), glm::vec2(15, 14));
  attack_left.emplace_back(glm::vec2(22, 42), glm::vec2(15, 14));
  attack_left.emplace_back(glm::vec2(41, 42), glm::vec2(15, 14));

  std::vector<engine::utils::Rectangle> moving;
  moving.emplace_back(glm::vec2(3, 23), glm::vec2(15, 14));
  moving.emplace_back(glm::vec2(22, 23), glm::vec2(15, 14));
  moving.emplace_back(glm::vec2(41, 23), glm::vec2(15, 14));

  std::vector<engine::utils::Rectangle> stand;
  stand.emplace_back(glm::vec2(3, 23), glm::vec2(15, 14));

  std::vector<engine::utils::Rectangle> death;
  death.emplace_back(glm::vec2(3, 4), glm::vec2(22, 14));

  auto texture_atlas =
      Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
          "assets/spritesheets/guerrero.png");

  SpriteAnimation::AnimationClip moving_bottom_anim(
      "moving_bottom", texture_atlas, moving_bottom, 100.0f);
  SpriteAnimation::AnimationClip moving_top_anim("moving_top", texture_atlas,
                                                 moving_top, 100.0f);
  SpriteAnimation::AnimationClip moving_right_anim(
      "moving_right", texture_atlas, moving_right, 100.0f);
  SpriteAnimation::AnimationClip moving_left_anim("moving_left", texture_atlas,
                                                  moving_left, 100.0f);
  SpriteAnimation::AnimationClip defend_bottom_anim(
      "defend_bottom", texture_atlas, defend_bottom, 100.0f);
  SpriteAnimation::AnimationClip defend_top_anim("defend_top", texture_atlas,
                                                 defend_top, 100.0f);
  SpriteAnimation::AnimationClip defend_right_anim(
      "defend_right", texture_atlas, defend_right, 100.0f);
  SpriteAnimation::AnimationClip defend_left_anim("defend_left", texture_atlas,
                                                  defend_left, 100.0f);
  SpriteAnimation::AnimationClip attack_bottom_anim(
      "attack_bottom", texture_atlas, attack_bottom, 100.0f);
  SpriteAnimation::AnimationClip attack_top_anim("attack_top", texture_atlas,
                                                 attack_top, 100.0f);
  SpriteAnimation::AnimationClip attack_right_anim(
      "attack_right", texture_atlas, attack_right, 100.0f);
  SpriteAnimation::AnimationClip attack_left_anim("attack_left", texture_atlas,
                                                  attack_left, 100.0f);
  SpriteAnimation::AnimationClip moving_anim("moving", texture_atlas, moving,
                                             100.0f);
  SpriteAnimation::AnimationClip stand_still("stand", texture_atlas, stand, 100.0f);
  SpriteAnimation::AnimationClip death_anim("death", texture_atlas, death,
                                            100.0f);

  SpriteAnimation anim({moving_bottom_anim, moving_top_anim, moving_right_anim,
                        moving_left_anim, defend_bottom_anim, defend_top_anim,
                        defend_right_anim, defend_left_anim, attack_bottom_anim,
                        attack_top_anim, attack_right_anim, attack_left_anim, death_anim});

  
  player.assign<SpriteAnimation>(anim);
  player.assign<Sprite>(texture_atlas);
  entities_created.push_back(player);

  // adding legs
  auto legs = entities.create();
  legs.assign<Physics>(glm::vec3(0, 0, 0));
  legs.assign<Transform>(glm::vec3(0.0f, 0.0f, 0.0f),
                                 player.component<Transform>().get());

  SpriteAnimation legs_anim({moving_anim, stand_still});
  legs.assign<SpriteAnimation>(legs_anim);
  legs.assign<Legs>();
  legs.assign<Sprite>(texture_atlas);
  ParentLink parentLink;
  parentLink.owner = player;
  legs.assign<ParentLink>(parentLink);
  entities_created.push_back(legs);


  // adding sword entity
  auto sword_hitbox = entities.create();
  sword_hitbox.assign<Transform>(glm::vec3(0.0f, -9.0f, 0.0f),
                                 player.component<Transform>().get());
  sword_hitbox.assign<AABBCollider>(glm::vec2(0.0f, 0.0f),
                                    glm::vec2(6.0f, 6.0f), true);
  sword_hitbox.assign<Physics>(glm::vec3(0.0f, 0.0f, 0.0f));
  MeleeWeapon weapon_cmp;
  weapon_cmp.damage = 10.0f;
  weapon_cmp.owner = player;
  weapon_cmp.drawn = false;
  sword_hitbox.assign<MeleeWeapon>(weapon_cmp);
  entities_created.push_back(sword_hitbox);
  return entities_created;
}

std::vector<entityx::Entity> EntityFactory::MakeGhost(
    entityx::EntityManager &entities, const glm::vec3 &position) {
  std::vector<entityx::Entity> entities_created;
  entityx::Entity ghost = entities.create();
  ghost.assign<Transform>(position);
  ghost.assign<AABBCollider>(glm::vec2(0, 0), glm::vec2(5, 8));
  ghost.assign<Health>(100.0f);
  ghost.assign<Physics>(glm::vec3(0, 0, 0));
  std::vector<ColorAnimation::KeyFrame> color_frames;
  color_frames.emplace_back(glm::vec3(1.0f, -0.3f, 0.0f), 0.2f);
  color_frames.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f), 0.2f);
  ghost.assign<ColorAnimation>(std::move(color_frames));
  ghost.assign<Ghost>();

  std::vector<engine::utils::Rectangle> moving_bottom;
  moving_bottom.emplace_back(glm::vec2(3, 88), glm::vec2(8, 13));
  moving_bottom.emplace_back(glm::vec2(15, 88), glm::vec2(8, 13));

  std::vector<engine::utils::Rectangle> moving_top;
  moving_bottom.emplace_back(glm::vec2(3, 71), glm::vec2(8, 13));
  moving_bottom.emplace_back(glm::vec2(15, 71), glm::vec2(8, 13));

  std::vector<engine::utils::Rectangle> moving_right;
  moving_bottom.emplace_back(glm::vec2(3, 54), glm::vec2(8, 13));
  moving_bottom.emplace_back(glm::vec2(15, 54), glm::vec2(8, 13));

  std::vector<engine::utils::Rectangle> moving_left;
  moving_bottom.emplace_back(glm::vec2(3, 37), glm::vec2(8, 13));
  moving_bottom.emplace_back(glm::vec2(15, 37), glm::vec2(8, 13));

  auto texture_atlas =
      Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
          "assets/spritesheets/fantasma.png");

  SpriteAnimation::AnimationClip moving_bottom_anim(
      "moving_bottom", texture_atlas, moving_bottom, 100.0f);
  SpriteAnimation::AnimationClip moving_top_anim("moving_top", texture_atlas,
                                                 moving_top, 100.0f);
  SpriteAnimation::AnimationClip moving_right_anim(
      "moving_right", texture_atlas, moving_right, 100.0f);
  SpriteAnimation::AnimationClip moving_left_anim("moving_left", texture_atlas,
                                                  moving_left, 100.0f);

  SpriteAnimation anim({moving_bottom_anim, moving_top_anim, moving_right_anim,
                        moving_left_anim});  

  ghost.assign<SpriteAnimation>(anim);
  ghost.assign<Sprite>(texture_atlas);


  //ghost.assign<Sprite>(texture_atlas,
    //                   Rectangle(glm::vec2(200, 200), glm::vec2(16, 16)));
  
  entities_created.push_back(ghost);

  // adding hitbox as a second entity
  auto attack_hitbox = entities.create();
  attack_hitbox.assign<Transform>(glm::vec3(0.0f, 0.0f, 0.0f),
                                  ghost.component<Transform>().get());
  attack_hitbox.assign<AABBCollider>(glm::vec2(0.0f, 0.0f),
                                     glm::vec2(9.0f, 9.0f), true);
  attack_hitbox.assign<Physics>(glm::vec3(0.0f, 0.0f, 0.0f));
  entities_created.push_back(attack_hitbox);
  return entities_created;
}

// COSAS QUE PREGUNTAR A MARIUS:
//			- ACLARAR TEMA DE LAS PIERNAS
//			- LO QUE ESTA ESCRITO DEBAJO
//			- TEMA DE LOS TICKS DE DAÑO
//			- TEMA DE EMPUJAR HACIA ATRAS AL COMERTE DAÑO

/*
std::vector<entityx::Entity> EntityFactory::MakeTurret(entityx::EntityManager &entities, const glm::vec3 &position) {
	
  std::vector<entityx::Entity> entities_created;
  entityx::Entity turret = entities.create();

  turret.assign<Transform>(position);
  turret.assign<AABBCollider>(glm::vec2(0, 0), glm::vec2(8, 8));
  turret.assign<Health>(50.0f);
  turret.assign<Physics>(glm::vec3(0, 0, 0));
  std::vector<ColorAnimation::KeyFrame> color_frames;
  color_frames.emplace_back(glm::vec3(1.0f, -0.3f, 0.0f), 0.2f);
  color_frames.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f), 0.2f);
  turret.assign<ColorAnimation>(std::move(color_frames));
  turret.assign<Turret>();

  // Si no tiene animacion y es solo una cabeza que se mueve como se hace
  std::vector<engine::utils::Rectangle> moving;
  moving.emplace_back(glm::vec2(3, 88), glm::vec2(8, 13));
  

  


  auto texture_atlas =
      Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
          "assets/spritesheets/torreta.png");
  	
}*/