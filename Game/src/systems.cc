#define _USE_MATH_DEFINES
#include <engine/core/audio_manager.h>
#include <engine/core/engine.h>
#include <engine/core/event_manager.h>

#include "systems.h"

#include <engine/components/common/physics.h>
#include <engine/components/common/transform.h>
#include <engine/components/two_d/aabb_collider.h>
#include <engine/components/two_d/color_animation.h>
#include <engine/components/two_d/sprite_animation.h>
#include <engine/events/ignore_collision.h>
#include <time.h>
#include <cmath>
#include "entity_factory.h"
#include "events.h"

#include <iostream>
#include <fstream>
#include <string.h>

#include <GLFW/glfw3.h>

using namespace engine::core;
using namespace engine::components::two_d;
using namespace engine::components::common;
using namespace engine::events;

bool once = false;
void KnightAnimationSystem::update(entityx::EntityManager &es,
                                   entityx::EventManager &events,
                                   entityx::TimeDelta dt) {
  entityx::ComponentHandle<SpriteAnimation> animation;
  entityx::ComponentHandle<Physics> physics;
  entityx::ComponentHandle<KnightAttack> attack;
  entityx::ComponentHandle<Player> player;
  entityx::ComponentHandle<Shield> shield;
  std::string animToPlay;

  entityx::Entity player_entity;
  for (auto e : es.entities_with_components<Player>()) {
    player_entity = e;
  }

  for (entityx::Entity e0 :
       es.entities_with_components(shield)) {
  	if (shield->owner == player_entity) {
  		break;
  	}
  }
  for (entityx::Entity e1 :
       es.entities_with_components(animation, physics, attack, player)) {
    if(shield->active) {
      if(!once){
        once = true;
        Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/gaunt/warrior/alt.wav", false, 0.6f);
      }    	
    	switch (shield->orientation) {
        case Shield::Orientation::UP:
          animToPlay = "defend_top";
          break;
        case Shield::Orientation::DOWN:
          animToPlay = "defend_bottom";
          break;
        case Shield::Orientation::LEFT:
          animToPlay = "defend_left";
          break;
        case Shield::Orientation::RIGHT:
          animToPlay = "defend_right";
          break;
      }
    } else if (attack->is_attacking) {
      once = false;
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
      once = false;
      animToPlay = "moving_right";
      player->orientation = Player::Orientation::RIGHT;
      if (timer2 == 0.0) {
        Engine::GetInstance().Get<AudioManager>().PlaySound(
            "assets/media/fx/gaunt/default/mov.wav", false, 0.6f);
      }
    } else if (physics->velocity.x < 0) {
      once = false;
      animToPlay = "moving_left";
      player->orientation = Player::Orientation::LEFT;

      if (timer2 == 0.0) {
        Engine::GetInstance().Get<AudioManager>().PlaySound(
            "assets/media/fx/gaunt/default/mov.wav", false, 0.6f);
      }
    } else if (physics->velocity.y > 0) {
      once = false;
      animToPlay = "moving_top";
      player->orientation = Player::Orientation::UP;

      if (timer2 == 0.0) {
        Engine::GetInstance().Get<AudioManager>().PlaySound(
            "assets/media/fx/gaunt/default/mov.wav", false, 0.6f);
      }
    } else if (physics->velocity.y < 0) {
      once = false;
      animToPlay = "moving_bottom";
      player->orientation = Player::Orientation::DOWN;

      if (timer2 == 0.0) {
        Engine::GetInstance().Get<AudioManager>().PlaySound(
            "assets/media/fx/gaunt/default/mov.wav", false, 0.6f);
      }
    } else {
      once = false;
      if (lastAnim.empty()) {
        animToPlay = "moving_bottom";
      } else
        animToPlay = lastOrientation;
    }
    animation->Play(animToPlay);
    // If last anim was walking, save orientation
    if (animToPlay.find("moving") != std::string::npos) {
      lastOrientation = animToPlay;
    }
    // Save last anim anyway for sound purposes
    lastAnim = animToPlay;
  }
  if (lastAnim.find("attack") != std::string::npos) {
    if (timer == 0.0) {
      Engine::GetInstance().Get<AudioManager>().PlaySound(
          "assets/media/fx/gaunt/warrior/attack.wav", false, 0.8f);
      Engine::GetInstance().Get<AudioManager>().PlaySound(
          "assets/media/fx/gaunt/default/attack_2.wav", false, 0.6f);
    }
    timer += dt;
    if (timer >= 0.5) {
      timer = 0.0;
    }
    // Sonido de pasos en caso de que este atacando y moviendo
    if (physics->velocity.x != 0 || physics->velocity.y != 0) {
      if (timer2 == 0.0) {
        Engine::GetInstance().Get<AudioManager>().PlaySound(
            "assets/media/fx/gaunt/default/mov.wav", false, 0.6f);
      }
    }
  } else
    timer = 0.0;

  timer2 += dt;
  if (timer2 >= 0.2) {
    timer2 = 0.0;
  }
}

void KnightWalkingSystem::update(entityx::EntityManager &es,
                                 entityx::EventManager &events,
                                 entityx::TimeDelta dt) {
  entityx::ComponentHandle<Player> player;
  entityx::ComponentHandle<SpriteAnimation> animation;
  entityx::ComponentHandle<Physics> physics;
  entityx::ComponentHandle<ParentLink> parent;
  entityx::ComponentHandle<Legs> legs;
  std::string animToPlay;

  for (entityx::Entity e1 :
       es.entities_with_components(animation, parent, legs)) {
    if ((parent->owner.component<Physics>()->velocity.x < 0) ||
        (parent->owner.component<Physics>()->velocity.x > 0) ||
        (parent->owner.component<Physics>()->velocity.y > 0) ||
        (parent->owner.component<Physics>()->velocity.y < 0)) {
      animToPlay = "moving";
    } else {
      animToPlay = "stand";
    }
    animation->Play(animToPlay);
  }
}

void GhostAnimationSystem::update(entityx::EntityManager &es,
                                  entityx::EventManager &events,
                                  entityx::TimeDelta dt) {
  entityx::ComponentHandle<Ghost> ghost;
  entityx::ComponentHandle<Transform> position_ghost;
  entityx::ComponentHandle<Physics> physics_ghost;
  entityx::ComponentHandle<SpriteAnimation> animation;
  
  std::string animToPlay;

  for (entityx::Entity e0 : es.entities_with_components(
           ghost, position_ghost, physics_ghost, animation)) {
    // Estos dos solo provocan que el fantasma se colapse cambiando de animacion
    // cuando el caballero se encuentra en una diagonal
    /*if (physics_ghost->velocity.x < 0) {
            animToPlay = "moving_left";
            animation->Play(animToPlay);
    }
    if (physics_ghost->velocity.x > 0) {
            animToPlay = "moving_right";
            animation->Play(animToPlay);
    }*/
    if (physics_ghost->velocity.y < 0) {
      animToPlay = "moving_bottom";
      animation->Play(animToPlay);
    }
    if (physics_ghost->velocity.y > 0) {
      animToPlay = "moving_top";
      animation->Play(animToPlay);
    }
  }
}

void LancerAnimationSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt){
  entityx::ComponentHandle<Lancer> lancer;
  //entityx::ComponentHandle<Transform> position_lancer;
  entityx::ComponentHandle<Physics> physics;
  entityx::ComponentHandle<SpriteAnimation> animation;

  std::string animToPlay;

  for (entityx::Entity e : es.entities_with_components(
    lancer, physics, animation)){

    if (lancer->is_attacking){
      switch (lancer->atk_orientation) {
        case Lancer::AttackOrientation::ATK_UP:
          animToPlay = "attack_top";
          break;
        case Lancer::AttackOrientation::ATK_DOWN:
          animToPlay = "attack_bottom";
          break;
        case Lancer::AttackOrientation::ATK_LEFT:
          animToPlay = "attack_left";
          break;
        case Lancer::AttackOrientation::ATK_RIGHT:
          animToPlay = "attack_right";
          break;
      }
    } else if (physics->velocity.x > 0) {
      animToPlay = "moving_right";
      lancer->orientation = Lancer::LancerOrientation::RIGHT;
      if (timer2 == 0.0) {
        Engine::GetInstance().Get<AudioManager>().PlaySound(
            "assets/media/fx/lanc/default/mov.wav", false, 0.6f);
      }
    } else if (physics->velocity.x < 0) {
      animToPlay = "moving_left";
      lancer->orientation = Lancer::LancerOrientation::LEFT;

      if (timer2 == 0.0) {
        Engine::GetInstance().Get<AudioManager>().PlaySound(
            "assets/media/fx/lanc/default/mov.wav", false, 0.6f);
      }
    } else if (physics->velocity.y > 0) {
      animToPlay = "moving_top";
      lancer->orientation = Lancer::LancerOrientation::UP;

      if (timer2 == 0.0) {
        Engine::GetInstance().Get<AudioManager>().PlaySound(
            "assets/media/fx/lanc/default/mov.wav", false, 0.6f);
      }
    } else if (physics->velocity.y < 0) {
      animToPlay = "moving_bottom";
      lancer->orientation = Lancer::LancerOrientation::DOWN;

      if (timer2 == 0.0) {
        Engine::GetInstance().Get<AudioManager>().PlaySound(
            "assets/media/fx/lanc/default/mov.wav", false, 0.6f);
      }
    } 
    animation->Play(animToPlay);
  }
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
          events.emit<OptionMenu>();
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

OptionsInputSystem::OptionsInputSystem()
    : options_up_pressed_(false), options_down_pressed_(false), options_enter_pressed_(false),
      options_right_pressed_(false), options_left_pressed_(false) {
  std::string filename = "assets/config/opciones.txt";
  std::fstream file(filename.c_str(), std::ios_base::in);

  file >> mode >> music >> fx;

  file.close();

  Engine::GetInstance().Get<EventManager>().Subscribe<KeyPressed>(*this);
  Engine::GetInstance().Get<EventManager>().Subscribe<KeyReleased>(*this);
}

void OptionsInputSystem::update(entityx::EntityManager &es,
                                entityx::EventManager &events,
                                entityx::TimeDelta dt) {

  entityx::ComponentHandle<ArrowOptions> arrow_options;
  entityx::ComponentHandle<Transform> position;
  entityx::ComponentHandle<GameOptions> opciones;
  
  for (entityx::Entity entity :
       es.entities_with_components(arrow_options, position, opciones)) {
    auto new_position = position->GetLocalPosition();

    if (options_up_pressed_) {
      options_up_pressed_ = false;
      switch (arrow_options->option) {
        case ArrowOptions::Option::MODE:
          break;
        case ArrowOptions::Option::MUSIC_VOL:
          new_position.y += 70;

          if (opciones->modo == GameOptions::Modo::TWO_D &&
              opciones->musica == GameOptions::Musica::MUSIC_OFF){
            new_position.x -= 150;
          }

          if (opciones->modo == GameOptions::Modo::THREE_D &&
              opciones->musica == GameOptions::Musica::MUSIC_ON){
            new_position.x += 150;
          }

          arrow_options->option = ArrowOptions::Option::MODE;
          break;
        case ArrowOptions::Option::FX_VOL:
          new_position.y += 70;

          if (opciones->musica == GameOptions::Musica::MUSIC_ON &&
              opciones->efectos == GameOptions::Efectos::FX_OFF){
            new_position.x -= 150;
          }

          if (opciones->musica == GameOptions::Musica::MUSIC_OFF &&
              opciones->efectos == GameOptions::Efectos::FX_ON){
            new_position.x += 150;
          }

          arrow_options->option = ArrowOptions::Option::MUSIC_VOL;
          break;
        case ArrowOptions::Option::SALIR:
          new_position.y += 70;

          if (opciones->efectos == GameOptions::Efectos::FX_OFF){
            new_position.x += 150;
          }

          arrow_options->option = ArrowOptions::Option::FX_VOL;
          break;
      }
    }
    if (options_down_pressed_) {
      options_down_pressed_ = false;
      switch (arrow_options->option) {
        case ArrowOptions::Option::MODE:
          new_position.y -= 70;
          
          if (opciones->modo == GameOptions::Modo::TWO_D &&
              opciones->musica == GameOptions::Musica::MUSIC_OFF){
            new_position.x += 150;
          }

          if (opciones->modo == GameOptions::Modo::THREE_D &&
              opciones->musica == GameOptions::Musica::MUSIC_ON){
            new_position.x -= 150;
          }

          arrow_options->option = ArrowOptions::Option::MUSIC_VOL;
          break;
        case ArrowOptions::Option::MUSIC_VOL:
          new_position.y -= 70;
          
          if (opciones->musica == GameOptions::Musica::MUSIC_ON &&
              opciones->efectos == GameOptions::Efectos::FX_OFF){
            new_position.x += 150;
          }

          if (opciones->musica == GameOptions::Musica::MUSIC_OFF &&
              opciones->efectos == GameOptions::Efectos::FX_ON){
            new_position.x -= 150;
          }

          arrow_options->option = ArrowOptions::Option::FX_VOL;
          break;
        case ArrowOptions::Option::FX_VOL:
          new_position.y -= 70;

          if (opciones->efectos == GameOptions::Efectos::FX_OFF){
            new_position.x -= 150;
          }

          arrow_options->option = ArrowOptions::Option::SALIR;
          break;
        case ArrowOptions::Option::SALIR:
          break;
      }
    }
    if (options_right_pressed_) {
      options_right_pressed_ = false;
      switch (arrow_options->option) {
        case ArrowOptions::Option::MODE:
          switch (opciones->modo) {
            case GameOptions::Modo::TWO_D:
              new_position.x += 150;
              opciones->modo = GameOptions::Modo::THREE_D;
              mode = 0;
              break;
            case GameOptions::Modo::THREE_D:
              break; 
          }
          break;
        case ArrowOptions::Option::MUSIC_VOL:
          switch (opciones->musica) {
            case GameOptions::Musica::MUSIC_ON:
              new_position.x += 150;
              opciones->musica = GameOptions::Musica::MUSIC_OFF;
              music = 0;
              break;
            case GameOptions::Musica::MUSIC_OFF:
              break;
          }
          break;
        case ArrowOptions::Option::FX_VOL:
          switch (opciones->efectos) {
            case GameOptions::Efectos::FX_ON:
              new_position.x += 150;
              opciones->efectos = GameOptions::Efectos::FX_OFF;
              fx = 0;
              break;
            case GameOptions::Efectos::FX_OFF:
              break;
          }
          break;
        case ArrowOptions::Option::SALIR:
          break;
      }
    }
    if (options_left_pressed_) {
      options_left_pressed_ = false;
      switch (arrow_options->option) {
        case ArrowOptions::Option::MODE:
          switch (opciones->modo) {
            case GameOptions::Modo::TWO_D:
              break;
            case GameOptions::Modo::THREE_D:
              new_position.x -= 150;
              opciones->modo = GameOptions::Modo::TWO_D;
              mode = 1;
              break; 
          }
          break;
        case ArrowOptions::Option::MUSIC_VOL:
          switch (opciones->musica) {
            case GameOptions::Musica::MUSIC_ON:             
              break;
            case GameOptions::Musica::MUSIC_OFF:
              new_position.x -= 150;
              opciones->musica = GameOptions::Musica::MUSIC_ON;
              music = 1;
              break;
          }
          break;
        case ArrowOptions::Option::FX_VOL:
          switch (opciones->efectos) {
            case GameOptions::Efectos::FX_ON:             
              break;
            case GameOptions::Efectos::FX_OFF:
              new_position.x -= 150;
              opciones->efectos = GameOptions::Efectos::FX_ON;
              fx = 1;
              break;
          }
          break;
        case ArrowOptions::Option::SALIR:
          break;
      }
    }
    if (options_enter_pressed_) {
      options_enter_pressed_ = false;
      switch (arrow_options->option) {
        case ArrowOptions::Option::MODE:
    //       switch (opciones->modo) {
    //         case GameOptions::Modo::TWO_D:
    //           new_position.x += 250;
    //           opciones->modo = GameOptions::Modo::THREE_D;
    //           break;
    //         case GameOptions::Modo::THREE_D:
    //           new_position.x -= 250;
    //           opciones->modo = GameOptions::Modo::TWO_D;
    //           break; 
    //       }
           break;
        case ArrowOptions::Option::MUSIC_VOL:
    //       switch (opciones->musica) {
    //         case GameOptions::Musica::MUSIC_ON:
    //           new_position.x += 250;
    //           opciones->musica = GameOptions::Musica::MUSIC_OFF;
    //           break;
    //         case GameOptions::Musica::MUSIC_OFF:
    //           new_position.x -= 250;
    //           opciones->musica = GameOptions::Musica::MUSIC_ON;
    //           break;
    //       }
           break;
        case ArrowOptions::Option::FX_VOL:
    //       switch (opciones->efectos) {
    //         case GameOptions::Efectos::FX_ON:
    //           new_position.x += 250;
    //           opciones->efectos = GameOptions::Efectos::FX_OFF;
    //           break;
    //         case GameOptions::Efectos::FX_OFF:
    //           new_position.x -= 250;
    //           opciones->efectos = GameOptions::Efectos::FX_ON;
    //           break;
    //       }
           break;
        case ArrowOptions::Option::SALIR:
          std::string filename = "assets/config/opciones.txt";
          
          std::ofstream outfile;
          outfile.open(filename, std::ofstream::out | std::ofstream::trunc);
          outfile << mode << " " << music << " " << fx;
          outfile.close();
          
          events.emit<BackToMainMenu>();
          
          break;
      }
    }
    position->SetLocalPosition(new_position);
  }
}

void OptionsInputSystem::receive(const KeyPressed &key_pressed) {
  if (key_pressed.key == GLFW_KEY_UP) {
    options_up_pressed_ = true;
  } else if (key_pressed.key == GLFW_KEY_DOWN) {
    options_down_pressed_ = true;
  } else if (key_pressed.key == GLFW_KEY_ENTER) {
    options_enter_pressed_ = true;
  } else if (key_pressed.key == GLFW_KEY_RIGHT) {
    options_right_pressed_ = true;
  } else if (key_pressed.key == GLFW_KEY_LEFT) {
    options_left_pressed_ = true;
  }
}

void OptionsInputSystem::receive(const KeyReleased &key_released) {
  if (key_released.key == GLFW_KEY_UP) {
    options_up_pressed_ = false;
  } else if (key_released.key == GLFW_KEY_DOWN) {
    options_down_pressed_ = false;
  } else if (key_released.key == GLFW_KEY_ENTER) {
    options_enter_pressed_ = false;
  } else if (key_released.key == GLFW_KEY_RIGHT) {
    options_right_pressed_ = false;
  } else if (key_released.key == GLFW_KEY_LEFT) {
    options_left_pressed_ = false;
  }
}

const float PlayerInputSystem::kSpeed = 140.0f;

const float PlayerInputSystem::kAttackDuration = 250.0f;

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
  keys_.emplace(GLFW_KEY_SPACE, false);
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

  entityx::ComponentHandle<Transform> shield_transform;
  entityx::ComponentHandle<Shield> shield_info;
  entityx::ComponentHandle<AABBCollider> shield_collider;
  for (auto e : es.entities_with_components(shield_transform, shield_info,
                                            shield_collider)) {
    if (shield_info->owner == player_entity) {
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
    shield_info->time_passed += (dt * 1000.0f);
    time_passed_since_last_attack_ += (dt * 1000.0f);
    if (keys_[GLFW_KEY_SPACE]) {
    	if (keys_[GLFW_KEY_UP]) {
    		shield_info->active = true;
	        shield_info->orientation = Shield::Orientation::UP;
	        shield_transform->SetLocalPosition(glm::vec3(0.0f, 9.0f, 0.0f));
	        shield_collider->half_size  = glm::vec2(8.0f, 3.0f);
	      } else if (keys_[GLFW_KEY_DOWN]) {
	      	shield_info->active = true;
	        shield_info->orientation = Shield::Orientation::DOWN;
	        shield_transform->SetLocalPosition(glm::vec3(0.0f, -9.0f, 0.0f));
	        shield_collider->half_size  = glm::vec2(8.0f, 3.0f);
	      } else if (keys_[GLFW_KEY_LEFT]) {
	      	shield_info->active = true;
	        shield_info->orientation = Shield::Orientation::LEFT;
	        shield_transform->SetLocalPosition(glm::vec3(-9.0f, 0.0f, 0.0f));
	        shield_collider->half_size  = glm::vec2(3.0f, 8.0f);
	      } else if (keys_[GLFW_KEY_RIGHT]) {
	      	shield_info->active = true;
	        shield_info->orientation = Shield::Orientation::RIGHT;
	        shield_transform->SetLocalPosition(glm::vec3(9.0f, 0.0f, 0.0f));
	        shield_collider->half_size  = glm::vec2(3.0f, 8.0f);
	      } else {
	        shield_info->active = false;
	        weapon_info->drawn = false;
	        attack.is_attacking = false;
	      }
    } else {
    	if(shield_info->time_passed >= 2000.0f) {
    		shield_info->time_passed = 0.0f;
    		if (((shield_info->owner).component<Energy>()->energy += 20.0f) >= 100.0f) {
    			(shield_info->owner).component<Energy>()->energy = 100.0f;
    		} else (shield_info->owner).component<Energy>()->energy += 20.0f;
    	}
    	shield_info->active = false;
    	weapon_info->drawn = false;  
	    if (time_passed_since_last_attack_ >= PlayerInputSystem::kAttackDuration) {
	      time_passed_since_last_attack_ = 0.0f;
	      attack.is_attacking = true;
	      weapon_info->drawn = true;
	      // HACK: dont harcode the weapon info
	      // use actual player width, height and transform to position the weapon
	      if (keys_[GLFW_KEY_UP]) {
	      	if (keys_[GLFW_KEY_SPACE]) {
	      		attack.is_attacking = false;
	        	weapon_info->drawn = false;
	        	shield_info->active = true;
	        	shield_info->orientation = Shield::Orientation::UP;
		        shield_transform->SetLocalPosition(glm::vec3(0.0f, 9.0f, 0.0f));
		        shield_collider->half_size  = glm::vec2(8.0f, 3.0f);
	      	} else {
	      		attack.orientation = KnightAttack::Orientation::UP;	
		        weapon_collider->half_size = glm::vec2(6.0f, 6.0f);
		        weapon_transform->SetLocalPosition(glm::vec3(0.0f, 9.0f, 0.0f));
	      	}       
	      } else if (keys_[GLFW_KEY_DOWN]) {
	      	if (keys_[GLFW_KEY_SPACE]) {
	      		attack.is_attacking = false;
	        	weapon_info->drawn = false;
	        	shield_info->active = true;
	        	shield_info->orientation = Shield::Orientation::DOWN;
	        	shield_transform->SetLocalPosition(glm::vec3(0.0f, -9.0f, 0.0f));
	        	shield_collider->half_size  = glm::vec2(8.0f, 3.0f);
	      	} else {
	      		attack.orientation = KnightAttack::Orientation::DOWN;
		        weapon_collider->half_size = glm::vec2(6.0f, 6.0f);
		        weapon_transform->SetLocalPosition(glm::vec3(0.0f, -9.0f, 0.0f));
	      	}  
	      } else if (keys_[GLFW_KEY_LEFT]) {
	      	if (keys_[GLFW_KEY_SPACE]) {
	      		attack.is_attacking = false;
	        	weapon_info->drawn = false;
	        	shield_info->active = true;
	        	shield_info->orientation = Shield::Orientation::LEFT;
	        	shield_transform->SetLocalPosition(glm::vec3(-9.0f, 0.0f, 0.0f));
	        	shield_collider->half_size  = glm::vec2(3.0f, 8.0f);
	      	} else {
	      		attack.orientation = KnightAttack::Orientation::LEFT;
		        weapon_collider->half_size = glm::vec2(6.0f, 6.0f);
		        weapon_transform->SetLocalPosition(glm::vec3(-9.0f, 0.0f, 0.0f));
	      	}   
	      } else if (keys_[GLFW_KEY_RIGHT]) {
	      	if (keys_[GLFW_KEY_SPACE]) {
	      		attack.is_attacking = false;
	        	weapon_info->drawn = false;
	        	shield_info->active = true;
	        	shield_info->orientation = Shield::Orientation::RIGHT;
	        	shield_transform->SetLocalPosition(glm::vec3(9.0f, 0.0f, 0.0f));
	        	shield_collider->half_size  = glm::vec2(3.0f, 8.0f);
	      	} else {
	      		attack.orientation = KnightAttack::Orientation::RIGHT;
		        weapon_collider->half_size = glm::vec2(6.0f, 6.0f);
		        weapon_transform->SetLocalPosition(glm::vec3(9.0f, 0.0f, 0.0f));
	      	}	        
	      } else {
	        attack.is_attacking = false;
	        weapon_info->drawn = false;
	      }

	      if (attack.is_attacking) {
	        // play sound
	      }
	    }  	
    }   
  });
}

float timerTurret;

void TurretWalkingSystem::update(entityx::EntityManager &es,
                                 entityx::EventManager &events,
                                 entityx::TimeDelta dt) {
  entityx::ComponentHandle<SpriteAnimation> animation;
  entityx::ComponentHandle<Physics> physics;
  entityx::ComponentHandle<ParentLink> parent;
  entityx::ComponentHandle<TurretLegs> legs;

  std::string animToPlay;

  for (entityx::Entity e1 :
       es.entities_with_components(animation, parent, legs)) {
    if ((parent->owner.component<Physics>()->velocity.x < 0) ||
        (parent->owner.component<Physics>()->velocity.x > 0) ||
        (parent->owner.component<Physics>()->velocity.y > 0) ||
        (parent->owner.component<Physics>()->velocity.y < 0)) {
      animToPlay = "moving";
      // TODO: Cuando este el sonido listo descomentar
      if (timerTurret == 0.0) {
        Engine::GetInstance().Get<AudioManager>().PlaySound(
            "assets/media/fx/turret/default/mov.wav", false, 0.8f);
      }
    } else {
      animToPlay = "stand";
    }
    animation->Play(animToPlay);
  }
  timerTurret += dt;
  if (timerTurret >= 0.22) {
    timerTurret = 0.0;
  }
}

void ManuelethAnimationSystem::update(entityx::EntityManager &es,
                            entityx::EventManager &events,
                            entityx::TimeDelta dt) {
  entityx::ComponentHandle<Manueleth> manueleth;
  entityx::ComponentHandle<SpriteAnimation> animation;
  std::string animToPlay;

  for (entityx::Entity e0 : es.entities_with_components(
           manueleth, animation)) {
  	switch (manueleth->comportamiento) {
  		case (Manueleth::Comportamiento::NORMAL):
  			animToPlay = "attacking";
  		break;
  		case (Manueleth::Comportamiento::PUSH):
  			animToPlay = "pushing";
  		break;
  		case (Manueleth::Comportamiento::SPECIAL):
  			animToPlay = "talking";
  		break;
  	}
  	animation->Play(animToPlay); 
  }
}


void ManuelethIaSystem::update(entityx::EntityManager &es,
                            entityx::EventManager &events,
                            entityx::TimeDelta dt) {
  entityx::ComponentHandle<Player> p;
  entityx::ComponentHandle<Transform> t;

  glm::vec3 player_position;
  es.each<Player, Transform>(
      [&](entityx::Entity entity, Player &player, Transform &player_transform) {
        player_position = player_transform.GetWorldPosition();
   });
  glm::vec3 manueleth_position;
  es.each<Manueleth, Transform>(
      [&](entityx::Entity entity, Manueleth &manueleth, Transform &transform) {

      	manueleth.time_for_shooting += (dt * 1000.0f);

      	manueleth_position = transform.GetWorldPosition();
      	const float distancia = std::sqrt(
            std::pow(std::abs(player_position.x - manueleth_position.x), 2) +
            std::pow(std::abs(player_position.y - manueleth_position.y), 2));

      	if (distancia <= 45.0f) {
      		if (manueleth.hits >= 3) {
      			manueleth.comportamiento = Manueleth::Comportamiento::PUSH;
            Engine::GetInstance().Get<AudioManager>().PlaySound(
              "assets/media/fx/manueleth/default/shockwave.wav", false, 1);
      			glm::vec3 new_position(manueleth_position.x, manueleth_position.y - 160.0f , manueleth_position.z);

      			 for (entityx::Entity e0 : es.entities_with_components(
           			p, t)) {
      			 	t->SetLocalPosition(new_position);

      			 }
      			manueleth.hits = 0;
      		}
      	} else {
      		if (manueleth.time_for_shooting >= 1000.0f) {
      		manueleth.comportamiento = Manueleth::Comportamiento::NORMAL;

      		manueleth_position = transform.GetWorldPosition();

            glm::vec3 vector_player_manueleth(player_position.x - manueleth_position.x, player_position.y - manueleth_position.y, 0.0f);
	            glm::vec3 vector_manueleth_v(0.0f, 1.0f, 0.0f);
	           
	            float angle_rad = std::atan2(vector_player_manueleth.y - vector_manueleth_v.y, vector_player_manueleth.x - vector_manueleth_v.x);

	            glm::vec3 new_velocity(0.0f, 0.0f, 0.0f);
	            new_velocity = glm::normalize(player_position -
	                             transform.GetWorldPosition()) *
	              100.0f;
	            
	            EntityFactory::MakeEnemyProjectile(
	                es, manueleth_position, angle_rad, new_velocity, "manueleth");
	            manueleth.time_for_shooting = 0.0;

              Engine::GetInstance().Get<AudioManager>().PlaySound(
              "assets/media/fx/manueleth/default/attack.wav", false, 0.8f);
	      	}
      	}
   });
}

const float TurretIaSystem::turretSpeed = 10.0f;

void TurretIaSystem::update(entityx::EntityManager &es,
                            entityx::EventManager &events,
                            entityx::TimeDelta dt) {
  glm::vec3 player_position;
  es.each<Player, Transform>(
      [&](entityx::Entity entity, Player &player, Transform &player_transform) {
        player_position = player_transform.GetWorldPosition();
      });

  // Comprobamos la distancia a la que esta, si esta a menos de X distancia se
  // movera y disparara (disminuyendo asi la precision)
  glm::vec3 turret_position;
  es.each<Turret, Transform, Physics>(
      [&](entityx::Entity entity, Turret &turret, Transform &turret_transform,
          Physics &turret_physics) {
        turret_position = turret_transform.GetWorldPosition();
        const float distancia = std::sqrt(
            std::pow(std::abs(player_position.x - turret_position.x), 2) +
            std::pow(std::abs(player_position.y - turret_position.y), 2));
        turret.time_passed += (dt * 250.0f);

        if (distancia < 50.0f) {
          turret_physics.velocity =
              -1.0f *
              glm::normalize(player_position -
                             turret_transform.GetWorldPosition()) *
              turretSpeed;              
        } else {
	          turret_physics.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	          
	        if (turret.time_passed >= turret.frecuencia) {
	            Engine::GetInstance().Get<AudioManager>().PlaySound(
	                "assets/media/fx/turret/default/attack.wav", false, 1);

	        	glm::vec3 vector_player_turret(player_position.x - turret_position.x, player_position.y - turret_position.y, 0.0f);
	            glm::vec3 vector_turret_v(0.0f, 1.0f, 0.0f);
	           
	            float angle_rad = std::atan2(vector_player_turret.y - vector_turret_v.y, vector_player_turret.x - vector_turret_v.x);

	            glm::vec3 new_velocity(0.0f, 0.0f, 0.0f);
	            new_velocity = glm::normalize(player_position -
	                             turret_transform.GetWorldPosition()) *
	              100.0f;
	            
	            EntityFactory::MakeEnemyProjectile(
	                es, turret_position, angle_rad, new_velocity, "torreta");
	            turret.time_passed = 0.0;
	          }              
	        }    
      });
}

void TrapIaSystem::update(entityx::EntityManager &es,
                                             entityx::EventManager &events,
                                             entityx::TimeDelta dt) {
	glm::vec3 trap_position;
	es.each<Trap, Transform>(
      [&](entityx::Entity entity, Trap &trap, Transform &trap_transform) {
        trap_position = trap_transform.GetWorldPosition();
      

		trap.time_passed += (dt * 250.0f);

		if (trap.time_passed >= trap.frecuencia) {
			Engine::GetInstance().Get<AudioManager>().PlaySound(
	                "assets/media/fx/turret/default/attack.wav", false, 1);
			glm::vec3 new_velocity(0.0f, 0.0f, 0.0f);
			switch(trap.orientation) {
				case Trap::Orientation::UP :
					EntityFactory::MakeEnemyProjectile(
	                es, trap_position, 1.57, glm::vec3(0.0f, 100.0f, 0.0f), "trampa");
				break;
				case Trap::Orientation::DOWN :
					EntityFactory::MakeEnemyProjectile(
	                es, trap_position, -1.57, glm::vec3(0.0f, -100.0f, 0.0f), "trampa");
				break;
				case Trap::Orientation::LEFT :
					EntityFactory::MakeEnemyProjectile(
	                es, trap_position, -3.14, glm::vec3(-100.0f, 0.0f, 0.0f), "trampa");
				break;
				case Trap::Orientation::RIGHT :
					EntityFactory::MakeEnemyProjectile(
	                es, trap_position, 0.0, glm::vec3(100.0f, 0.0f, 0.0f), "trampa");
				break;
			}
			trap.time_passed = 0.0;
		}
	});

}

void EnemyProjectileAnimationSystem::update(entityx::EntityManager &es,
                                             entityx::EventManager &events,
                                             entityx::TimeDelta dt) {
  entityx::ComponentHandle<SpriteAnimation> animation;
  entityx::ComponentHandle<Physics> physics;
  entityx::ComponentHandle<EnemyProjectile> enemyProjectile;
  std::string animToPlay;

  for (entityx::Entity e1 :
       es.entities_with_components(animation, physics, enemyProjectile)) {
    animToPlay = "shooting";
    animation->Play(animToPlay);
  }
}

const float GhostIaSystem::kSpeed = 50.0f;
float timerGhost;

void GhostIaSystem::update(entityx::EntityManager &es,
                           entityx::EventManager &events,
                           entityx::TimeDelta dt) {
  glm::vec3 player_position;
  es.each<Player, Transform>(
      [&](entityx::Entity entity, Player &player, Transform &transform) {
        player_position = transform.GetWorldPosition();
      });

  glm::vec3 new_velocity(0.0f, 0.0f, 0.0f);
  es.each<Ghost, Transform, Physics>([&](entityx::Entity entity, Ghost &ghost,
                                         Transform &transform,
                                         Physics &physics) {
    ghost.time_passed += dt * 1000.0f;

    switch (ghost.comportamiento) {
      case Ghost::Comportamiento::DAMAGE_TOP:
        if (ghost.time_passed >= ghost.kHitDuration) {
          ghost.time_passed = 0.0;
          ghost.comportamiento = Ghost::Comportamiento::FOLLOW;
        } else {
          new_velocity.y = -1.0f;
          physics.velocity = glm::normalize(new_velocity) * kSpeed;
          if (timerGhost == 0.0) {
            Engine::GetInstance().Get<AudioManager>().PlaySound(
                "assets/media/fx/ghost/default/back.wav", false, 0.8f);
          }
        }
        break;
      case Ghost::Comportamiento::DAMAGE_BOTTOM:
        if (ghost.time_passed >= ghost.kHitDuration) {
          ghost.time_passed = 0.0;
          ghost.comportamiento = Ghost::Comportamiento::FOLLOW;
        } else {
          new_velocity.y = 1.0f;
          physics.velocity = glm::normalize(new_velocity) * kSpeed;
          if (timerGhost == 0.0) {
            Engine::GetInstance().Get<AudioManager>().PlaySound(
                "assets/media/fx/ghost/default/back.wav", false, 0.8f);
          }
        }
        break;
      case Ghost::Comportamiento::DAMAGE_LEFT:
        if (ghost.time_passed >= ghost.kHitDuration) {
          ghost.time_passed = 0.0;
          ghost.comportamiento = Ghost::Comportamiento::FOLLOW;
        } else {
          new_velocity.x = 1.0f;
          physics.velocity = glm::normalize(new_velocity) * kSpeed;
          if (timerGhost == 0.0) {
            Engine::GetInstance().Get<AudioManager>().PlaySound(
                "assets/media/fx/ghost/default/back.wav", false, 0.8f);
          }
        }
        break;
      case Ghost::Comportamiento::DAMAGE_RIGHT:
        if (ghost.time_passed >= ghost.kHitDuration) {
          ghost.time_passed = 0.0;
          ghost.comportamiento = Ghost::Comportamiento::FOLLOW;
        } else {
          new_velocity.x = -1.0f;
          physics.velocity = glm::normalize(new_velocity) * kSpeed;
          if (timerGhost == 0.0) {
            Engine::GetInstance().Get<AudioManager>().PlaySound(
                "assets/media/fx/ghost/default/back.wav", false, 0.8f);
          }
        }
        break;
      case Ghost::Comportamiento::FOLLOW:
        physics.velocity =
            glm::normalize(player_position - transform.GetWorldPosition()) *
            kSpeed;
        if (timerGhost == 0.0) {
          Engine::GetInstance().Get<AudioManager>().PlaySound(
              "assets/media/fx/ghost/default/mov.wav", false, 0.8f);
        }
        break;
    }
  });
  timerGhost += dt;
  if (timerGhost >= 1.4) {
    timerGhost = 0.0;
  }
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

    auto e0_orientation =
        e0_weapon->owner.component<KnightAttack>()->orientation;
    // e1 se desplaza hacia atras
    auto ghost = collision_copy.e1.component<Ghost>();

    switch (e0_orientation) {
      case KnightAttack::Orientation::UP:
        ghost->comportamiento = Ghost::Comportamiento::DAMAGE_BOTTOM;
        break;
      case KnightAttack::Orientation::DOWN:
        ghost->comportamiento = Ghost::Comportamiento::DAMAGE_TOP;
        break;
      case KnightAttack::Orientation::LEFT:
        ghost->comportamiento = Ghost::Comportamiento::DAMAGE_RIGHT;
        break;
      case KnightAttack::Orientation::RIGHT:
        ghost->comportamiento = Ghost::Comportamiento::DAMAGE_LEFT;
        break;
    }
    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/ghost/default/hit.wav", false, 0.7f);
    auto e1_color_animation = collision_copy.e1.component<ColorAnimation>();
    e1_color_animation->Play();
  } else if (e1_weapon && e1_weapon->drawn &&
             e1_weapon->owner.component<Player>() &&
             collision_copy.e0.component<Ghost>()) {
    auto e0_health = collision_copy.e0.component<Health>();
    e0_health->hp -= e1_weapon->damage;

    auto e1_orientation =
        e1_weapon->owner.component<KnightAttack>()->orientation;
    // e0 se desplaza hacia atras
    auto ghost = collision_copy.e0.component<Ghost>();

    switch (e1_orientation) {
      case KnightAttack::Orientation::UP:
        ghost->comportamiento = Ghost::Comportamiento::DAMAGE_BOTTOM;
        break;
      case KnightAttack::Orientation::DOWN:
        ghost->comportamiento = Ghost::Comportamiento::DAMAGE_TOP;
        break;
      case KnightAttack::Orientation::LEFT:
        ghost->comportamiento = Ghost::Comportamiento::DAMAGE_RIGHT;
        break;
      case KnightAttack::Orientation::RIGHT:
        ghost->comportamiento = Ghost::Comportamiento::DAMAGE_LEFT;
        break;
    }
    auto e1_color_animation = collision_copy.e1.component<ColorAnimation>();
    e1_color_animation->Play();
    // Torreta
  } else if (e0_weapon && e0_weapon->drawn &&
             e0_weapon->owner.component<Player>() &&
             collision_copy.e1.component<Turret>()) {
    auto e1_health = collision_copy.e1.component<Health>();
    e1_health->hp -= e0_weapon->damage;
    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/ghost/default/hit.wav", false, 0.7f);
    auto e1_color_animation = collision_copy.e1.component<ColorAnimation>();
    e1_color_animation->Play();
  } else if (e1_weapon && e1_weapon->drawn &&
             e1_weapon->owner.component<Player>() &&
             collision_copy.e0.component<Turret>()) {
    auto e0_health = collision_copy.e0.component<Health>();
    e0_health->hp -= e1_weapon->damage;

    auto e1_color_animation = collision_copy.e1.component<ColorAnimation>();
    e1_color_animation->Play();
    // Manueleth
  } else if (e0_weapon && e0_weapon->drawn &&
             e0_weapon->owner.component<Player>() &&
             collision_copy.e1.component<Manueleth>()) {
    auto e1_health = collision_copy.e1.component<Health>();
    e1_health->hp -= e0_weapon->damage;
    auto e1_manueleth = collision_copy.e1.component<Manueleth>();
    e1_manueleth->hits += 1;
    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/manueleth/default/hit.wav", false, 0.5f);
    auto e1_color_animation = collision_copy.e1.component<ColorAnimation>();
    e1_color_animation->Play();
  } else if (e1_weapon && e1_weapon->drawn &&
             e1_weapon->owner.component<Player>() &&
             collision_copy.e0.component<Manueleth>()) {
    auto e0_health = collision_copy.e0.component<Health>();
    e0_health->hp -= e1_weapon->damage;
	auto e0_manueleth = collision_copy.e0.component<Manueleth>();
    e0_manueleth->hits += 1;
    auto e1_color_animation = collision_copy.e1.component<ColorAnimation>();
    e1_color_animation->Play();
  }
}

void KnightAttackSystem::update(entityx::EntityManager &es,
                                entityx::EventManager &events,
                                entityx::TimeDelta dt) {}

void TurretAttackSystem::configure(entityx::EventManager &event_manager) {
  event_manager.subscribe<Collision>(*this);
}

void TurretAttackSystem::receive(const Collision &collision) {
  auto collision_copy = collision;
  if (!collision_copy.e0.valid() || !collision_copy.e1.valid()) {
    return;
  }
  auto e0_projectile = collision_copy.e0.component<EnemyProjectile>();
  auto e1_projectile = collision_copy.e1.component<EnemyProjectile>();

  if (e0_projectile && collision_copy.e1.component<Player>()) {
    auto e1_health = collision_copy.e1.component<Health>();
    e1_health->hp -= e0_projectile->damage;

    entityx::Entity proyectil = collision.e0;
    proyectil.destroy();

    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/gaunt/default/hit.wav", false, 1);
    auto e1_color_animation = collision_copy.e1.component<ColorAnimation>();
    e1_color_animation->Play();

  } else if (e1_projectile && collision_copy.e0.component<Player>()) {
    auto e0_health = collision_copy.e0.component<Health>();
    e0_health->hp -= e1_projectile->damage;

    entityx::Entity proyectil = collision.e1;
    proyectil.destroy();

    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/gaunt/default/hit.wav", false, 1);
    auto e0_color_animation = collision_copy.e0.component<ColorAnimation>();
    e0_color_animation->Play();
  } else {
    if (e1_projectile) {
      entityx::Entity proyectil = collision.e1;
      proyectil.destroy();
    } else if (e0_projectile) {
      entityx::Entity proyectil = collision.e0;
      proyectil.destroy();
    }
  }
}

void TurretAttackSystem::update(entityx::EntityManager &es,
                                entityx::EventManager &events,
                                entityx::TimeDelta dt) {}


void ShieldSystem::configure(entityx::EventManager &event_manager) {
  event_manager.subscribe<Collision>(*this);
}

// TODO: BAJADA DE ENERGIA AL COLISIONAR Y SUBIDA DE ENERGIA AL NO COLISIONAR
void ShieldSystem::receive(const Collision &collision) {
  auto collision_copy = collision;
  if (!collision_copy.e0.valid() || !collision_copy.e1.valid()) {
    return;
  }
  auto e0_projectile = collision_copy.e0.component<EnemyProjectile>();
  auto e1_projectile = collision_copy.e1.component<EnemyProjectile>();

  if (e0_projectile && collision_copy.e1.component<Shield>()) {
  	auto e1_player = collision_copy.e1.component<Shield>()->owner;
    auto e1_energy = e1_player.component<Energy>();
    
    if ((e1_energy->energy -= 10.0f) < 0.0f) {
    	e1_energy->energy = 0.0f;
    } else e1_energy->energy -= 10.0f;

    entityx::Entity proyectil = collision.e0;
    proyectil.destroy();

    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/gaunt/warrior/alt_hit.wav", false, 1); // Sonido de bloqueo
    /*auto e1_color_animation = collision_copy.e1.component<ColorAnimation>();
    e1_color_animation->Play();*/

  } else if (e1_projectile && collision_copy.e0.component<Shield>()) {
    auto e0_player = collision_copy.e0.component<Shield>()->owner;
    auto e0_energy = e0_player.component<Energy>();
    if ((e0_energy->energy -= 10.0f) < 0.0f) {
    	e0_energy->energy = 0.0f;
    } else e0_energy->energy -= 10.0f;

    entityx::Entity proyectil = collision.e1;
    proyectil.destroy();

    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/gaunt/warrior/alt_hit.wav", false, 1);	// Sonido de bloqueo
    /*auto e0_color_animation = collision_copy.e0.component<ColorAnimation>();
    e0_color_animation->Play();*/
  } 
}

float last_nrg = -1.0f;
void ShieldSystem::update(entityx::EntityManager &es,
                                entityx::EventManager &events,
                                entityx::TimeDelta dt) {
  es.each<Energy, Health>(
    [&](entityx::Entity Player, Energy &energy, Health &health){
      if(energy.energy != last_nrg && last_nrg != -1.0f){
        last_nrg = energy.energy;
        //Telling the UI that energy is changing
        events.emit<Energy>(energy);
      }
      else if(last_nrg == -1.0f) last_nrg = energy.init_nrg;
    });
}


float lastPlayerHP;

void HealthSystem::update(entityx::EntityManager &es,
                          entityx::EventManager &events,
                          entityx::TimeDelta dt) {
  es.each<Health>([&](entityx::Entity entity, Health &health) {
    if(entity.component<Player>()){
      if(lastPlayerHP != health.hp && health.hp != health.init_hp){
        events.emit<Health>(health);
        lastPlayerHP = health.hp;
      }
    }
    if (health.hp <= 0.0f) {
      Engine::GetInstance().Get<AudioManager>().PlaySound(health.death_fx,
                                                          false, 1);
      es.each<Legs, ParentLink>(
          [&](entityx::Entity entity_legs, Legs &legs, ParentLink &parent) {
            if (parent.owner == entity) {
              entity_legs.destroy();
            }
          });
      es.each<TurretLegs, ParentLink>([&](entityx::Entity entity_legs,
                                          TurretLegs &legs,
                                          ParentLink &parent) {
        if (parent.owner == entity) {
          entity_legs.destroy();
        }
      });

      es.each<GhostHitBox>(
          [&](entityx::Entity entity_hitbox, GhostHitBox &ghost_hitbox) {
            if (ghost_hitbox.owner == entity) {
              entity_hitbox.destroy();
            }
          });
      entity.destroy();
    }
  });
}

void ChestCollisionSystem::configure(entityx::EventManager &event_manager) {
	event_manager.subscribe<Collision>(*this);
}


void ChestCollisionSystem::receive(const engine::events::Collision &collision) {
  auto collision_copy = collision;
  if (!collision_copy.e0.valid() || !collision_copy.e1.valid()) {
    return;
  }
  auto e0_player = collision_copy.e0.component<Player>();
  auto e1_player = collision_copy.e1.component<Player>();

  if (e0_player && collision_copy.e1.component<Chest>()) {
  	auto chest = collision_copy.e1.component<Chest>();
  	if (chest->key == true) {
  		// Mensaje de que has encontrado la llave, actualizar GUI con la imagen de la llave
  		e0_player->key = true;
  		std::cerr << "has encontrado la llave" << std::endl;
  	} else {
  		// Mensaje de que la llave esta en otro cofre
  		std::cerr << "la llave esta en otro cofre" << std::endl;
  	}
  } else if (e1_player && collision_copy.e0.component<Chest>()) {
  	auto chest = collision_copy.e0.component<Chest>();
  	if (chest->key == true) {
  		// Mensaje de que has encontrado la llave, actualizar GUI con la imagen de la llave
  		e1_player->key = true;
  		std::cerr << "has encontrado la llave" << std::endl;
  	} else {
  		// Mensaje de que la llave esta en otro cofre
  		std::cerr << "la llave esta en otro cofre" << std::endl;
  	}
  }  
}


void ChestCollisionSystem::update(entityx::EntityManager &es,
                                entityx::EventManager &events,
                                entityx::TimeDelta dt) {}
