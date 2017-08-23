#define _USE_MATH_DEFINES
#include <engine/core/engine.h>
#include <engine/core/event_manager.h>
#include <engine/core/audio_manager.h>

#include "systems.h"

#include <engine/components/common/physics.h>
#include <engine/components/common/transform.h>
#include <engine/components/two_d/aabb_collider.h>
#include <engine/components/two_d/color_animation.h>
#include <engine/components/two_d/sprite_animation.h>
#include <time.h>
#include <cmath>
#include "events.h"

#include <GLFW/glfw3.h>
 
using namespace engine::core;
using namespace engine::components::two_d;
using namespace engine::components::common;
using namespace engine::events;

void KnightAnimationSystem::update(entityx::EntityManager &es,
                                   entityx::EventManager &events,
                                   entityx::TimeDelta dt) {
  entityx::ComponentHandle<SpriteAnimation> animation;
  entityx::ComponentHandle<Physics> physics;
  entityx::ComponentHandle<KnightAttack> attack;
  std::string animToPlay;
  //auto soundEngine = 
  //    Engine::GetInstance().Get<AudioManager>();

  for (entityx::Entity e1 :
       es.entities_with_components(animation, physics, attack)) {
    if (attack->is_attacking) {
      switch (attack->orientation) {
        case KnightAttack::Orientation::UP:
          animToPlay = "attack_top";
          break;
        case KnightAttack::Orientation::DOWN:
          animToPlay = "attack_bottom";
          break;
        case KnightAttack::Orientation::LEFT:
          animToPlay = "attack_left";
          break;
        case KnightAttack::Orientation::RIGHT:
          animToPlay = "attack_right";
          break;
      }
    } else if (physics->velocity.x > 0) {
      animToPlay = "moving_right";
    } else if (physics->velocity.x < 0) {
      animToPlay = "moving_left";
    } else if (physics->velocity.y > 0) {
      animToPlay = "moving_top";
    } else if (physics->velocity.y < 0) {
      animToPlay = "moving_bottom";
    } else {
      animToPlay = "moving_top";
    }
    animation->Play(animToPlay);
  }
}

void GhostAnimationSystem::update(entityx::EntityManager &es,
                                  entityx::EventManager &events,
                                  entityx::TimeDelta dt) {
  //  /* initialize random seed: */
  //  srand(time(NULL));
  //
  //  // Buscamos la entidad del jugador, en función de su posición moveremos al
  //  // fantasma
  //  entityx::ComponentHandle<Player> player;
  //  entityx::ComponentHandle<engine::physics::common::Physics> physics_player;
  //  entityx::ComponentHandle<KnightAttack> attack;
  //  entityx::ComponentHandle<Position> position_player;
  //  soundEngine->setSoundVolume(0.1);
  //
  //  for (entityx::Entity e : es.entities_with_components(
  //           player, physics_player, attack, position_player)) {
  //  }
  //
  //  // para todos los fantasmas, hacer que se muevan hacia el jugador
  //  entityx::ComponentHandle<GhostAnimation> ghostAnimation;
  //  entityx::ComponentHandle<Graphics> graphics;
  //  entityx::ComponentHandle<Position> position_ghost;
  //  entityx::ComponentHandle<engine::physics::common::Physics> physics_ghost;
  //  entityx::ComponentHandle<Body> body_ghost;
  //  entityx::ComponentHandle<Body> body;
  //
  //  int currentSec = (int)(glfwGetTime() * 100.0);
  //
  //  for (entityx::Entity e1 :
  //       es.entities_with_components(ghostAnimation, physics_ghost, graphics,
  //                                   position_ghost, body_ghost)) {
  //    glm::vec2 v;
  //    for (entityx::Entity e2 : es.entities_with_components(body)) {
  //      if (position_player->position.y > position_ghost->position.y) {
  //        if ((currentSec % 200 == 99 || (currentSec % 200 == 198))) {
  //          // soundEngine->play2D("assets/media/fx/talk.wav");
  //        }
  //        v.y = SPEED_GHOST;
  //      }
  //      if (position_player->position.y < position_ghost->position.y) {
  //        if ((currentSec % 200 == 99 || (currentSec % 200 == 198))) {
  //          // soundEngine->play2D("assets/media/fx/talk.wav");
  //        }
  //        v.y = -SPEED_GHOST;
  //      }
  //      if (position_player->position.x > position_ghost->position.x) {
  //        if ((currentSec % 200 == 99 || (currentSec % 200 == 198))) {
  //          // soundEngine->play2D("assets/media/fx/talk.wav");
  //        }
  //        v.x = SPEED_GHOST;
  //      }
  //      if (position_player->position.x < position_ghost->position.x) {
  //        if ((currentSec % 200 == 99 || (currentSec % 200 == 198))) {
  //          // soundEngine->play2D("assets/media/fx/talk.wav");
  //        }
  //        v.x = -SPEED_GHOST;
  //      }
  //    } /* for */
  //    physics_ghost->velocity = decompose(v);
  //
  //    if (position_player->position.y > position_ghost->position.y + 10) {
  //      getNext(ghostAnimation, graphics, dt, ghostAnimation->mov_top);
  //    } else if (position_player->position.y < position_ghost->position.y) {
  //      getNext(ghostAnimation, graphics, dt, ghostAnimation->mov_down);
  //    }
  //  }
}

MenuInputSystem::MenuInputSystem()
    : up_pressed_(false), down_pressed_(false), enter_pressed_(false) {
  Engine::GetInstance().Get<EventManager>().Subscribe<KeyPressed>(*this);
  Engine::GetInstance().Get<EventManager>().Subscribe<KeyReleased>(*this);
}

void MenuInputSystem::update(entityx::EntityManager &es,
                             entityx::EventManager &events,
                             entityx::TimeDelta dt) {
  entityx::ComponentHandle<ArrowMenu> arrow_menu;
  entityx::ComponentHandle<Transform> transform;
  for (entityx::Entity entity :
       es.entities_with_components(arrow_menu, transform)) {
    auto new_position = transform->GetLocalPosition();
    if (up_pressed_) {
      up_pressed_ = false;
      switch (arrow_menu->option) {
        case ArrowMenu::Option::JUGAR:
          break;
        case ArrowMenu::Option::OPCIONES:
          new_position.y += 70;
          arrow_menu->option = ArrowMenu::Option::JUGAR;
          break;
        case ArrowMenu::Option::SALIR:
          new_position.y += 70;
          arrow_menu->option = ArrowMenu::Option::OPCIONES;
          break;
      }
    }
    if (down_pressed_) {
      down_pressed_ = false;
      switch (arrow_menu->option) {
        case ArrowMenu::Option::JUGAR:
          new_position.y -= 70;
          arrow_menu->option = ArrowMenu::Option::OPCIONES;
          break;
        case ArrowMenu::Option::OPCIONES:
          new_position.y -= 70;
          arrow_menu->option = ArrowMenu::Option::SALIR;
          break;
        case ArrowMenu::Option::SALIR:
          break;
      }
    }
    if (enter_pressed_) {
      switch (arrow_menu->option) {
        case ArrowMenu::Option::JUGAR:
          events.emit<StartGame>();
          break;
        case ArrowMenu::Option::OPCIONES:
          break;
        case ArrowMenu::Option::SALIR:
          exit(0);
          break;
      }
    }
    transform->SetLocalPosition(new_position);
  }
}

void MenuInputSystem::receive(const KeyPressed &key_pressed) {
  if (key_pressed.key == GLFW_KEY_UP) {
    up_pressed_ = true;
  } else if (key_pressed.key == GLFW_KEY_DOWN) {
    down_pressed_ = true;
  } else if (key_pressed.key == GLFW_KEY_ENTER) {
    enter_pressed_ = true;
  }
}

void MenuInputSystem::receive(const KeyReleased &key_released) {
  if (key_released.key == GLFW_KEY_UP) {
    up_pressed_ = false;
  } else if (key_released.key == GLFW_KEY_DOWN) {
    down_pressed_ = false;
  } else if (key_released.key == GLFW_KEY_ENTER) {
    enter_pressed_ = false;
  }
}

void OptionsInputSystem::update(entityx::EntityManager &es,
                                entityx::EventManager &events,
                                entityx::TimeDelta dt) {
  entityx::ComponentHandle<ArrowOptions> arrowOptions;
  entityx::ComponentHandle<Transform> position;
  glm::vec3 pos1(325, 247, 0);
  glm::vec3 pos2(325, 157, 0);
  glm::vec3 pos3(325, 65, 0);
  for (entityx::Entity entity :
       es.entities_with_components(arrowOptions, position)) {
    if (true) {
      switch (arrowOptions->option) {
        case ArrowOptions::Option::MUSIC_VOL:
          break;
        case ArrowOptions::Option::FX_VOL:
          position->SetLocalPosition(pos1);
          arrowOptions->option = ArrowOptions::Option::MUSIC_VOL;
          break;
        case ArrowOptions::Option::SALIR:
          position->SetLocalPosition(pos2);
          arrowOptions->option = ArrowOptions::Option::FX_VOL;
          break;
      }
    }
    if (true) {
      switch (arrowOptions->option) {
        case ArrowOptions::Option::MUSIC_VOL:
          position->SetLocalPosition(pos2);
          arrowOptions->option = ArrowOptions::Option::FX_VOL;
          break;
        case ArrowOptions::Option::FX_VOL:
          position->SetLocalPosition(pos3);
          arrowOptions->option = ArrowOptions::Option::SALIR;
          break;
        case ArrowOptions::Option::SALIR:
          break;
      }
    }
    if (true) {
      switch (arrowOptions->option) {
        case ArrowOptions::Option::MUSIC_VOL:
          break;
        case ArrowOptions::Option::FX_VOL:
          break;
        case ArrowOptions::Option::SALIR:
          break;
      }
    }
  }
}

const float PlayerInputSystem::kSpeed = 150.0f;

const float PlayerInputSystem::kAttackDuration = 500.0f;

PlayerInputSystem::PlayerInputSystem()
    : time_passed_since_last_attack_(kAttackDuration) {
  keys_.emplace(GLFW_KEY_W, false);
  keys_.emplace(GLFW_KEY_S, false);
  keys_.emplace(GLFW_KEY_A, false);
  keys_.emplace(GLFW_KEY_D, false);
  keys_.emplace(GLFW_KEY_UP, false);
  keys_.emplace(GLFW_KEY_DOWN, false);
  keys_.emplace(GLFW_KEY_LEFT, false);
  keys_.emplace(GLFW_KEY_RIGHT, false);
  Engine::GetInstance().Get<EventManager>().Subscribe<KeyPressed>(*this);
  Engine::GetInstance().Get<EventManager>().Subscribe<KeyReleased>(*this);
}

void PlayerInputSystem::receive(const KeyPressed &key_pressed) {
  if (keys_.count(key_pressed.key)) {
    keys_[key_pressed.key] = true;
  }
}

void PlayerInputSystem::receive(const KeyReleased &key_released) {
  if (keys_.count(key_released.key)) {
    keys_[key_released.key] = false;
  }
}

void PlayerInputSystem::update(entityx::EntityManager &es,
                               entityx::EventManager &events,
                               entityx::TimeDelta dt) {
  entityx::Entity player_entity;
  for (auto e : es.entities_with_components<Player>()) {
    player_entity = e;
  }

  entityx::ComponentHandle<Transform> weapon_transform;
  entityx::ComponentHandle<MeleeWeapon> weapon_info;
  entityx::ComponentHandle<AABBCollider> weapon_collider;
  for (auto e : es.entities_with_components(weapon_transform, weapon_info,
                                            weapon_collider)) {
    if (weapon_info->owner == player_entity) {
      break;
    }
  }

  es.each<Player, Physics, KnightAttack>([&](entityx::Entity entity,
                                             Player &player, Physics &physics,
                                             KnightAttack &attack) {
    glm::vec3 new_velocity(0.0f, 0.0f, 0.0f);
    if (keys_[GLFW_KEY_W]) {
      new_velocity.y += 1.0f;
    }
    if (keys_[GLFW_KEY_S]) {
      new_velocity.y += -1.0f;
    }
    if (keys_[GLFW_KEY_A]) {
      new_velocity.x += -1.0f;
    }
    if (keys_[GLFW_KEY_D]) {
      new_velocity.x += 1.0f;
    }
    if (new_velocity != glm::vec3(0.0f, 0.0f, 0.0f)) {
      new_velocity = glm::normalize(new_velocity) * kSpeed;
      // play sound
    }
    physics.velocity = new_velocity;

    weapon_info->drawn = false;
    time_passed_since_last_attack_ += (dt * 1000.0f);
    if (time_passed_since_last_attack_ >= PlayerInputSystem::kAttackDuration) {
      time_passed_since_last_attack_ = 0.0f;
      attack.is_attacking = true;
      weapon_info->drawn = true;
      // HACK: dont harcode the weapon info
      // use actual player width, height and transform to position the weapon
      if (keys_[GLFW_KEY_UP]) {
        attack.orientation = KnightAttack::Orientation::UP;
        weapon_collider->half_size = glm::vec2(6.0f, 1.0f);
        weapon_transform->SetLocalPosition(glm::vec3(0.0f, 9.0f, 0.0f));
      } else if (keys_[GLFW_KEY_DOWN]) {
        attack.orientation = KnightAttack::Orientation::DOWN;
        weapon_collider->half_size = glm::vec2(6.0f, 1.0f);
        weapon_transform->SetLocalPosition(glm::vec3(0.0f, -9.0f, 0.0f));
      } else if (keys_[GLFW_KEY_LEFT]) {
        attack.orientation = KnightAttack::Orientation::LEFT;
        weapon_collider->half_size = glm::vec2(1.0f, 6.0f);
        weapon_transform->SetLocalPosition(glm::vec3(-9.0f, 0.0f, 0.0f));
      } else if (keys_[GLFW_KEY_RIGHT]) {
        attack.orientation = KnightAttack::Orientation::RIGHT;
        weapon_collider->half_size = glm::vec2(1.0f, 6.0f);
        weapon_transform->SetLocalPosition(glm::vec3(9.0f, 0.0f, 0.0f));
      } else {
        attack.is_attacking = false;
        weapon_info->drawn = false;
      }

      if (attack.is_attacking) {
        // play sound
      }
    }
  });
}

const float GhostIaSystem::kSpeed = 50.0f;

void GhostIaSystem::update(entityx::EntityManager &es,
                           entityx::EventManager &events,
                           entityx::TimeDelta dt) {
  glm::vec3 player_position;
  es.each<Player, Transform>(
      [&](entityx::Entity entity, Player &player, Transform &transform) {
        player_position = transform.GetWorldPosition();
      });

  es.each<Ghost, Transform, Physics>([&](entityx::Entity entity, Ghost &ghost,
                                         Transform &transform,
                                         Physics &physics) {
    physics.velocity =
        glm::normalize(player_position - transform.GetWorldPosition()) * kSpeed;
  });
}

void KnightAttackSystem::configure(entityx::EventManager &event_manager) {
  event_manager.subscribe<Collision>(*this);
}

void KnightAttackSystem::receive(const Collision &collision) {
  auto collision_copy = collision;
  if (!collision_copy.e0.valid() || !collision_copy.e1.valid()) {
    return;
  }
  auto e0_weapon = collision_copy.e0.component<MeleeWeapon>();
  auto e1_weapon = collision_copy.e1.component<MeleeWeapon>();
  if (e0_weapon && e0_weapon->drawn && e0_weapon->owner.component<Player>() &&
      collision_copy.e1.component<Ghost>()) {
    auto e1_health = collision_copy.e1.component<Health>();
    e1_health->hp -= e0_weapon->damage;
    auto e1_color_animation = collision_copy.e1.component<ColorAnimation>();
    e1_color_animation->Play();
  } else if (e1_weapon && e1_weapon->drawn &&
             e1_weapon->owner.component<Player>() &&
             collision_copy.e0.component<Ghost>()) {
    auto e0_health = collision_copy.e0.component<Health>();
    e0_health->hp -= e1_weapon->damage;
  }
}

void KnightAttackSystem::update(entityx::EntityManager &es,
                                entityx::EventManager &events,
                                entityx::TimeDelta dt) {}

void HealthSystem::update(entityx::EntityManager &es,
                          entityx::EventManager &events,
                          entityx::TimeDelta dt) {
  es.each<Health>([&](entityx::Entity entity, Health &health) {
    if (health.hp <= 0.0f) {
      entity.destroy();
    }
  });
}
