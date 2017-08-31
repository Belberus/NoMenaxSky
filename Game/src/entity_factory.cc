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
  player.assign<Health>(200.0f, "assets/media/fx/gaunt/default/death.wav");

  std::vector<ColorAnimation::KeyFrame> color_frames;
  color_frames.emplace_back(glm::vec3(1.0f, -0.3f, 0.0f), 0.2f);
  color_frames.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f), 0.2f);
  player.assign<ColorAnimation>(std::move(color_frames));

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
  ghost.assign<AABBCollider>(glm::vec2(0, 2), glm::vec2(3, 4));
  ghost.assign<Physics>(glm::vec3(0, 0, 0));
  std::vector<ColorAnimation::KeyFrame> color_frames;
  color_frames.emplace_back(glm::vec3(1.0f, -0.3f, 0.0f), 0.2f);
  color_frames.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f), 0.2f);
  ghost.assign<ColorAnimation>(std::move(color_frames));
  ghost.assign<Ghost>();
  ghost.assign<Health>(20.0f, "assets/media/fx/ghost/default/death.wav");
  
  std::vector<engine::utils::Rectangle> moving_bottom;
  moving_bottom.emplace_back(glm::vec2(3, 88), glm::vec2(8, 13));
  moving_bottom.emplace_back(glm::vec2(15, 88), glm::vec2(8, 13));

  std::vector<engine::utils::Rectangle> moving_top;
  moving_top.emplace_back(glm::vec2(3, 71), glm::vec2(8, 13));
  moving_top.emplace_back(glm::vec2(15, 71), glm::vec2(8, 13));

  std::vector<engine::utils::Rectangle> moving_right;
  moving_right.emplace_back(glm::vec2(3, 54), glm::vec2(8, 13));
  moving_right.emplace_back(glm::vec2(15, 54), glm::vec2(8, 13));

  std::vector<engine::utils::Rectangle> moving_left;
  moving_left.emplace_back(glm::vec2(3, 37), glm::vec2(8, 13));
  moving_left.emplace_back(glm::vec2(15, 37), glm::vec2(8, 13));

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
  
  entities_created.push_back(ghost);

  // adding hitbox as a second entity
  auto attack_hitbox = entities.create();
  attack_hitbox.assign<GhostHitBox>(5.0f, ghost);
  attack_hitbox.assign<Transform>(glm::vec3(0.0f, 0.0f, 0.0f),
                                  ghost.component<Transform>().get());
  attack_hitbox.assign<AABBCollider>(glm::vec2(0.0f, 2.0f),
                                     glm::vec2(5.0f, 5.0f), true);
  attack_hitbox.assign<Physics>(glm::vec3(0.0f, 0.0f, 0.0f));
  entities_created.push_back(attack_hitbox);
  return entities_created;
}

std::vector<entityx::Entity> EntityFactory::MakeTurret(entityx::EntityManager &entities, const glm::vec3 &position) {
	
  std::vector<entityx::Entity> entities_created;
  entityx::Entity turret = entities.create();

  turret.assign<Transform>(position);
  turret.assign<AABBCollider>(glm::vec2(0, 0), glm::vec2(5, 10));
  turret.assign<Physics>(glm::vec3(0, 0, 0));
  std::vector<ColorAnimation::KeyFrame> color_frames;
  color_frames.emplace_back(glm::vec3(1.0f, -0.3f, 0.0f), 0.2f);
  color_frames.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f), 0.2f);
  turret.assign<ColorAnimation>(std::move(color_frames));
  turret.assign<Turret>();
  turret.assign<Health>(40.0f, "assets/media/fx/turret/default/death.wav");

  engine::utils::Rectangle head (glm::vec2(3, 62), glm::vec2(15, 20));
    
  std::vector<engine::utils::Rectangle> moving;
  moving.emplace_back(glm::vec2(3, 38), glm::vec2(15, 20));
  moving.emplace_back(glm::vec2(22, 38), glm::vec2(15, 20));
  moving.emplace_back(glm::vec2(41, 38), glm::vec2(15, 20));

  std::vector<engine::utils::Rectangle> stand;
  stand.emplace_back(glm::vec2(22, 62), glm::vec2(15, 20));

  auto texture_atlas =
      Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
          "assets/spritesheets/torreta.png");
  	
  SpriteAnimation::AnimationClip moving_anim(
      "moving", texture_atlas, moving, 100.0f);
  SpriteAnimation::AnimationClip stand_anim(
      "stand", texture_atlas, stand, 100.0f);

  // La cabeza no tiene animacion porque no se mueve
  turret.assign<Sprite>(texture_atlas, head);
  entities_created.push_back(turret);
  
  entityx::Entity legs = entities.create();

  legs.assign<Physics>(glm::vec3(0, 0, 0));
  legs.assign<Transform>(glm::vec3(0.0f, 0.0f, 0.0f),
                                 turret.component<Transform>().get());

  SpriteAnimation legs_anim({moving_anim, stand_anim});
  legs.assign<SpriteAnimation>(legs_anim);
  legs.assign<TurretLegs>();
  legs.assign<Sprite>(texture_atlas);
  ParentLink parentLink;
  parentLink.owner = turret;
  legs.assign<ParentLink>(parentLink);

  entities_created.push_back(legs);
  return entities_created;
}

 std::vector<entityx::Entity> EntityFactory::MakeTrap(
      entityx::EntityManager &entities, const glm::vec3 &position,  const std::string &orient){

 	std::vector<entityx::Entity> entities_created;
    entityx::Entity trap = entities.create();

    if (orient == "abajo") {
    	trap.assign<Trap>(Trap::Orientation::DOWN);
    } else if (orient == "arriba") {
    	trap.assign<Trap>(Trap::Orientation::UP);
    } else if (orient == "izquierda") {
    	trap.assign<Trap>(Trap::Orientation::LEFT);
    } else if (orient == "derecha") {
    	trap.assign<Trap>(Trap::Orientation::RIGHT);
    }

    trap.assign<Transform>(position);
    
    entities_created.push_back(trap);
    return entities_created;
 }

 std::vector<entityx::Entity> EntityFactory::MakeManueleth(
      entityx::EntityManager &entities, const glm::vec3 &position){

 	  std::vector<entityx::Entity> entities_created;
	  entityx::Entity manueleth = entities.create();

	  manueleth.assign<Transform>(position);
	  manueleth.assign<AABBCollider>(glm::vec2(0, 0), glm::vec2(5, 10));
	  manueleth.assign<Physics>(glm::vec3(0, 0, 0));
	  std::vector<ColorAnimation::KeyFrame> color_frames;
	  color_frames.emplace_back(glm::vec3(1.0f, -0.3f, 0.0f), 0.2f);
	  color_frames.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f), 0.2f);
	  manueleth.assign<ColorAnimation>(std::move(color_frames));
	  manueleth.assign<Manueleth>();
	  manueleth.assign<Health>(250.0f, "assets/media/fx/turret/default/death.wav");
	    
	  std::vector<engine::utils::Rectangle> talking;
	  talking.emplace_back(glm::vec2(3, 83), glm::vec2(19, 19));
	  talking.emplace_back(glm::vec2(26, 83), glm::vec2(19, 19));

	  std::vector<engine::utils::Rectangle> attacking;
	  attacking.emplace_back(glm::vec2(3, 37), glm::vec2(19, 19));
	  attacking.emplace_back(glm::vec2(26, 37), glm::vec2(19, 19));

	  std::vector<engine::utils::Rectangle> pushing;
	  pushing.emplace_back(glm::vec2(3, 60), glm::vec2(19, 19));

	  std::vector<engine::utils::Rectangle> death;
	  death.emplace_back(glm::vec2(3, 14), glm::vec2(19, 19));
	  death.emplace_back(glm::vec2(26, 14), glm::vec2(19, 19));

	  auto texture_atlas =
	      Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
	          "assets/spritesheets/manueleth.png");
	  	
	  SpriteAnimation::AnimationClip talking_anim(
	      "talking", texture_atlas, talking, 100.0f);
	  SpriteAnimation::AnimationClip attacking_anim(
	      "attacking", texture_atlas, attacking, 100.0f);
	   SpriteAnimation::AnimationClip pushing_anim(
	      "pushing", texture_atlas, pushing, 100.0f);
	    SpriteAnimation::AnimationClip death_anim(
	      "death", texture_atlas, death, 100.0f);

	  SpriteAnimation manueleth_anim({talking_anim, attacking_anim, pushing_anim, death_anim});
	  manueleth.assign<SpriteAnimation>(manueleth_anim); 
	  manueleth.assign<Sprite>(texture_atlas);
	  entities_created.push_back(manueleth);
	  return entities_created;
 }

std::vector<entityx::Entity> EntityFactory::MakeEnemyProjectile(entityx::EntityManager &entities, const glm::vec3 &position, const float &rotation, const glm::vec3 &velocity, const std::string &type) {

	std::vector<entityx::Entity> entities_created;
    entityx::Entity enemyProjectile = entities.create();
    std::vector<engine::utils::Rectangle> shoot;
    std::string loadTexture;

    Transform t(position);
    //enemyProjectile.assign<Transform>(position);
    glm::quat model_rotation;
    model_rotation = glm::rotate(model_rotation, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    t.SetLocalOrientation(model_rotation);
    enemyProjectile.assign<Transform>(t);

    if (type == "torreta") {
	  	enemyProjectile.assign<AABBCollider>(glm::vec2(3, 0), glm::vec2(2, 2));
	  	enemyProjectile.assign<Physics>(velocity);
	  	enemyProjectile.assign<EnemyProjectile>(15.0f);

	  	loadTexture = "assets/spritesheets/torreta.png";

	  	shoot.emplace_back(glm::vec2(3, 3), glm::vec2(15, 7));
	  	shoot.emplace_back(glm::vec2(22, 3), glm::vec2(15, 7));

	  } else if (type == "trampa") {
		enemyProjectile.assign<AABBCollider>(glm::vec2(3, 0), glm::vec2(2, 2));
		enemyProjectile.assign<Physics>(velocity);
		enemyProjectile.assign<EnemyProjectile>(20.0f);

		loadTexture = "assets/spritesheets/proyectil_trampa.png";

	  	shoot.emplace_back(glm::vec2(3, 3), glm::vec2(12, 5));
	  	shoot.emplace_back(glm::vec2(19, 3), glm::vec2(12, 5));
	  
	  } else if (type == "manueleth") {
	  	enemyProjectile.assign<AABBCollider>(glm::vec2(3, 0), glm::vec2(2, 2));
		enemyProjectile.assign<Physics>(velocity);
		enemyProjectile.assign<EnemyProjectile>(20.0f);

		loadTexture = "assets/spritesheets/manueleth.png";

	  	shoot.emplace_back(glm::vec2(3, 3), glm::vec2(12, 5));
	  	shoot.emplace_back(glm::vec2(19, 3), glm::vec2(12, 5));
	  }

    auto texture_atlas =
      Engine::GetInstance().Get<ResourceManager>().Load<Texture>(loadTexture);

    SpriteAnimation::AnimationClip shooting_anim(
      "shooting", texture_atlas, shoot, 100.0f);

	SpriteAnimation shoot_anim({shooting_anim});
	enemyProjectile.assign<SpriteAnimation>(shoot_anim);
	enemyProjectile.assign<Sprite>(texture_atlas);

	entities_created.push_back(enemyProjectile);
	return entities_created;
}