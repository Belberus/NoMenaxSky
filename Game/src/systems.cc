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
#include "entity_factory_2d.h"
#include "entity_factory_3d.h"

#include "events.h"

#include <iostream>
#include <fstream>
#include <string.h>
#include <iostream>

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
    } else if (physics->velocity.x < 0) {
      once = false;
      animToPlay = "moving_left";
    } else if (physics->velocity.y > 0) {
      once = false;
      animToPlay = "moving_top";
    } else if (physics->velocity.y < 0) {
      once = false;
      animToPlay = "moving_bottom";
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

void MasiatrixWalkingSystem::update(entityx::EntityManager &es,
                                 entityx::EventManager &events,
                                 entityx::TimeDelta dt) {
  entityx::ComponentHandle<SpriteAnimation> animation;
  entityx::ComponentHandle<Physics> physics;
  entityx::ComponentHandle<ParentLink> parent;
  entityx::ComponentHandle<MasiatrixLegs> legs;
  std::string animToPlay;

  for (entityx::Entity e1 :
       es.entities_with_components(animation, parent, legs)) {
    if ((parent->owner.component<Physics>()->velocity.x < 0) ||
        (parent->owner.component<Physics>()->velocity.x > 0) ||
        (parent->owner.component<Physics>()->velocity.y > 0) ||
        (parent->owner.component<Physics>()->velocity.y < 0)) {
      animToPlay = "walking";
    } else {
      animToPlay = "stand";
    }
    animation->Play(animToPlay);
  }
}



void WizardAnimationSystem::update(entityx::EntityManager &es,
                                   entityx::EventManager &events,
                                   entityx::TimeDelta dt) {
  entityx::ComponentHandle<SpriteAnimation> animation;
  entityx::ComponentHandle<Physics> physics;
  entityx::ComponentHandle<Wizard> wizard;
  entityx::ComponentHandle<Player> player;
  std::string animToPlay;

  entityx::Entity player_entity;
  for (auto e : es.entities_with_components<Player>()) {
    player_entity = e;
  }

  for (entityx::Entity e1 :
       es.entities_with_components(animation, physics, wizard, player)) {
  	if (physics->velocity.x > 0) {
	      animToPlay = "walking_right";
	      if (timer2 == 0.0) {
	        Engine::GetInstance().Get<AudioManager>().PlaySound(
	            "assets/media/fx/gaunt/default/mov.wav", false, 0.6f);
	      }
	    } else if (physics->velocity.x < 0) {
	      animToPlay = "walking_left";
	      if (timer2 == 0.0) {
	        Engine::GetInstance().Get<AudioManager>().PlaySound(
	            "assets/media/fx/gaunt/default/mov.wav", false, 0.6f);
	      }
	    } else if (physics->velocity.y > 0) {
	      animToPlay = "walking_top";
	      if (timer2 == 0.0) {
	        Engine::GetInstance().Get<AudioManager>().PlaySound(
	            "assets/media/fx/gaunt/default/mov.wav", false, 0.6f);
	      }
	    } else if (physics->velocity.y < 0) {
	      animToPlay = "walking_bottom";
	      if (timer2 == 0.0) {
	        Engine::GetInstance().Get<AudioManager>().PlaySound(
	            "assets/media/fx/gaunt/default/mov.wav", false, 0.6f);
	      }
	    } else if (player->orientation == Player::Orientation::UP) {
	      animToPlay = "moving_top";
	    } else if (player->orientation == Player::Orientation::DOWN) {
	      animToPlay = "moving_bottom";
	    } else if (player->orientation == Player::Orientation::LEFT) {
	      animToPlay = "moving_left";
	    } else if (player->orientation == Player::Orientation::RIGHT) {
	      animToPlay = "moving_right";
	    }
		animation->Play(animToPlay);
	  timer2 += dt;
	  if (timer2 >= 0.2) {
	    timer2 = 0.0;
	  }
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
          events.emit<CharSelect>();
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

PauseInputSystem::PauseInputSystem()
    :three_d(false), up_pressed_(false), down_pressed_(false), enter_pressed_(false) {
  Engine::GetInstance().Get<EventManager>().Subscribe<KeyPressed>(*this);
  Engine::GetInstance().Get<EventManager>().Subscribe<KeyReleased>(*this);
}

float timerEnter = 0.0f;

void PauseInputSystem::update(entityx::EntityManager &es,
                             entityx::EventManager &events,
                             entityx::TimeDelta dt) {
  entityx::ComponentHandle<Transform> transform;
  entityx::ComponentHandle<PauseOptions> arrow_menu;
  entityx::ComponentHandle<ThreeD> threed;
  for(auto e : es.entities_with_components(threed)){
    three_d = true;
  }

  int displacement = 70;
  if(three_d){
    displacement =  70;
  }
  for (entityx::Entity entity :
       es.entities_with_components(arrow_menu, transform)) {
    auto new_position = transform->GetLocalPosition();
    if (up_pressed_) {
      up_pressed_ = false;
      switch (arrow_menu->option) {
        case PauseOptions::Option::CONTINUAR:
          break;
        case PauseOptions::Option::FX:
          new_position.y += displacement;
          arrow_menu->option = PauseOptions::Option::CONTINUAR;
        break;
        case PauseOptions::Option::MUSIC:
          new_position.y += displacement;
          arrow_menu->option = PauseOptions::Option::FX;
        break;       
        case PauseOptions::Option::SALIR:
          new_position.y += displacement;
          arrow_menu->option = PauseOptions::Option::MUSIC;
        break;  
      }
    }
    if (down_pressed_) {
      down_pressed_ = false;
      switch (arrow_menu->option) {
        case PauseOptions::Option::CONTINUAR:
          new_position.y -= displacement;
          arrow_menu->option = PauseOptions::Option::FX;
          break;
        case PauseOptions::Option::FX:
          new_position.y -= displacement;
          arrow_menu->option = PauseOptions::Option::MUSIC;
          break;
        case PauseOptions::Option::MUSIC:
          new_position.y -= displacement;
          arrow_menu->option = PauseOptions::Option::SALIR;
          break;  
        case PauseOptions::Option::SALIR:
          break;
      }
    }
    if (enter_pressed_) {
      switch (arrow_menu->option) {
        case PauseOptions::Option::CONTINUAR:
          events.emit<BackToGame>();
          break;
        case PauseOptions::Option::FX:
            events.emit<MuteFx>();
          break;
        case PauseOptions::Option::MUSIC:
            events.emit<MuteMusic>();  
          break;  
        case PauseOptions::Option::SALIR:
            events.emit<BackToMainMenu>();
          break;
      }
      enter_pressed_ = false;
    }
    timerEnter += dt;
    if(timerEnter >= 0.5){
      timerEnter = 0;
    }
    transform->SetLocalPosition(new_position);
  }
}

void PauseInputSystem::receive(const KeyPressed &key_pressed) {
  if (key_pressed.key == GLFW_KEY_UP) {
    up_pressed_ = true;
  } else if (key_pressed.key == GLFW_KEY_DOWN) {
    down_pressed_ = true;
  } else if (key_pressed.key == GLFW_KEY_ENTER) {
    enter_pressed_ = true;
  }
}

void PauseInputSystem::receive(const KeyReleased &key_released) {
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
              opciones->musica == GameOptions::Musica::MUSIC_OFF) {
            new_position.x -= 150;
          }

          if (opciones->modo == GameOptions::Modo::THREE_D &&
              opciones->musica == GameOptions::Musica::MUSIC_ON) {
            new_position.x += 150;
          }

          arrow_options->option = ArrowOptions::Option::MODE;
          break;
        case ArrowOptions::Option::FX_VOL:
          new_position.y += 70;

          if (opciones->musica == GameOptions::Musica::MUSIC_ON &&
              opciones->efectos == GameOptions::Efectos::FX_OFF) {
            new_position.x -= 150;
          }

          if (opciones->musica == GameOptions::Musica::MUSIC_OFF &&
              opciones->efectos == GameOptions::Efectos::FX_ON) {
            new_position.x += 150;
          }

          arrow_options->option = ArrowOptions::Option::MUSIC_VOL;
          break;
        case ArrowOptions::Option::SALIR:
          new_position.y += 70;

          if (opciones->efectos == GameOptions::Efectos::FX_OFF) {
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
              opciones->musica == GameOptions::Musica::MUSIC_OFF) {
            new_position.x += 150;
          }

          if (opciones->modo == GameOptions::Modo::THREE_D &&
              opciones->musica == GameOptions::Musica::MUSIC_ON) {
            new_position.x -= 150;
          }

          arrow_options->option = ArrowOptions::Option::MUSIC_VOL;
          break;
        case ArrowOptions::Option::MUSIC_VOL:
          new_position.y -= 70;

          if (opciones->musica == GameOptions::Musica::MUSIC_ON &&
              opciones->efectos == GameOptions::Efectos::FX_OFF) {
            new_position.x += 150;
          }

          if (opciones->musica == GameOptions::Musica::MUSIC_OFF &&
              opciones->efectos == GameOptions::Efectos::FX_ON) {
            new_position.x -= 150;
          }

          arrow_options->option = ArrowOptions::Option::FX_VOL;
          break;
        case ArrowOptions::Option::FX_VOL:
          new_position.y -= 70;

          if (opciones->efectos == GameOptions::Efectos::FX_OFF) {
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


SelectionInputSystem::SelectionInputSystem() :
    selection_enter_pressed_(false), selection_right_pressed_(false), selection_left_pressed_(false) {

    Engine::GetInstance().Get<EventManager>().Subscribe<KeyPressed>(*this);
    Engine::GetInstance().Get<EventManager>().Subscribe<KeyReleased>(*this);
}

void SelectionInputSystem::update(entityx::EntityManager &es, 
        entityx::EventManager &events, entityx::TimeDelta dt) {

  entityx::ComponentHandle<Characters> character;
  entityx::ComponentHandle<Cursor> cursor;
  entityx::ComponentHandle<Transform> position;

  for (entityx::Entity e :
          es.entities_with_components(character, cursor, position)){
    auto new_position = position->GetLocalPosition();

    if (selection_right_pressed_){
       selection_right_pressed_ = false;
       switch (character->role) {
         case Characters::Role::KNIGHT:
            new_position.x += 315;
            character->role = Characters::Role::WIZARD;
            break;
          case Characters::Role::WIZARD:
            break;
       }
    }

    if (selection_left_pressed_){
       selection_left_pressed_ = false;
       switch (character->role) {
         case Characters::Role::KNIGHT:
            break;
          case Characters::Role::WIZARD:
            new_position.x -= 315;
            character->role = Characters::Role::KNIGHT;
            break;
       }
    }

    position->SetLocalPosition(new_position);

    if (selection_enter_pressed_){
      selection_enter_pressed_ = false;
      if (character->role == Characters::Role::KNIGHT) {
        events.emit<StartGame>("knight");
      } else events.emit<StartGame>("wizard");
    }
  }
}

void SelectionInputSystem::receive(const KeyPressed &key_pressed) {
  if (key_pressed.key == GLFW_KEY_ENTER) {
    selection_enter_pressed_ = true;
  } else if (key_pressed.key == GLFW_KEY_RIGHT) {
    selection_right_pressed_ = true;
  } else if (key_pressed.key == GLFW_KEY_LEFT) {
    selection_left_pressed_ = true;
  }
}

void SelectionInputSystem::receive(const KeyReleased &key_released) {
  if (key_released.key == GLFW_KEY_ENTER) {
    selection_enter_pressed_ = false;
  } else if (key_released.key == GLFW_KEY_RIGHT) {
    selection_right_pressed_ = false;
  } else if (key_released.key == GLFW_KEY_LEFT) {
    selection_left_pressed_ = false;
  }
}

DeathInputSystem::DeathInputSystem() :
    selection_enter_pressed_(false) {

    Engine::GetInstance().Get<EventManager>().Subscribe<KeyPressed>(*this);
    Engine::GetInstance().Get<EventManager>().Subscribe<KeyReleased>(*this);
}

void DeathInputSystem::update(entityx::EntityManager &es, 
  entityx::EventManager &events, entityx::TimeDelta dt){

  if (selection_enter_pressed_){
    selection_enter_pressed_ = false;
    events.emit<BackToMainMenu>();
  }
}

void DeathInputSystem::receive(const KeyPressed &key_pressed) {
  if (key_pressed.key == GLFW_KEY_ENTER) {
    selection_enter_pressed_ = true;
  } 
}

void DeathInputSystem::receive(const KeyReleased &key_released) {
  if (key_released.key == GLFW_KEY_ENTER) {
    selection_enter_pressed_ = false;
  } 
}

TextInputSystem::TextInputSystem() :
    selection_enter_pressed_(false) {

    Engine::GetInstance().Get<EventManager>().Subscribe<KeyPressed>(*this);
    Engine::GetInstance().Get<EventManager>().Subscribe<KeyReleased>(*this);
}

void TextInputSystem::update(entityx::EntityManager &es, 
  entityx::EventManager &events, entityx::TimeDelta dt){

  if (selection_enter_pressed_){
    selection_enter_pressed_ = false;
    events.emit<BackToGame>();
  }
}

void TextInputSystem::receive(const KeyPressed &key_pressed) {
  if (key_pressed.key == GLFW_KEY_ENTER) {
    selection_enter_pressed_ = true;
  } 
}

void TextInputSystem::receive(const KeyReleased &key_released) {
  if (key_released.key == GLFW_KEY_ENTER) {
    selection_enter_pressed_ = false;
  } 
}

const float PlayerInputSystem::kKnightSpeed = 150.0f;

const float PlayerInputSystem::kKnightThreeDSpeed = 40.0f;

const float PlayerInputSystem::kWizardSpeed = 200.0f;

const float PlayerInputSystem::kWizardThreeDSpeed = 60.0f;

const float PlayerInputSystem::kAttackDuration = 250.0f;

const float PlayerInputSystem::kMagicAttackDuration = 400.0f;

const float PlayerInputSystem::kAltAttackDuration = 1000.0f;

bool oncee = true;

PlayerInputSystem::PlayerInputSystem()
    : three_d(false), knight_threed_speed(kKnightThreeDSpeed), wizard_threed_speed(kWizardThreeDSpeed), knight_speed(kKnightSpeed), wizard_speed(kWizardSpeed), time_passed_since_last_attack_(kAttackDuration),time_passed_since_last_magic_attack_(kMagicAttackDuration), time_passed_since_last_alt_attack_(kAltAttackDuration), paused_(false){
  keys_.emplace(GLFW_KEY_W, false);
  keys_.emplace(GLFW_KEY_S, false);
  keys_.emplace(GLFW_KEY_A, false);
  keys_.emplace(GLFW_KEY_D, false);
  keys_.emplace(GLFW_KEY_UP, false);
  keys_.emplace(GLFW_KEY_DOWN, false);
  keys_.emplace(GLFW_KEY_LEFT, false);
  keys_.emplace(GLFW_KEY_RIGHT, false);
  keys_.emplace(GLFW_KEY_SPACE, false);
  keys_.emplace(GLFW_KEY_ESCAPE, false);
  keys_.emplace(GLFW_KEY_O, false); // Mantener para acceder a los shortcuts (es una o)
  keys_.emplace(GLFW_KEY_1, false); // Iniciar nivel 1
  keys_.emplace(GLFW_KEY_2, false); // Iniciar nivel 2
  keys_.emplace(GLFW_KEY_3, false); // Iniciar nivel 3
  keys_.emplace(GLFW_KEY_4, false); // Da llave al jugador
  keys_.emplace(GLFW_KEY_5, false); // Activa ambas palancas
  keys_.emplace(GLFW_KEY_6, false); // Restaura la vida
  keys_.emplace(GLFW_KEY_7, false); // Elimina todos lo enemigos (fantasmas, torretas y lanceros) cargados
  Engine::GetInstance().Get<EventManager>().Subscribe<KeyPressed>(*this);
  Engine::GetInstance().Get<EventManager>().Subscribe<KeyReleased>(*this);
  Engine::GetInstance().Get<EventManager>().Subscribe<BackToGame>(*this); 
  Engine::GetInstance().Get<EventManager>().Subscribe<SetThreeD>(*this); 
  oncee = true;
}

bool PlayerInputSystem::is_paused(){
  return paused_;
}

void PlayerInputSystem::set_paused(bool paused){
  paused_ = paused;
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

void PlayerInputSystem::receive(const SetThreeD &setThreeD){
  three_d = setThreeD.three_d;
}

void PlayerInputSystem::receive(const BackToGame &resumeGame){
  set_paused(false);
}

float timerEnergy = 0.0f;
void PlayerInputSystem::update(entityx::EntityManager &es,
                               entityx::EventManager &events,
                               entityx::TimeDelta dt) {
  entityx::Entity player_entity;
  for (auto e : es.entities_with_components<Player>()) {
    player_entity = e;
  }
  
  if (player_entity.has_component<KnightAttack>()) {
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

	  if(keys_[GLFW_KEY_ESCAPE] && !is_paused()) {
	    events.subscribe<BackToGame>(*this);
	    set_paused(true);
	    events.emit<PauseMenuEvent>();
	  }
	  // if(keys_[GLFW_KEY_ESCAPE] && keys_[GLFW_KEY_SPACE] && pausedM){
	  //   pausedM = false;
	  //   events.emit<BackToGame>();
	  // }

	  if(!is_paused()){
	    es.each<Player, Physics, KnightAttack, Health, Transform>([&](entityx::Entity entity,
	                                               Player &player, Physics &physics,
	                                               KnightAttack &attack, Health &health, Transform &t) {
        // ShortCuts
        if (keys_[GLFW_KEY_O]) {
          if (keys_[GLFW_KEY_1]) {
          events.emit<StartLevel1>();
          }
          if (keys_[GLFW_KEY_2]) {
            events.emit<StartLevel2>();
          }
          if (keys_[GLFW_KEY_3]) {
            events.emit<StartLevel3>();
          } 
          if (keys_[GLFW_KEY_4]) {
            player.key = true;
          }
          if (keys_[GLFW_KEY_5]) {
            player.levers_activated = 2;
          }
          if (keys_[GLFW_KEY_6]) {
            health.hp = health.init_hp;
          }
          if (keys_[GLFW_KEY_7]) {
            es.each<Ghost, Health>([&](entityx::Entity entity,
                                Ghost &ghost, Health &health) {
              health.hp = 0.0f;
            });
            es.each<Turret, Health>([&](entityx::Entity entity,
                                Turret &turret, Health &health) {
              health.hp = 0.0f;
            });
            es.each<Lancer, Health>([&](entityx::Entity entity,
                                Lancer &lancer, Health &health) {
              health.hp = 0.0f;
            });
          }
        }
        
        // Normal keys
	      glm::vec3 new_velocity(0.0f, 0.0f, 0.0f);
	      if (keys_[GLFW_KEY_W]) {
          if(entity.component<ThreeD>()){
            glm::quat rot;
            switch(player.orientation){
              case Player::Orientation::DOWN:
                rot = glm::rotate(rot, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                break;
              case Player::Orientation::LEFT:
                rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                break;
              case Player::Orientation::RIGHT:
                rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
                break;
              default:
                break;
            }
            t.SetLocalOrientation(rot);
          }          
	        player.orientation = Player::Orientation::UP;
	        new_velocity.y += 1.0f;
	      }
	      if (keys_[GLFW_KEY_S]) {
          if(entity.component<ThreeD>()){
            glm::quat rot;
            switch(player.orientation){
              case Player::Orientation::DOWN:
                rot = glm::rotate(rot, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                break;
              case Player::Orientation::LEFT:
                rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                break;
              case Player::Orientation::RIGHT:
                rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
                break;
              default:
                break;
            }
            t.SetLocalOrientation(rot);
          }
	        player.orientation = Player::Orientation::DOWN;
	        new_velocity.y += -1.0f;
	      }
	      if (keys_[GLFW_KEY_A]) {
          if(entity.component<ThreeD>()){
            glm::quat rot;
            switch(player.orientation){
              case Player::Orientation::DOWN:
                rot = glm::rotate(rot, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                break;
              case Player::Orientation::LEFT:
                rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                break;
              case Player::Orientation::RIGHT:
                rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
                break;
              default:
                break;
            }
            t.SetLocalOrientation(rot);
          }
	        player.orientation = Player::Orientation::LEFT;
	        new_velocity.x += -1.0f;
	      }
	      if (keys_[GLFW_KEY_D]) {
          if(entity.component<ThreeD>()){
            glm::quat rot;
            switch(player.orientation){
              case Player::Orientation::DOWN:
                rot = glm::rotate(rot, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                break;
              case Player::Orientation::LEFT:
                rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                break;
              case Player::Orientation::RIGHT:
                rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
                break;
              default:
                break;
            }
            t.SetLocalOrientation(rot);
          }
	        player.orientation = Player::Orientation::RIGHT;
	        new_velocity.x += 1.0f;
	      }
	      if (new_velocity != glm::vec3(0.0f, 0.0f, 0.0f)) {
          if(entity.component<ThreeD>()){
            new_velocity = glm::normalize(new_velocity) * knight_threed_speed;
          }
          else{
            new_velocity = glm::normalize(new_velocity) * knight_speed;
          }
          Engine::GetInstance().Get<AudioManager>().PlaySound(
            "assets/media/fx/gaunt/default/mov.wav", false, 0.6f);
	        // play sound
	      }

	      physics.velocity = new_velocity;
	      shield_info->time_passed += (dt * 1000.0f);
	      time_passed_since_last_attack_ += (dt * 1000.0f);
	      if (keys_[GLFW_KEY_SPACE] && ((shield_info->owner.component<Energy>()->energy > 0.0f))) {
	      	if (keys_[GLFW_KEY_UP]) {
              if(entity.component<ThreeD>()){
                glm::quat rot;
                switch(shield_info->orientation){
                  case Shield::Orientation::DOWN:
                    rot = glm::rotate(rot, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                    break;
                  case Shield::Orientation::LEFT:
                    rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                    break;
                  case Shield::Orientation::RIGHT:
                    rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
                    break;
                  default:
                    break;
                }
                t.SetLocalOrientation(rot);
              }
	      	    shield_info->active = true;
	  	        shield_info->orientation = Shield::Orientation::UP;
              if(entity.component<ThreeD>()){
                shield_transform->SetLocalPosition(glm::vec3(0.0f, 3.0f, 0.0f));
                shield_collider->half_size  = glm::vec2(2.5f, 1.0f);
              }else{
                shield_transform->SetLocalPosition(glm::vec3(0.0f, 9.0f, 0.0f));
                shield_collider->half_size  = glm::vec2(6.0f, 3.0f);
              }
	  	        
	  	      } else if (keys_[GLFW_KEY_DOWN]) {
              if(entity.component<ThreeD>()){
                glm::quat rot;
                switch(shield_info->orientation){
                  case Shield::Orientation::DOWN:
                    rot = glm::rotate(rot, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                    break;
                  case Shield::Orientation::LEFT:
                    rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                    break;
                  case Shield::Orientation::RIGHT:
                    rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
                    break;
                  default:
                    break;
                }
                t.SetLocalOrientation(rot);
              }
	  	      	shield_info->active = true;
	  	        shield_info->orientation = Shield::Orientation::DOWN;
	  	        if(entity.component<ThreeD>()){
                shield_transform->SetLocalPosition(glm::vec3(0.0f, -3.0f, 0.0f));
                shield_collider->half_size  = glm::vec2(2.5f, 1.0f);
              }else{
                shield_transform->SetLocalPosition(glm::vec3(0.0f, -9.0f, 0.0f));
                shield_collider->half_size  = glm::vec2(6.0f, 3.0f);
              }
	  	      } else if (keys_[GLFW_KEY_LEFT]) {
              if(entity.component<ThreeD>()){
                glm::quat rot;
                switch(shield_info->orientation){
                  case Shield::Orientation::DOWN:
                    rot = glm::rotate(rot, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                    break;
                  case Shield::Orientation::LEFT:
                    rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                    break;
                  case Shield::Orientation::RIGHT:
                    rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
                    break;
                  default:
                    break;
                }
                t.SetLocalOrientation(rot);
              }
	  	      	shield_info->active = true;
	  	        shield_info->orientation = Shield::Orientation::LEFT;
	  	        if(entity.component<ThreeD>()){
                shield_transform->SetLocalPosition(glm::vec3(-3.0f, 0.0f, 0.0f));
                shield_collider->half_size  = glm::vec2(1.0f, 2.5f);
              }else{
                shield_transform->SetLocalPosition(glm::vec3(-9.0f, 0.0f, 0.0f));
                shield_collider->half_size  = glm::vec2(3.0f, 6.0f);
              }
	  	      } else if (keys_[GLFW_KEY_RIGHT]) {
              if(entity.component<ThreeD>()){
                glm::quat rot;
                switch(shield_info->orientation){
                  case Shield::Orientation::DOWN:
                    rot = glm::rotate(rot, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                    break;
                  case Shield::Orientation::LEFT:
                    rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                    break;
                  case Shield::Orientation::RIGHT:
                    rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
                    break;
                  default:
                    break;
                }
                t.SetLocalOrientation(rot);
              }
	  	      	shield_info->active = true;
	  	        shield_info->orientation = Shield::Orientation::RIGHT;
	  	        if(entity.component<ThreeD>()){
                shield_transform->SetLocalPosition(glm::vec3(3.0f, 0.0f, 0.0f));
                shield_collider->half_size  = glm::vec2(1.0f, 2.5f);
              }else{
                shield_transform->SetLocalPosition(glm::vec3(9.0f, 0.0f, 0.0f));
                shield_collider->half_size  = glm::vec2(3.0f, 6.0f);
              }
	  	      } else {
	  	        shield_info->active = false;
	  	        weapon_info->drawn = false;
	  	        attack.is_attacking = false;
	  	      }
	      } 
	      else {
	        if(shield_info->owner.component<Energy>()->energy <= 0.0f){
	          Engine::GetInstance().Get<AudioManager>().PlaySound(
	          "assets/media/fx/gaunt/default/low_nrg.wav", false, 0.5f);
	        }
	      	if(shield_info->time_passed >= 2000.0f) {
	      		shield_info->time_passed = 0.0f;
	      		float actual_energy = (shield_info->owner).component<Energy>()->energy;
	      		if ((actual_energy + 20.0f) >= 100.0f) {
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
	  	      	if (keys_[GLFW_KEY_SPACE] && ((shield_info->owner.component<Energy>()->energy > 0.0f))) {
	  	      		attack.is_attacking = false;
	  	        	weapon_info->drawn = false;
	  	        	shield_info->active = true;
	  	        	shield_info->orientation = Shield::Orientation::UP;
	  		        shield_transform->SetLocalPosition(glm::vec3(0.0f, 3.0f, 0.0f));
	  		        shield_collider->half_size  = glm::vec2(2.5f, 1.0f);
	  	      	} else {
                  if(entity.component<ThreeD>()){
                    glm::quat rot;
                    switch(attack.orientation){
                      case KnightAttack::Orientation::DOWN:
                        rot = glm::rotate(rot, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                        break;
                      case KnightAttack::Orientation::LEFT:
                        rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                        break;
                      case KnightAttack::Orientation::RIGHT:
                        rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
                        break;
                      default:
                        break;
                    }
                  t.SetLocalOrientation(rot);
                  } 
	  	      		attack.orientation = KnightAttack::Orientation::UP;
                if(entity.component<ThreeD>()){
                  weapon_collider->half_size = glm::vec2(2.0f, 2.0f);
                  weapon_transform->SetLocalPosition(glm::vec3(0.0f, 3.0f, 0.0f));
                }
                else{
                  weapon_collider->half_size = glm::vec2(6.0f, 6.0f);
                  weapon_transform->SetLocalPosition(glm::vec3(0.0f, 9.0f, 0.0f));
                }
	  	      	}       
	  	      } else if (keys_[GLFW_KEY_DOWN]) {
	  	      	if (keys_[GLFW_KEY_SPACE] && ((shield_info->owner.component<Energy>()->energy > 0.0f))) {
	  	      		attack.is_attacking = false;
	  	        	weapon_info->drawn = false;
	  	        	shield_info->active = true;
	  	        	shield_info->orientation = Shield::Orientation::DOWN;
	  	        	shield_transform->SetLocalPosition(glm::vec3(0.0f, -3.0f, 0.0f));
	  	        	shield_collider->half_size  = glm::vec2(2.5f, 1.0f);
	  	      	} else {
                  if(entity.component<ThreeD>()){
                    glm::quat rot;
                    switch(attack.orientation){
                      case KnightAttack::Orientation::DOWN:
                        rot = glm::rotate(rot, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                        break;
                      case KnightAttack::Orientation::LEFT:
                        rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                        break;
                      case KnightAttack::Orientation::RIGHT:
                        rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
                        break;
                      default:
                        break;
                  }
                  t.SetLocalOrientation(rot);
                }
	  	      		attack.orientation = KnightAttack::Orientation::DOWN;
	  		        if(entity.component<ThreeD>()){
                  weapon_collider->half_size = glm::vec2(2.0f, 2.0f);
                  weapon_transform->SetLocalPosition(glm::vec3(0.0f, -3.0f, 0.0f));
                }
                else{
                  weapon_collider->half_size = glm::vec2(6.0f, 6.0f);
                  weapon_transform->SetLocalPosition(glm::vec3(0.0f, -9.0f, 0.0f));
                }
	  	      	}  
	  	      } else if (keys_[GLFW_KEY_LEFT]) {
	  	      	if (keys_[GLFW_KEY_SPACE] && ((shield_info->owner.component<Energy>()->energy > 0.0f))) {
	  	      		attack.is_attacking = false;
	  	        	weapon_info->drawn = false;
	  	        	shield_info->active = true;
	  	        	shield_info->orientation = Shield::Orientation::LEFT;
	  	        	shield_transform->SetLocalPosition(glm::vec3(-3.0f, 0.0f, 0.0f));
	  	        	shield_collider->half_size  = glm::vec2(1.0f, 2.5f);
	  	      	} else {
                  if(entity.component<ThreeD>()){
                    glm::quat rot;
                    switch(attack.orientation){
                      case KnightAttack::Orientation::DOWN:
                        rot = glm::rotate(rot, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                        break;
                      case KnightAttack::Orientation::LEFT:
                        rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                        break;
                      case KnightAttack::Orientation::RIGHT:
                        rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
                        break;
                      default:
                        break;
                  }
                  t.SetLocalOrientation(rot);
                }
	  	      		attack.orientation = KnightAttack::Orientation::LEFT;
	  		        if(entity.component<ThreeD>()){
                  weapon_collider->half_size = glm::vec2(2.0f, 2.0f);
                  weapon_transform->SetLocalPosition(glm::vec3(-3.0f, 0.0f, 0.0f));
                }
                else{
                  weapon_collider->half_size = glm::vec2(6.0f, 6.0f);
                  weapon_transform->SetLocalPosition(glm::vec3(-9.0f, 0.0f, 0.0f));
                }
	  	      	}   
	  	      } else if (keys_[GLFW_KEY_RIGHT]) {
	  	      	if (keys_[GLFW_KEY_SPACE] && ((shield_info->owner.component<Energy>()->energy > 0.0f))) {
	  	      		attack.is_attacking = false;
	  	        	weapon_info->drawn = false;
	  	        	shield_info->active = true;
	  	        	shield_info->orientation = Shield::Orientation::RIGHT;
	  	        	shield_transform->SetLocalPosition(glm::vec3(3.0f, 0.0f, 0.0f));
	  	        	shield_collider->half_size  = glm::vec2(1.0f, 2.5f);
	  	      	} else {
                if(entity.component<ThreeD>()){
                  glm::quat rot;
                    switch(attack.orientation){
                      case KnightAttack::Orientation::DOWN:
                        rot = glm::rotate(rot, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                        break;
                      case KnightAttack::Orientation::LEFT:
                        rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                        break;
                      case KnightAttack::Orientation::RIGHT:
                        rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
                        break;
                      default:
                        break;
                  }
                  t.SetLocalOrientation(rot);
                }
	  	      		attack.orientation = KnightAttack::Orientation::RIGHT;
	  		        if(entity.component<ThreeD>()){
                  weapon_collider->half_size = glm::vec2(2.0f, 2.0f);
                  weapon_transform->SetLocalPosition(glm::vec3(3.0f, 0.0f, 0.0f));
                }
                else{
                  weapon_collider->half_size = glm::vec2(6.0f, 6.0f);
                  weapon_transform->SetLocalPosition(glm::vec3(9.0f, 0.0f, 0.0f));
                }
	  	      	}	        
	  	      } else {
	  	        attack.is_attacking = false;
	  	        weapon_info->drawn = false;
	  	      }
	  	      if (attack.is_attacking) {
	  	        // play sound
              Engine::GetInstance().Get<AudioManager>().PlaySound(
                  "assets/media/fx/gaunt/warrior/attack.wav", false, 0.8f);
              Engine::GetInstance().Get<AudioManager>().PlaySound(
                  "assets/media/fx/gaunt/default/attack_2.wav", false, 0.6f);
	  	      } 
	  	    }  	
	      }   
	    });
	  }
	} else if (player_entity.has_component<Wizard>()){
		if(keys_[GLFW_KEY_ESCAPE] && !is_paused()) {
	    events.subscribe<BackToGame>(*this);
	    set_paused(true);
	    events.emit<PauseMenuEvent>();
	  }
	  // if(keys_[GLFW_KEY_ESCAPE] && keys_[GLFW_KEY_SPACE] && pausedM){
	  //   pausedM = false;
	  //   events.emit<BackToGame>();
	  // }

	  if(!is_paused()){
	    es.each<Player, Physics, Wizard, Health, Transform>([&](entityx::Entity entity,
	                                               Player &player, Physics &physics,
	                                               Wizard &wizard, Health &health, Transform &t) {
        // ShortCuts
        if (keys_[GLFW_KEY_O]) {
          if (keys_[GLFW_KEY_1]) {
          events.emit<StartLevel1>();
          }
          if (keys_[GLFW_KEY_2]) {
            events.emit<StartLevel2>();
          }
          if (keys_[GLFW_KEY_3]) {
            events.emit<StartLevel3>();
          } 
          if (keys_[GLFW_KEY_4]) {
            player.key = true;
          }
          if (keys_[GLFW_KEY_5]) {
            player.levers_activated = 2;
          }
          if (keys_[GLFW_KEY_6]) {
            health.hp = health.init_hp;
          }
          if (keys_[GLFW_KEY_7]) {
            es.each<Ghost, Health>([&](entityx::Entity entity,
                                Ghost &ghost, Health &health) {
              health.hp = 0.0f;
            });
            es.each<Turret, Health>([&](entityx::Entity entity,
                                Turret &turret, Health &health) {
              health.hp = 0.0f;
            });
            es.each<Lancer, Health>([&](entityx::Entity entity,
                                Lancer &lancer, Health &health) {
              health.hp = 0.0f;
            });
          }
        }


        // Normal keys
	      glm::vec3 new_velocity(0.0f, 0.0f, 0.0f);
	      if (keys_[GLFW_KEY_W]) {
          if(entity.component<ThreeD>()){
            glm::quat rot;
            switch(player.orientation){
              case Player::Orientation::DOWN:
                rot = glm::rotate(rot, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                break;
              case Player::Orientation::LEFT:
                rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                break;
              case Player::Orientation::RIGHT:
                rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
                break;
              default:
                break;
            }
            t.SetLocalOrientation(rot);
          }
	        player.orientation = Player::Orientation::UP;
	        new_velocity.y += 1.0f;
	      }
	      if (keys_[GLFW_KEY_S]) {
          if(entity.component<ThreeD>()){
            glm::quat rot;
            switch(player.orientation){
              case Player::Orientation::DOWN:
                rot = glm::rotate(rot, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                break;
              case Player::Orientation::LEFT:
                rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                break;
              case Player::Orientation::RIGHT:
                rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
                break;
              default:
                break;
            }
            t.SetLocalOrientation(rot);
          }
	        player.orientation = Player::Orientation::DOWN;
	        new_velocity.y += -1.0f;
	      }
	      if (keys_[GLFW_KEY_A]) {
          if(entity.component<ThreeD>()){
            glm::quat rot;
            switch(player.orientation){
              case Player::Orientation::DOWN:
                rot = glm::rotate(rot, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                break;
              case Player::Orientation::LEFT:
                rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                break;
              case Player::Orientation::RIGHT:
                rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
                break;
              default:
                break;
            }
            t.SetLocalOrientation(rot);
          }
	        player.orientation = Player::Orientation::LEFT;
	        new_velocity.x += -1.0f;
	      }
	      if (keys_[GLFW_KEY_D]) {
          if(entity.component<ThreeD>()){
            glm::quat rot;
            switch(player.orientation){
              case Player::Orientation::DOWN:
                rot = glm::rotate(rot, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                break;
              case Player::Orientation::LEFT:
                rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                break;
              case Player::Orientation::RIGHT:
                rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
                break;
              default:
                break;
            }
            t.SetLocalOrientation(rot);
          }
	        player.orientation = Player::Orientation::RIGHT;
	        new_velocity.x += 1.0f;
	      }
	      if (new_velocity != glm::vec3(0.0f, 0.0f, 0.0f)) {
          if(entity.component<ThreeD>()){
            new_velocity = glm::normalize(new_velocity) * wizard_threed_speed;
          }
          else{
            new_velocity = glm::normalize(new_velocity) * wizard_speed;
          }
	        // play sound
          Engine::GetInstance().Get<AudioManager>().PlaySound(
            "assets/media/fx/gaunt/default/mov.wav", false, 0.6f);
	      }

	      physics.velocity = new_velocity;
	      time_passed_since_last_magic_attack_ += (dt * 1000.0f);
        time_passed_since_last_alt_attack_ += (dt *1000.0f);

	      glm::vec3 player_position = player_entity.component<Transform>()->GetWorldPosition();

	      float actual_energy = player_entity.component<Energy>()->energy; 
    

	      if (time_passed_since_last_alt_attack_ >= PlayerInputSystem::kAltAttackDuration) {
          //First, Alt attack
          //Alt up
          if(keys_[GLFW_KEY_SPACE] && 
            keys_[GLFW_KEY_UP] && 
            !keys_[GLFW_KEY_DOWN] &&
            !keys_[GLFW_KEY_RIGHT] &&
            !keys_[GLFW_KEY_LEFT] &&
            (actual_energy > 0.0f)){

            if(entity.component<ThreeD>()){
              glm::quat rot;
              switch(player.orientation){
                case Player::Orientation::DOWN:
                  rot = glm::rotate(rot, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                  break;
                case Player::Orientation::LEFT:
                  rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                  break;
                case Player::Orientation::RIGHT:
                  rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
                  break;
                default:
                  break;
              }
              t.SetLocalOrientation(rot);
            }

            player.orientation = Player::Orientation::UP;
            time_passed_since_last_alt_attack_ = 0.0f;
            wizard.is_attacking = true;
            if(!entity.component<ThreeD>()){
              EntityFactory2D().MakeWizardProjectile(es, player_position, 1.57,glm::vec3(0.0f, 100.0f, 0.0f), "special");
            }
            else{
              EntityFactory3D().MakeWizardProjectile(es, player_position, 1.57,glm::vec3(0.0f, 100.0f, 0.0f), "special");
            }            
            Engine::GetInstance().Get<AudioManager>().PlaySound(
                "assets/media/fx/gaunt/mage/alt.wav", false, 0.5f);
            Engine::GetInstance().Get<AudioManager>().PlaySound(
                  "assets/media/fx/gaunt/default/attack_2.wav", false, 0.6f);
            if((actual_energy > 0.0) && (actual_energy - 30.0f) <= 0.0f){
              player_entity.component<Energy>()->energy = 0.0f;
              Engine::GetInstance().Get<AudioManager>().PlaySound(
                "assets/media/fx/gaunt/default/low_nrg.wav", false, 0.5f);
            }
            else player_entity.component<Energy>()->energy -= 30.0f;
          }
          //Alt Down
          else if(keys_[GLFW_KEY_SPACE] && 
            !keys_[GLFW_KEY_UP] && 
            keys_[GLFW_KEY_DOWN] &&
            !keys_[GLFW_KEY_RIGHT] &&
            !keys_[GLFW_KEY_LEFT] &&
            (actual_energy > 0.0f)){

            if(entity.component<ThreeD>()){
              glm::quat rot;
              switch(player.orientation){
                case Player::Orientation::UP:
                  rot = glm::rotate(rot, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                  break;
                case Player::Orientation::LEFT:
                  rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
                  break;
                case Player::Orientation::RIGHT:
                  rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                  break;
                default:
                  break;
              }
              t.SetLocalOrientation(rot);
            }

            player.orientation = Player::Orientation::DOWN;
            time_passed_since_last_alt_attack_ = 0.0f;
            wizard.is_attacking = true;
            if(!entity.component<ThreeD>()){
              EntityFactory2D().MakeWizardProjectile(es, player_position, -1.57,glm::vec3(0.0f, -100.0f, 0.0f), "special");
            }
            else{
              EntityFactory3D().MakeWizardProjectile(es, player_position, -1.57,glm::vec3(0.0f, -100.0f, 0.0f), "special");
            }            
                        Engine::GetInstance().Get<AudioManager>().PlaySound(
                "assets/media/fx/gaunt/mage/alt.wav", false, 0.5f);
            Engine::GetInstance().Get<AudioManager>().PlaySound(
                  "assets/media/fx/gaunt/default/attack_2.wav", false, 0.6f);
            if((actual_energy > 0.0) && (actual_energy - 30.0f) <= 0.0f){
              player_entity.component<Energy>()->energy = 0.0f;
              Engine::GetInstance().Get<AudioManager>().PlaySound(
                "assets/media/fx/gaunt/default/low_nrg.wav", false, 0.5f);
            }
            else player_entity.component<Energy>()->energy -= 30.0f;
          }
          //Alt Right
          else if(keys_[GLFW_KEY_SPACE] && 
            !keys_[GLFW_KEY_UP] && 
            !keys_[GLFW_KEY_DOWN] &&
            keys_[GLFW_KEY_RIGHT] &&
            !keys_[GLFW_KEY_LEFT] &&
            (actual_energy > 0.0f)){

            if(entity.component<ThreeD>()){
              glm::quat rot;
              switch(player.orientation){
                case Player::Orientation::DOWN:
                  rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                  break;
                case Player::Orientation::LEFT:
                  rot = glm::rotate(rot, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                  break;
                case Player::Orientation::UP:
                  rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
                  break;
                default:
                  break;
              }
              t.SetLocalOrientation(rot);
            }

            player.orientation = Player::Orientation::RIGHT;
            time_passed_since_last_alt_attack_ = 0.0f;
            wizard.is_attacking = true;
            if(!entity.component<ThreeD>()){
              EntityFactory2D().MakeWizardProjectile(es, player_position, 0.0,glm::vec3(100.0f, 0.0f, 0.0f), "special");
            }
            else{
              EntityFactory3D().MakeWizardProjectile(es, player_position, 0.0,glm::vec3(100.0f, 0.0f, 0.0f), "special");
            }            
                        Engine::GetInstance().Get<AudioManager>().PlaySound(
                "assets/media/fx/gaunt/mage/alt.wav", false, 0.5f);
            Engine::GetInstance().Get<AudioManager>().PlaySound(
                  "assets/media/fx/gaunt/default/attack_2.wav", false, 0.6f);
            if((actual_energy > 0.0) && (actual_energy - 30.0f) <= 0.0f){
              player_entity.component<Energy>()->energy = 0.0f;
              Engine::GetInstance().Get<AudioManager>().PlaySound(
                "assets/media/fx/gaunt/default/low_nrg.wav", false, 0.5f);
            }
            else player_entity.component<Energy>()->energy -= 30.0f;
          }
          //Alt left
          else if(keys_[GLFW_KEY_SPACE] && 
            !keys_[GLFW_KEY_UP] && 
            !keys_[GLFW_KEY_DOWN] &&
            !keys_[GLFW_KEY_RIGHT] &&
            keys_[GLFW_KEY_LEFT] &&
            (actual_energy > 0.0f)){

            if(entity.component<ThreeD>()){
              glm::quat rot;
              switch(player.orientation){
                case Player::Orientation::DOWN:
                  rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
                  break;
                case Player::Orientation::UP:
                  rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                  break;
                case Player::Orientation::RIGHT:
                  rot = glm::rotate(rot, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                  break;
                default:
                  break;
              }
              t.SetLocalOrientation(rot);
            }

            player.orientation = Player::Orientation::LEFT;
            time_passed_since_last_alt_attack_ = 0.0f;
            wizard.is_attacking = true;
            if(!entity.component<ThreeD>()){
              EntityFactory2D().MakeWizardProjectile(es, player_position, -3.14,glm::vec3(-100.0f, 0.0f, 0.0f), "special");
            }
            else{
              EntityFactory3D().MakeWizardProjectile(es, player_position, -3.14,glm::vec3(-100.0f, 0.0f, 0.0f), "special");
            }            
                        Engine::GetInstance().Get<AudioManager>().PlaySound(
                "assets/media/fx/gaunt/mage/alt.wav", false, 0.5f);
            Engine::GetInstance().Get<AudioManager>().PlaySound(
                  "assets/media/fx/gaunt/default/attack_2.wav", false, 0.6f);
            if((actual_energy > 0.0) && (actual_energy - 30.0f) <= 0.0f){
              player_entity.component<Energy>()->energy = 0.0f;
              Engine::GetInstance().Get<AudioManager>().PlaySound(
                "assets/media/fx/gaunt/default/low_nrg.wav", false, 0.5f);
            }
            else player_entity.component<Energy>()->energy -= 30.0f;
          }
          //Diagonales
          /*
          else if(keys_[GLFW_KEY_UP] && 
            !keys_[GLFW_KEY_DOWN] &&
            keys_[GLFW_KEY_RIGHT] &&
            !keys_[GLFW_KEY_LEFT] &&
            keys_[GLFW_KEY_SPACE] &&
            (actual_energy > 0.0f)){
            player.orientation = Player::Orientation::RIGHT;
            time_passed_since_last_alt_attack_ = 0.0f;
            wizard.is_attacking = true;
            EntityFactory2D().MakeWizardProjectile(es, player_position, 0.785,glm::vec3(50.0f, 50.0f, 0.0f), "special");
            Engine::GetInstance().Get<AudioManager>().PlaySound(
                "assets/media/fx/gaunt/mage/alt.wav", false, 0.5f);
            if((actual_energy > 0.0) && (actual_energy - 30.0f) <= 0.0f){
              player_entity.component<Energy>()->energy = 0.0f;
              Engine::GetInstance().Get<AudioManager>().PlaySound(
                "assets/media/fx/gaunt/default/low_nrg.wav", false, 0.5f);
            }
            else player_entity.component<Energy>()->energy -= 30.0f;
          }
          else if(
            !keys_[GLFW_KEY_DOWN] &&
            !keys_[GLFW_KEY_RIGHT] &&
            keys_[GLFW_KEY_UP] && 
            keys_[GLFW_KEY_LEFT] &&
            keys_[GLFW_KEY_SPACE] &&
            (actual_energy > 0.0f)){
            std::cout << "diagonal arriba izquierda" << std::endl;
            player.orientation = Player::Orientation::LEFT;
            time_passed_since_last_alt_attack_ = 0.0f;
            wizard.is_attacking = true;
            EntityFactory2D().MakeWizardProjectile(es, player_position, -2.355,glm::vec3(-50.0f, 50.0f, 0.0f), "special");
            Engine::GetInstance().Get<AudioManager>().PlaySound(
                "assets/media/fx/gaunt/mage/alt.wav", false, 0.5f);
            if((actual_energy > 0.0) && (actual_energy - 30.0f) <= 0.0f){
              player_entity.component<Energy>()->energy = 0.0f;
              Engine::GetInstance().Get<AudioManager>().PlaySound(
                "assets/media/fx/gaunt/default/low_nrg.wav", false, 0.5f);
            }
            else player_entity.component<Energy>()->energy -= 30.0f;
          }
          else if(!keys_[GLFW_KEY_UP] && 
            keys_[GLFW_KEY_DOWN] &&
            !keys_[GLFW_KEY_LEFT] &&
            keys_[GLFW_KEY_RIGHT] &&
            keys_[GLFW_KEY_SPACE] &&
            (actual_energy > 0.0f)){
            std::cout << "diagonal abajo derecha" << std::endl;
            player.orientation = Player::Orientation::RIGHT;
            time_passed_since_last_alt_attack_ = 0.0f;
            wizard.is_attacking = true;
            EntityFactory2D().MakeWizardProjectile(es, player_position, -0.785,glm::vec3(50.0f, -50.0f, 0.0f), "special");
            Engine::GetInstance().Get<AudioManager>().PlaySound(
                "assets/media/fx/gaunt/mage/alt.wav", false, 0.5f);
            if((actual_energy > 0.0) && (actual_energy - 30.0f) <= 0.0f){
              player_entity.component<Energy>()->energy = 0.0f;
              Engine::GetInstance().Get<AudioManager>().PlaySound(
                "assets/media/fx/gaunt/default/low_nrg.wav", false, 0.5f);
            }
            else player_entity.component<Energy>()->energy -= 30.0f;
          }
          else if(!keys_[GLFW_KEY_UP] && 
            keys_[GLFW_KEY_DOWN] &&
            !keys_[GLFW_KEY_RIGHT] &&
            keys_[GLFW_KEY_LEFT] &&
            keys_[GLFW_KEY_SPACE] &&
            (actual_energy > 0.0f)){
            std::cout << "diagonal abajo izq" << std::endl;
            player.orientation = Player::Orientation::LEFT;
            time_passed_since_last_alt_attack_ = 0.0f;
            wizard.is_attacking = true;
            EntityFactory2D().MakeWizardProjectile(es, player_position, -2.355,glm::vec3(-50.0f, -50.0f, 0.0f), "special");
            Engine::GetInstance().Get<AudioManager>().PlaySound(
                "assets/media/fx/gaunt/mage/alt.wav", false, 0.5f);
            if((actual_energy > 0.0) && (actual_energy - 30.0f) <= 0.0f){
              player_entity.component<Energy>()->energy = 0.0f;
              Engine::GetInstance().Get<AudioManager>().PlaySound(
                "assets/media/fx/gaunt/default/low_nrg.wav", false, 0.5f);
            }
            else player_entity.component<Energy>()->energy -= 30.0f;
          }*/
        }
        if(time_passed_since_last_magic_attack_ >= PlayerInputSystem::kMagicAttackDuration){         
          //Normal attack now
          //Normal Up
          if(!keys_[GLFW_KEY_SPACE] && 
            keys_[GLFW_KEY_UP] && 
            !keys_[GLFW_KEY_DOWN] &&
            !keys_[GLFW_KEY_RIGHT] &&
            !keys_[GLFW_KEY_LEFT]){

            if(entity.component<ThreeD>()){
              glm::quat rot;
              switch(player.orientation){
                case Player::Orientation::DOWN:
                  rot = glm::rotate(rot, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                  break;
                case Player::Orientation::LEFT:
                  rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                  break;
                case Player::Orientation::RIGHT:
                  rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
                  break;
                default:
                  break;
              }
              t.SetLocalOrientation(rot);
            }

            player.orientation = Player::Orientation::UP;
            time_passed_since_last_magic_attack_ = 0.0f;
            wizard.is_attacking = true;
            if(!entity.component<ThreeD>()){
              EntityFactory2D().MakeWizardProjectile(es, player_position, 1.57,glm::vec3(0.0f, 170.0f, 0.0f), "normal");
            }
            else{

              EntityFactory3D().MakeWizardProjectile(es, player_position, 1.57,glm::vec3(0.0f, 170.0f, 0.0f), "normal");
            }            
                        Engine::GetInstance().Get<AudioManager>().PlaySound(
                "assets/media/fx/gaunt/mage/attack.wav", false, 0.5f);
          }
          else if(!keys_[GLFW_KEY_SPACE] && 
            !keys_[GLFW_KEY_UP] && 
            keys_[GLFW_KEY_DOWN] &&
            !keys_[GLFW_KEY_RIGHT] &&
            !keys_[GLFW_KEY_LEFT]){

            if(entity.component<ThreeD>()){
              glm::quat rot;
              switch(player.orientation){
                case Player::Orientation::DOWN:
                  rot = glm::rotate(rot, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                  break;
                case Player::Orientation::LEFT:
                  rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                  break;
                case Player::Orientation::RIGHT:
                  rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
                  break;
                default:
                  break;
              }
              t.SetLocalOrientation(rot);
            }

            player.orientation = Player::Orientation::DOWN;
            time_passed_since_last_magic_attack_ = 0.0f;
            wizard.is_attacking = true;
            if(!entity.component<ThreeD>()){
              EntityFactory2D().MakeWizardProjectile(es, player_position, -1.57,glm::vec3(0.0f, -170.0f, 0.0f), "normal");
            }
            else{
              EntityFactory3D().MakeWizardProjectile(es, player_position, -1.57,glm::vec3(0.0f, -170.0f, 0.0f), "normal");
            }            
                        Engine::GetInstance().Get<AudioManager>().PlaySound(
                "assets/media/fx/gaunt/mage/attack.wav", false, 0.5f);
          }
          else if(!keys_[GLFW_KEY_SPACE] && 
            !keys_[GLFW_KEY_UP] && 
            !keys_[GLFW_KEY_DOWN] &&
            keys_[GLFW_KEY_RIGHT] &&
            !keys_[GLFW_KEY_LEFT]){

            if(entity.component<ThreeD>()){
              glm::quat rot;
              switch(player.orientation){
                case Player::Orientation::DOWN:
                  rot = glm::rotate(rot, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                  break;
                case Player::Orientation::LEFT:
                  rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                  break;
                case Player::Orientation::RIGHT:
                  rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
                  break;
                default:
                  break;
              }
              t.SetLocalOrientation(rot);
            }

            player.orientation = Player::Orientation::RIGHT;
            time_passed_since_last_magic_attack_ = 0.0f;
            wizard.is_attacking = true;
            if(!entity.component<ThreeD>()){
              EntityFactory2D().MakeWizardProjectile(es, player_position, 0.0,glm::vec3(170.0f, 0.0f, 0.0f), "normal");
            }
            else{
              EntityFactory3D().MakeWizardProjectile(es, player_position, 0.0,glm::vec3(170.0f, 0.0f, 0.0f), "normal");
            }            
                        Engine::GetInstance().Get<AudioManager>().PlaySound(
                "assets/media/fx/gaunt/mage/attack.wav", false, 0.5f);
          }
          else if(!keys_[GLFW_KEY_SPACE] && 
            !keys_[GLFW_KEY_UP] && 
            !keys_[GLFW_KEY_DOWN] &&
            !keys_[GLFW_KEY_RIGHT] &&
            keys_[GLFW_KEY_LEFT]){

            if(entity.component<ThreeD>()){
              glm::quat rot;
              switch(player.orientation){
                case Player::Orientation::DOWN:
                  rot = glm::rotate(rot, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                  break;
                case Player::Orientation::LEFT:
                  rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                  break;
                case Player::Orientation::RIGHT:
                  rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
                  break;
                default:
                  break;
              }
              t.SetLocalOrientation(rot);
            }
            
            player.orientation = Player::Orientation::LEFT;
            time_passed_since_last_magic_attack_ = 0.0f;
            wizard.is_attacking = true;
            if(!entity.component<ThreeD>()){
              EntityFactory2D().MakeWizardProjectile(es, player_position, -3.14,glm::vec3(-170.0f, 0.0f, 0.0f), "normal");
            }
            else{
              EntityFactory3D().MakeWizardProjectile(es, player_position, -3.14,glm::vec3(-170.0f, 0.0f, 0.0f), "normal");
            }            
                        Engine::GetInstance().Get<AudioManager>().PlaySound(
                "assets/media/fx/gaunt/mage/attack.wav", false, 0.5f);
          }
          // Diagonales
          /**
          else if(!keys_[GLFW_KEY_SPACE] && 
            keys_[GLFW_KEY_UP] && 
            !keys_[GLFW_KEY_DOWN] &&
            !keys_[GLFW_KEY_LEFT] &&
            keys_[GLFW_KEY_RIGHT]){
            player.orientation = Player::Orientation::RIGHT;
            time_passed_since_last_magic_attack_ = 0.0f;
            wizard.is_attacking = true;
            EntityFactory2D().MakeWizardProjectile(es, player_position, 0.785,glm::vec3(85.0f, 85.0f, 0.0f), "normal");
            Engine::GetInstance().Get<AudioManager>().PlaySound(
                "assets/media/fx/gaunt/mage/attack.wav", false, 0.5f);
          }
          else if(!keys_[GLFW_KEY_SPACE] && 
            keys_[GLFW_KEY_UP] && 
            !keys_[GLFW_KEY_DOWN] &&
            !keys_[GLFW_KEY_RIGHT] &&
            keys_[GLFW_KEY_LEFT]){
            player.orientation = Player::Orientation::LEFT;
            time_passed_since_last_magic_attack_ = 0.0f;
            wizard.is_attacking = true;
            EntityFactory2D().MakeWizardProjectile(es, player_position, 2.355,glm::vec3(-85.0f, 85.0f, 0.0f), "normal");
            Engine::GetInstance().Get<AudioManager>().PlaySound(
                "assets/media/fx/gaunt/mage/attack.wav", false, 0.5f);
          }
          else if(!keys_[GLFW_KEY_SPACE] && 
            !keys_[GLFW_KEY_UP] && 
            keys_[GLFW_KEY_DOWN] &&
            keys_[GLFW_KEY_RIGHT] &&
            !keys_[GLFW_KEY_LEFT]){
            player.orientation = Player::Orientation::RIGHT;
            time_passed_since_last_magic_attack_ = 0.0f;
            wizard.is_attacking = true;
            EntityFactory2D().MakeWizardProjectile(es, player_position, -0.785,glm::vec3(85.0f, -85.0f, 0.0f), "normal");
            Engine::GetInstance().Get<AudioManager>().PlaySound(
                "assets/media/fx/gaunt/mage/attack.wav", false, 0.5f);
          }
          else if(!keys_[GLFW_KEY_SPACE] && 
            !keys_[GLFW_KEY_UP] && 
            keys_[GLFW_KEY_DOWN] &&
            !keys_[GLFW_KEY_RIGHT] &&
            keys_[GLFW_KEY_LEFT]){
            player.orientation = Player::Orientation::LEFT;
            time_passed_since_last_magic_attack_ = 0.0f;
            wizard.is_attacking = true;
            EntityFactory2D().MakeWizardProjectile(es, player_position, -2.355,glm::vec3(-85.0f, -85.0f, 0.0f), "normal");
            Engine::GetInstance().Get<AudioManager>().PlaySound(
                "assets/media/fx/gaunt/mage/attack.wav", false, 0.5f);
          }*/
          else if(!keys_[GLFW_KEY_SPACE] && 
            !keys_[GLFW_KEY_UP] && 
            !keys_[GLFW_KEY_DOWN] &&
            !keys_[GLFW_KEY_RIGHT] &&
            !keys_[GLFW_KEY_LEFT]){
            wizard.is_attacking = false;
          }
          timerEnergy += dt;
          if(!keys_[GLFW_KEY_SPACE] && timerEnergy >=0.33f){
            timerEnergy = 0.0f; 
            if((actual_energy + 5.0f) >= 100.0f){
              player_entity.component<Energy>()->energy = 100.0f;
            }
            else player_entity.component<Energy>()->energy += 5.0f;
          }
        }
	   });
	  }
	}
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

  for (entityx::Entity e0 : es.entities_with_components(manueleth, animation)) {
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
  entityx::ComponentHandle<ThreeD> threed;

  bool three_d = false;
  for (entityx::Entity e1 : es.entities_with_components(threed)) {
    three_d = true;
  }

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
        float safeDistance = 45.0f;
        if(three_d){
          safeDistance = 30.0f;
        }
      	if (distancia <= safeDistance) {
      		if (manueleth.hits >= 3) {
      			manueleth.comportamiento = Manueleth::Comportamiento::PUSH;
            Engine::GetInstance().Get<AudioManager>().PlaySound(
              "assets/media/fx/manueleth/default/shockwave.wav", false, 0.6f);
            float throwDistance = 160.0f;
            if(three_d){
              throwDistance = 75.0f;
            }
      			glm::vec3 new_position(manueleth_position.x, manueleth_position.y - throwDistance , manueleth_position.z);

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
	            

	            if(!three_d){
                new_velocity = glm::normalize(player_position -
                               transform.GetWorldPosition()) * 100.0f;
                EntityFactory2D().MakeEnemyProjectile(
                  es, manueleth_position, angle_rad, new_velocity, "manueleth");
                manueleth.time_for_shooting = 0.0;
              }
              else{
                new_velocity = glm::normalize(player_position -
                               transform.GetWorldPosition()) * 40.0f;
                EntityFactory3D().MakeEnemyProjectile(
                  es, manueleth_position, angle_rad, new_velocity, "manueleth");
                manueleth.time_for_shooting = 0.0;
              }
	            
              Engine::GetInstance().Get<AudioManager>().PlaySound(
              "assets/media/fx/manueleth/default/attack.wav", false, 0.5f);
	      	}
      	}
   });
}

const float TurretIaSystem::turretSpeed = 10.0f;
const float TurretIaSystem::turretThreeDSpeed = 4.0f;
void TurretIaSystem::update(entityx::EntityManager &es,
                            entityx::EventManager &events,
                            entityx::TimeDelta dt) {

  entityx::ComponentHandle<ThreeD> threed;
  for (entityx::Entity e1 : es.entities_with_components(threed)) {
  three_d = true;
  }
  
  glm::vec3 player_position;
  es.each<Player, Transform>(
      [&](entityx::Entity entity, Player &player, Transform &player_transform) {
        player_position = player_transform.GetWorldPosition();
      });

  // Comprobamos la distancia a la que esta, si esta a menos de X distancia se
  // movera y disparara (disminuyendo asi la precision)
  glm::vec3 turret_position;
  es.each<Turret, Transform, Physics>([&](entityx::Entity entity,
                                          Turret &turret,
                                          Transform &turret_transform,
                                          Physics &turret_physics) {

    turret_position = turret_transform.GetWorldPosition();

    glm::vec3 vector_player_turret(player_position.x - turret_position.x,
                                       player_position.y - turret_position.y,
                                       0.0f);
    glm::vec3 vector_turret_v(0.0f, 1.0f, 0.0f);

    float angle_rad =
        std::atan2(vector_player_turret.y - vector_turret_v.y,
                   vector_player_turret.x - vector_turret_v.x);
    if(three_d){
      glm::quat rot;
      rot = glm::rotate(rot, angle_rad, glm::vec3(0.0f, 0.0f, 1.0f));
      turret_transform.SetLocalOrientation(rot);
    }

    const float distancia =
        std::sqrt(std::pow(std::abs(player_position.x - turret_position.x), 2) +
                  std::pow(std::abs(player_position.y - turret_position.y), 2));
    turret.time_passed += (dt * 250.0f);
    float safeDistance = 50.0f;
    if(three_d){
      safeDistance = 25.0f;
    }
    if (distancia < safeDistance) {
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

        if(!three_d){
          glm::vec3 new_velocity(0.0f, 0.0f, 0.0f);
          new_velocity = glm::normalize(player_position -
                                      turret_transform.GetWorldPosition()) * 100.0f;
          EntityFactory2D().MakeEnemyProjectile(es, turret_position, angle_rad,
                                               new_velocity, "torreta");
        }
        else{
          glm::vec3 new_velocity_threed(0.0f,0.0f,0.0f);
          new_velocity_threed = glm::normalize(player_position -
                                      turret_transform.GetWorldPosition()) * 50.0f;
          EntityFactory3D().MakeEnemyProjectile(es, turret_position, angle_rad,
                                               new_velocity_threed, "torreta");
        }
        
        turret.time_passed = 0.0;
      }
    }
  });
}

void TrapIaSystem::update(entityx::EntityManager &es,
                          entityx::EventManager &events,
                          entityx::TimeDelta dt) {
  glm::vec3 trap_position;
  es.each<Trap, Transform>([&](entityx::Entity entity, Trap &trap,
                               Transform &trap_transform) {
    trap_position = trap_transform.GetWorldPosition();

    trap.time_passed += (dt * 250.0f);

    if (trap.time_passed >= trap.frecuencia) {
      Engine::GetInstance().Get<AudioManager>().PlaySound(
          "assets/media/fx/turret/default/attack.wav", false, 1);
      glm::vec3 new_velocity(0.0f, 0.0f, 0.0f);
      switch (trap.orientation) {
        case Trap::Orientation::UP:
          if(!entity.component<ThreeD>()){
            EntityFactory2D().MakeEnemyProjectile(
                es, trap_position, 1.57, glm::vec3(0.0f, 100.0f, 0.0f), "trampa");
          }
          else{
            EntityFactory3D().MakeEnemyProjectile(
                es, trap_position, 1.57, glm::vec3(0.0f, 100.0f, 0.0f), "trampa");
          }
        break;
        case Trap::Orientation::DOWN:
          if(!entity.component<ThreeD>()){
            EntityFactory2D().MakeEnemyProjectile(es, trap_position, -1.57,
                                                   glm::vec3(0.0f, -100.0f, 0.0f),
                                                   "trampa");          }
          else{
            EntityFactory3D().MakeEnemyProjectile(es, trap_position, -1.57,
                                                   glm::vec3(0.0f, -100.0f, 0.0f),
                                                   "trampa");
          }
          break;
        case Trap::Orientation::LEFT:
          if(!entity.component<ThreeD>()){
            EntityFactory2D().MakeEnemyProjectile(es, trap_position, -3.14,
                                                   glm::vec3(-100.0f, 0.0f, 0.0f),
                                                   "trampa");
          }
          else{
            EntityFactory3D().MakeEnemyProjectile(es, trap_position, -3.14,
                                                   glm::vec3(-100.0f, 0.0f, 0.0f),
                                                   "trampa");
          }
          break;
        case Trap::Orientation::RIGHT:
          if(!entity.component<ThreeD>()){
            EntityFactory2D().MakeEnemyProjectile(
                es, trap_position, 0.0, glm::vec3(100.0f, 0.0f, 0.0f), "trampa");
          }
          else{
            EntityFactory3D().MakeEnemyProjectile(
                es, trap_position, 0.0, glm::vec3(100.0f, 0.0f, 0.0f), "trampa");
          }
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

// Anterior valor: 50 (muy rapido para 3D, pensar algo!)
const float GhostIaSystem::kSpeed = 50.0f;
const float GhostIaSystem::kThreeDSpeed = 15.0f;

float timerGhost;

void GhostIaSystem::update(entityx::EntityManager &es,
                           entityx::EventManager &events,
                           entityx::TimeDelta dt) {
  entityx::ComponentHandle<ThreeD> threed;
  bool three_d = false;
  for (entityx::Entity e1 : es.entities_with_components(threed)) {
    three_d = true;
  }
  glm::vec3 player_position;
  es.each<Player, Transform>(
      [&](entityx::Entity entity, Player &player, Transform &transform) {
        player_position = transform.GetWorldPosition();
      });

  glm::vec3 new_velocity(0.0f, 0.0f, 0.0f);
  es.each<Ghost, Transform, Physics>([&](entityx::Entity entity, Ghost &ghost,
                                         Transform &transform,
                                         Physics &physics) {
    glm::vec3 ghost_position;
    ghost_position = transform.GetWorldPosition();

    glm::vec3 vector_player_ghost(player_position.x - ghost_position.x,
                                       player_position.y - ghost_position.y,
                                       0.0f);
    glm::vec3 vector_ghost_v(0.0f, 1.0f, 0.0f);

    float angle_rad =
        std::atan2(vector_player_ghost.y - vector_ghost_v.y,
                   vector_player_ghost.x - vector_ghost_v.x);
    if(three_d){
      glm::quat rot;
      rot = glm::rotate(rot, angle_rad, glm::vec3(0.0f, 0.0f, 1.0f));
      transform.SetLocalOrientation(rot);
    }

    ghost.time_passed += dt * 1000.0f;
    switch (ghost.comportamiento) {
      case Ghost::Comportamiento::DAMAGE_TOP:
        if (ghost.time_passed >= ghost.kHitDuration) {
          ghost.time_passed = 0.0;
          ghost.comportamiento = Ghost::Comportamiento::FOLLOW;
        } else {
          new_velocity.y = -1.0f;
          if(!three_d){
            physics.velocity = glm::normalize(new_velocity) * kSpeed;
          }
          else{
            physics.velocity = glm::normalize(new_velocity) * kThreeDSpeed;
          }
          
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
          if(!three_d){
            physics.velocity = glm::normalize(new_velocity) * kSpeed;
          }
          else{
            physics.velocity = glm::normalize(new_velocity) * kThreeDSpeed;
          }          if (timerGhost == 0.0) {
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
          if(!three_d){
            physics.velocity = glm::normalize(new_velocity) * kSpeed;
          }
          else{
            physics.velocity = glm::normalize(new_velocity) * kThreeDSpeed;
          }          if (timerGhost == 0.0) {
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
          if(!three_d){
            physics.velocity = glm::normalize(new_velocity) * kSpeed;
          }
          else{
            physics.velocity = glm::normalize(new_velocity) * kThreeDSpeed;
          }          if (timerGhost == 0.0) {
            Engine::GetInstance().Get<AudioManager>().PlaySound(
                "assets/media/fx/ghost/default/back.wav", false, 0.8f);
          }
        }
        break;
      case Ghost::Comportamiento::FOLLOW:
          if(!three_d){
            physics.velocity =
              glm::normalize(player_position - transform.GetWorldPosition()) *
              kSpeed;          
            }
          else{
            glm::vec3 newVelocity((player_position.x - transform.GetWorldPosition().x),
              (player_position.y - transform.GetWorldPosition().y), 0);
            physics.velocity = glm::normalize(newVelocity) * kThreeDSpeed;
          }
        
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
    if(!collision_copy.e1.component<ThreeD>()){
      auto e1_color_animation = collision_copy.e1.component<ColorAnimation>();
      e1_color_animation->Play();
    }
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
    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/ghost/default/hit.wav", false, 0.7f);
    if(!collision_copy.e0.component<ThreeD>()){
      auto e0_color_animation = collision_copy.e0.component<ColorAnimation>();
      e0_color_animation->Play();
    }    
    // Torreta
  } else if (e0_weapon && e0_weapon->drawn &&
             e0_weapon->owner.component<Player>() &&
             collision_copy.e1.component<Turret>()) {
    auto e1_health = collision_copy.e1.component<Health>();
    e1_health->hp -= e0_weapon->damage;
    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/ghost/default/hit.wav", false, 0.7f);
    if(!collision_copy.e1.component<ThreeD>()){
      auto e1_color_animation = collision_copy.e1.component<ColorAnimation>();
      e1_color_animation->Play();
    }
  } else if (e1_weapon && e1_weapon->drawn &&
             e1_weapon->owner.component<Player>() &&
             collision_copy.e0.component<Turret>()) {
    auto e0_health = collision_copy.e0.component<Health>();
    e0_health->hp -= e1_weapon->damage;
    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/ghost/default/hit.wav", false, 0.7f);
    if(!collision_copy.e0.component<ThreeD>()){
      auto e0_color_animation = collision_copy.e0.component<ColorAnimation>();
      e0_color_animation->Play();
    }
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
    if(!collision_copy.e1.component<ThreeD>()){
      auto e1_color_animation = collision_copy.e1.component<ColorAnimation>();
      e1_color_animation->Play();
    }
  } else if (e1_weapon && e1_weapon->drawn &&
             e1_weapon->owner.component<Player>() &&
             collision_copy.e0.component<Manueleth>()) {
    auto e0_health = collision_copy.e0.component<Health>();
    e0_health->hp -= e1_weapon->damage;
    auto e0_manueleth = collision_copy.e0.component<Manueleth>();
    e0_manueleth->hits += 1;
    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/manueleth/default/hit.wav", false, 0.5f);
    if(!collision_copy.e0.component<ThreeD>()){
      auto e0_color_animation = collision_copy.e0.component<ColorAnimation>();
      e0_color_animation->Play();
    }
    // Lancero
  } else if (e0_weapon && e0_weapon->drawn && 
             e0_weapon->owner.component<Player>() &&
             collision_copy.e1.component<Lancer>()) {
    auto e1_health = collision_copy.e1.component<Health>();
    e1_health->hp -= e0_weapon->damage;
    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/lanc/default/hit.wav", false, 0.7f);
    if(!collision_copy.e1.component<ThreeD>()){
      auto e1_color_animation = collision_copy.e1.component<ColorAnimation>();
      e1_color_animation->Play();
    }
  } else if (e1_weapon && e1_weapon->drawn &&
             e1_weapon->owner.component<Player>() &&
             collision_copy.e0.component<Lancer>()) {
    auto e0_health = collision_copy.e0.component<Health>();
    e0_health->hp -= e1_weapon->damage;
    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/lanc/default/hit.wav", false, 0.7f);
    if(!collision_copy.e0.component<ThreeD>()){
      auto e0_color_animation = collision_copy.e0.component<ColorAnimation>();
      e0_color_animation->Play();
    }
    // Masiatrix
  } else if (e0_weapon && e0_weapon->drawn &&
             e0_weapon->owner.component<Player>() &&
             collision_copy.e1.component<Masiatrix>()) {
    auto e1_health = collision_copy.e1.component<Health>();
    e1_health->hp -= e0_weapon->damage;
    auto e1_masiatrix = collision_copy.e1.component<Masiatrix>();
    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/masiatrix/default/hit.wav", false, 0.5f);
    if(!collision_copy.e1.component<ThreeD>()){
      auto e1_color_animation = collision_copy.e1.component<ColorAnimation>();
      e1_color_animation->Play();
    }
  } else if (e1_weapon && e1_weapon->drawn &&
             e1_weapon->owner.component<Player>() &&
             collision_copy.e0.component<Masiatrix>()) {
    auto e0_health = collision_copy.e0.component<Health>();
    e0_health->hp -= e1_weapon->damage;
    auto e0_masiatrix = collision_copy.e0.component<Masiatrix>();
    if(!collision_copy.e0.component<ThreeD>()){
      auto e0_color_animation = collision_copy.e0.component<ColorAnimation>();
      e0_color_animation->Play();
    }
    // Menax
  } else if (e0_weapon && e0_weapon->drawn &&
             e0_weapon->owner.component<Player>() &&
             collision_copy.e1.component<Menax>()) {
    auto e1_health = collision_copy.e1.component<Health>();
    e1_health->hp -= e0_weapon->damage;
    auto e1_menax = collision_copy.e1.component<Menax>();
    e1_menax->hits++;

    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/menax/default/hit.wav", false, 0.5f);
    if(!collision_copy.e1.component<ThreeD>()){
      auto e1_color_animation = collision_copy.e1.component<ColorAnimation>();
      e1_color_animation->Play();
    }
  } else if (e1_weapon && e1_weapon->drawn &&
             e1_weapon->owner.component<Player>() &&
             collision_copy.e0.component<Menax>()) {
    auto e0_health = collision_copy.e0.component<Health>();
    e0_health->hp -= e1_weapon->damage;
    auto e0_menax = collision_copy.e0.component<Menax>();
    e0_menax->hits++;

    if(!collision_copy.e0.component<ThreeD>()){
      auto e0_color_animation = collision_copy.e0.component<ColorAnimation>();
      e0_color_animation->Play();
    }
  }
}

void KnightAttackSystem::update(entityx::EntityManager &es,
                                entityx::EventManager &events,
                                entityx::TimeDelta dt) {}

void EnemyProjectileSystem::configure(entityx::EventManager &event_manager) {
  event_manager.subscribe<Collision>(*this);
}

void EnemyProjectileSystem::receive(const Collision &collision) {
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
    if(!collision_copy.e1.component<ThreeD>()){
      auto e1_color_animation = collision_copy.e1.component<ColorAnimation>();
      e1_color_animation->Play();
    }    
  } else if (e1_projectile && collision_copy.e0.component<Player>()) {
    auto e0_health = collision_copy.e0.component<Health>();
    e0_health->hp -= e1_projectile->damage;

    entityx::Entity proyectil = collision.e1;
    proyectil.destroy();

    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/gaunt/default/hit.wav", false, 1);
    if(!collision_copy.e0.component<ThreeD>()){
      auto e0_color_animation = collision_copy.e0.component<ColorAnimation>();
      e0_color_animation->Play();
    }    
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

void EnemyProjectileSystem::update(entityx::EntityManager &es,
                                entityx::EventManager &events,
                                entityx::TimeDelta dt) {}


void ShieldSystem::configure(entityx::EventManager &event_manager) {
  event_manager.subscribe<Collision>(*this);
}

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
    
    float actual_energy = e1_energy->energy; 
    if ((actual_energy - 10.0f) <= 0.0f) {
    	e1_energy->energy = 0.0f;
    	collision_copy.e1.component<Shield>()->active = false;
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
    
    float actual_energy = e0_energy->energy; 
    if ((actual_energy - 10.0f) <= 0.0f) {
    	e0_energy->energy = 0.0f;
    	collision_copy.e0.component<Shield>()->active = false;
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
int times_masiatrix_died = 0;

void HealthSystem::update(entityx::EntityManager &es,
                          entityx::EventManager &events,
                          entityx::TimeDelta dt) {
  bool es_player = false;
  es.each<Health>([&](entityx::Entity entity, Health &health) {
    if (entity.component<Player>()) {
      es_player = true;
      if (lastPlayerHP != health.hp && health.hp != health.init_hp) {
        events.emit<Health>(health);
        lastPlayerHP = health.hp;
      }
    } else {
      es_player = false;
    }
    if (health.hp <= 0.0f) {
      Engine::GetInstance().Get<AudioManager>().PlaySound(health.death_fx,
                                                          false, 0.7);
      if (entity.component<Manueleth>()) {
        es.each<WizardProjectile>(
          [&](entityx::Entity entity_p, WizardProjectile &w) {
            entity_p.destroy();
          });
      	entity.destroy();
      	events.emit<StartLevel2>();
      } else if (entity.component<Masiatrix>()) {
        bool three_d = entity.component<ThreeD>();
        if (entity.component<Masiatrix>()->real){
          es.each<MasiatrixLegs, ParentLink>(
          [&](entityx::Entity entity_legs, MasiatrixLegs &legs, ParentLink &parent) {
              entity_legs.destroy();
          });
          es.each<Masiatrix>(
          [&](entityx::Entity entity, Masiatrix &masiatrix) {
              entity.destroy();
          });
          times_masiatrix_died++;
          if (times_masiatrix_died >= 3) {
            events.emit<StartLevel3>();
          } else {
            MasiatrixNextPhase mnp(times_masiatrix_died, three_d);    
            events.emit<MasiatrixNextPhase>(mnp);
          }
        } else {
          es.each<MasiatrixLegs, ParentLink>(
          [&](entityx::Entity entity_legs, MasiatrixLegs &legs, ParentLink &parent) {
            if (parent.owner == entity) {
              entity_legs.destroy();
            }
          });
          entity.destroy();
        }
      } else if (entity.component<Menax>()) {
        es.each<MenaxHitBox>(
          [&](entityx::Entity entity_legs, MenaxHitBox &menaxHit) {
            if (menaxHit.owner == entity) {
              entity_legs.destroy();
            }
          });
        entity.destroy();
        //Enviar evento para pantalla final o whatever
      }
      else {
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

        es.each<LancerLegs, ParentLink>([&](entityx::Entity entity_legs,
                                          LancerLegs &legs,
                                          ParentLink &parent) {
          if (parent.owner == entity) {
            entity_legs.destroy();
          }
        });

        es.each<LancerHitBox>(
          [&](entityx::Entity entity_hitbox, LancerHitBox &lancer_hitbox) {
          if (lancer_hitbox.owner == entity) {
            entity_hitbox.destroy();
          }
        });

        es.each<GhostHitBox>(
          [&](entityx::Entity entity_hitbox, GhostHitBox &ghost_hitbox) {
          if (ghost_hitbox.owner == entity) {
            entity_hitbox.destroy();
          }
        });
        
        entity.destroy();

        if (es_player) {
          events.emit<Death>();
        }
      }
    }
  });
}

void ChestSystem::configure(entityx::EventManager &event_manager) {
  event_manager.subscribe<Collision>(*this);
  event_manager_ = &event_manager;
}

bool check = true;
bool once2 = true;
void ChestSystem::receive(const engine::events::Collision &collision) {
  auto collision_copy = collision;
  if (!collision_copy.e0.valid() || !collision_copy.e1.valid()) {
    return;
  }
  auto e0_player = collision_copy.e0.component<Player>();
  auto e1_player = collision_copy.e1.component<Player>();

  if (e0_player && collision_copy.e1.component<Chest>()) {
    auto chest = collision_copy.e1.component<Chest>();
    if (chest->key == true) {
      if(once2){
        once2=false;
        Engine::GetInstance().Get<AudioManager>().PlaySound(
            "assets/media/fx/defaults/box_open.wav", false, 0.8f);
        PlayText pt("Has encontrado la llave!\n\nDirigete a la puerta del boss para enfrentarte a el.\n\n\n\n\n                    Pulsa [ENTER] para continuar.");
        event_manager_->emit<PlayText>(pt);
      }      
      if(!e0_player->key){
        e0_player->key = true;
        check = true;
      } 
      else {
        if(once2){
          once2 = false;
          Engine::GetInstance().Get<AudioManager>().PlaySound(
            "assets/media/fx/defaults/box_open.wav", false, 0.8f);
          PlayText pt("Ya tienes la llave.\n\nDirigete a la puerta del boss para enfrentarte a el.\n\n\n\n\n                    Pulsa [ENTER] para continuar.");
          event_manager_->emit<PlayText>(pt);
        }
      }
    } else {
        if(once2){
          Engine::GetInstance().Get<AudioManager>().PlaySound(
            "assets/media/fx/defaults/box_open.wav", false, 0.8f);
          once2 = false;
          PlayText pt("La llave no esta en este cofre.\n\nSigue buscando, seguro que la encuentras.\n\n\n\n\n                    Pulsa [ENTER] para continuar.");
          event_manager_->emit<PlayText>(pt);
        }        
    }
  } else if (e1_player && collision_copy.e0.component<Chest>()) {
    auto chest = collision_copy.e0.component<Chest>();
    Engine::GetInstance().Get<AudioManager>().PlaySound(
            "assets/media/fx/defaults/box_open.wav", false, 0.8f);
    if (chest->key == true) {

      if(!e1_player->key){
        e1_player->key = true;
        check = true;
        PlayText pt("Has encontrado la llave!\n\nDirigete a la puerta del boss para enfrentarte a el.\n\n\n\n\n                    Pulsa [ENTER] para continuar.");
        event_manager_->emit<PlayText>(pt);
      }
      else{
        if(once2){
          once2 = false;
          Engine::GetInstance().Get<AudioManager>().PlaySound(
            "assets/media/fx/defaults/box_open.wav", false, 0.8f);
          PlayText pt("Ya tienes la llave.\n\nDirigete a la puerta del boss para enfrentarte a el.\n\n\n\n\n                    Pulsa [ENTER] para continuar.");
          event_manager_->emit<PlayText>(pt);
        }
      }
    } else {
        if(once2){
          Engine::GetInstance().Get<AudioManager>().PlaySound(
            "assets/media/fx/defaults/box_open.wav", false, 0.8f);
          once2 = false;
          PlayText pt("La llave no esta en este cofre.\n\nSigue buscando, seguro que la encuentras.\n\n\n\n\n                    Pulsa [ENTER] para continuar.");
          event_manager_->emit<PlayText>(pt);
        }  
    }
  }
}

float timerChest = 0.0f;
void ChestSystem::update(entityx::EntityManager &es,
                                entityx::EventManager &events,
                                entityx::TimeDelta dt) {
  es.each<Player, Transform>(
    [&](entityx::Entity entity, Player &player, Transform &transform){
      if(player.key  && check){
        check = false;
        events.emit<Player>(player);
        
      }
    });
  if(timerChest >= 5){
    once2 = true;
    timerChest = 0.0f;
  }
  timerChest +=dt;

}

float timerLancer;

void LancerWalkingSystem::update(entityx::EntityManager &es,
                                 entityx::EventManager &events,
                                 entityx::TimeDelta dt) {
  entityx::ComponentHandle<SpriteAnimation> animation;
  entityx::ComponentHandle<Physics> physics;
  entityx::ComponentHandle<ParentLink> parent;
  entityx::ComponentHandle<LancerLegs> legs;

  std::string animToPlay;

  for (entityx::Entity e1 :
       es.entities_with_components(animation, parent, legs)) {
    if ((parent->owner.component<Physics>()->velocity.x < 0) ||
        (parent->owner.component<Physics>()->velocity.x > 0) ||
        (parent->owner.component<Physics>()->velocity.y > 0) ||
        (parent->owner.component<Physics>()->velocity.y < 0)) {
      animToPlay = "moving";

      if (timerLancer == 0.0) {
        Engine::GetInstance().Get<AudioManager>().PlaySound(
            "assets/media/fx/lanc/default/mov.wav", false, 0.8f);
      }
    } else {
      animToPlay = "stand";
    }
    animation->Play(animToPlay);
  }
  timerLancer += dt;
  if (timerLancer >= 0.22) {
    timerLancer = 0.0;
  }
}

const float LancerIaSystem::lancerSpeed = 70.0f;
const float LancerIaSystem::lancerThreeDSpeed = 30.0f;
void LancerIaSystem::update(entityx::EntityManager &es,
                            entityx::EventManager &events,
                            entityx::TimeDelta dt) {


  for(auto e : es.entities_with_components<ThreeD>()){
    three_d = true;
  }

  glm::vec3 player_position;
  es.each<Player, Transform>(
      [&](entityx::Entity entity, Player &player, Transform &player_transform) {
        player_position = player_transform.GetWorldPosition();
      });

  glm::vec3 lancer_position;
  es.each<Lancer, Transform, Physics>(
      [&](entityx::Entity entity, Lancer &lancer, Transform &lancer_transform,
          Physics &lancer_physics) {
        lancer_position = lancer_transform.GetWorldPosition();
        const float distancia = std::sqrt(
            std::pow(std::abs(player_position.x - lancer_position.x), 2) +
            std::pow(std::abs(player_position.y - lancer_position.y), 2));

        
        if(three_d){
          glm::vec3 vector_player_lancer(player_position.x - lancer_position.x,
                                           player_position.y - lancer_position.y,
                                           0.0f);
          glm::vec3 vector_lancer_v(0.0f, 1.0f, 0.0f);

          float angle_rad =
              std::atan2(vector_player_lancer.y - vector_lancer_v.y,
                         vector_player_lancer.x - vector_lancer_v.x);
          glm::quat rot;
          rot = glm::rotate(rot, angle_rad, glm::vec3(0.0f, 0.0f, 1.0f));
          lancer_transform.SetLocalOrientation(rot);
        }

        lancer.time_passed += (dt * 1000.0f);

        float safeDistance = 30.0f;
        float followDistance = 35.0f;
        float keepDistance = 10.0f;
        float speed = lancerSpeed;
        if(three_d){
          safeDistance = 12.0f;
          followDistance = 17.0f;
          keepDistance = 4.0f;
          speed = lancerThreeDSpeed;
        }

        if (lancer.time_passed >= 5000.0f) {
        	lancer.is_attacking = true;
          Engine::GetInstance().Get<AudioManager>().PlaySound(
            "assets/media/fx/lanc/default/attack.wav", false, 0.8f);
        	if (lancer_position.y >= player_position.y) {
        		lancer.orientation = Lancer::LancerOrientation::DOWN;
        	} else if (lancer_position.y < player_position.y) {
        		lancer.orientation = Lancer::LancerOrientation::UP;
        	} /*else if (lancer_position.x >= player_position.x) {	
        		lancer.orientation = Lancer::LancerOrientation::LEFT;
        	} else if (lancer_position.x < player_position.x) {
        		lancer.orientation = Lancer::LancerOrientation::RIGHT;
        	}*/ 
        	if (distancia < safeDistance) { 
        		lancer_physics.velocity =
	              -1.0f *
	              glm::normalize(player_position -
	                             lancer_transform.GetWorldPosition()) *
	              speed;
        	} else if (distancia > followDistance) {
        		lancer_physics.velocity =
	              1.0f *
	              glm::normalize(player_position -
	                             lancer_transform.GetWorldPosition()) *
	              speed;
        	} else {
        		lancer_physics.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
        	}

        	if (lancer.time_passed >= 10000.0f ) {
        		lancer.time_passed = 0.0f;
        		lancer.is_attacking = false;
        	}
        } else {
        	lancer.is_attacking = false;
        	if (distancia < safeDistance) {
        		lancer_physics.velocity =
	              -1.0f *
	              glm::normalize(player_position -
	                             lancer_transform.GetWorldPosition()) *
	              keepDistance;
        	} else if (distancia > followDistance) {
        		lancer_physics.velocity =
	              1.0f *
	              glm::normalize(player_position -
	                             lancer_transform.GetWorldPosition()) *
	              keepDistance;
        	} else {
        		lancer_physics.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
        	}   	
        }
    });
}

void LancerAnimationSystem::update(entityx::EntityManager &es,
                            entityx::EventManager &events,
                            entityx::TimeDelta dt) {
  entityx::ComponentHandle<Lancer> lancer;
  entityx::ComponentHandle<Transform> position_lancer;
  entityx::ComponentHandle<Physics> physics_lancer;
  entityx::ComponentHandle<SpriteAnimation> animation;
  
  std::string animToPlay;

  for (entityx::Entity e0 : es.entities_with_components(
           lancer, position_lancer, physics_lancer, animation)) {
  	if (lancer->is_attacking) {
  		switch (lancer->orientation) {
  			case Lancer::LancerOrientation::UP:
  			animToPlay = "attack_top";
  			break;
  			case Lancer::LancerOrientation::DOWN:
  			animToPlay = "attack_bottom";
  			break;
  			case Lancer::LancerOrientation::RIGHT:
  			animToPlay = "attack_right";
  			break;
  			case Lancer::LancerOrientation::LEFT:
  			animToPlay = "attack_left";
  			break;
  		}
  	} else {
  		switch (lancer->orientation) {
  			case Lancer::LancerOrientation::UP:
  			animToPlay = "moving_top";
  			break;
  			case Lancer::LancerOrientation::DOWN:
  			animToPlay = "moving_bottom";
  			break;
  			case Lancer::LancerOrientation::RIGHT:
  			animToPlay = "moving_right";
  			break;
  			case Lancer::LancerOrientation::LEFT:
  			animToPlay = "moving_left";
  			break;
  		}
  	}
  	animation->Play(animToPlay);
  }
}


void LancerAttackSystem::configure(entityx::EventManager &event_manager) {
  event_manager.subscribe<Collision>(*this);
}

void LancerAttackSystem::receive(const Collision &collision) {
  auto collision_copy = collision;
  if (!collision_copy.e0.valid() || !collision_copy.e1.valid()) {
    return;
  }
  auto e0_projectile = collision_copy.e0.component<LancerHitBox>();
  auto e1_projectile = collision_copy.e1.component<LancerHitBox>();

  if (e0_projectile && collision_copy.e1.component<Player>() && (e0_projectile->owner).component<Lancer>()->is_attacking){
    auto e1_health = collision_copy.e1.component<Health>();
    e1_health->hp -= e0_projectile->damage;

    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/gaunt/default/hit.wav", false, 1);
    if(!collision_copy.e1.component<ThreeD>()){
      auto e1_color_animation = collision_copy.e1.component<ColorAnimation>();
      e1_color_animation->Play();
    }
  } else if (e1_projectile && collision_copy.e0.component<Player>() && (e1_projectile->owner).component<Lancer>()->is_attacking) {
    auto e0_health = collision_copy.e0.component<Health>();
    e0_health->hp -= e1_projectile->damage;

    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/gaunt/default/hit.wav", false, 1);
    if(!collision_copy.e0.component<ThreeD>()){
      auto e0_color_animation = collision_copy.e0.component<ColorAnimation>();
      e0_color_animation->Play();
    }
  } 
}

void LancerAttackSystem::update(entityx::EntityManager &es,
                                entityx::EventManager &events,
                                entityx::TimeDelta dt) {}


void GhostAttackSystem::configure(entityx::EventManager &event_manager) {
  event_manager.subscribe<Collision>(*this);
}

void GhostAttackSystem::receive(const Collision &collision) {
  auto collision_copy = collision;
  if (!collision_copy.e0.valid() || !collision_copy.e1.valid()) {
    return;
  }
  auto e0_projectile = collision_copy.e0.component<GhostHitBox>();
  auto e1_projectile = collision_copy.e1.component<GhostHitBox>();

  if (e0_projectile && collision_copy.e1.component<Player>()){
    auto e1_health = collision_copy.e1.component<Health>();
    e1_health->hp -= e0_projectile->damage;

    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/gaunt/default/hit.wav", false, 1);
    if(!collision_copy.e1.component<ThreeD>()){
      auto e1_color_animation = collision_copy.e1.component<ColorAnimation>();
      e1_color_animation->Play();
    }

  } else if (e1_projectile && collision_copy.e0.component<Player>()) {
    auto e0_health = collision_copy.e0.component<Health>();
    e0_health->hp -= e1_projectile->damage;

    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/gaunt/default/hit.wav", false, 1);
    if(!collision_copy.e0.component<ThreeD>()){
      auto e0_color_animation = collision_copy.e0.component<ColorAnimation>();
      e0_color_animation->Play();
    }
  } 
}
void GhostAttackSystem::update(entityx::EntityManager &es,
                                entityx::EventManager &events,
                                entityx::TimeDelta dt) {}

void LeverSystem::configure(entityx::EventManager &event_manager) {
  event_manager.subscribe<Collision>(*this);
  event_manager_ = &event_manager;
}


float timerLever = 5.0f;

void LeverSystem::receive(const engine::events::Collision &collision) {
  auto collision_copy = collision;
  if (!collision_copy.e0.valid() || !collision_copy.e1.valid()) {
    return;
  }
  auto e0_player = collision_copy.e0.component<Player>();
  auto e1_player = collision_copy.e1.component<Player>();

  if (e0_player && collision_copy.e1.component<Lever>()) {
    auto lever = collision_copy.e1.component<Lever>();
    if(timerLever >= 5.0f){
      timerLever = 0.0f;
      if(!lever->activated){
        Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/defaults/lever.wav", false, 0.7f);
        lever->activated = true;
        e0_player->levers_activated++;
        PlayText pt("Palanca activada!\n\nRecuerda que son DOS las palancas que tienes que activar para enfrentarte al boss.\n\n\n\n\n                    Pulsa [ENTER] para continuar.");
        event_manager_->emit<PlayText>(pt);
      }
      else{
        PlayText pt("Esta palanca ya esta activada.\n\nRecuerda que son DOS las palancas que tienes que activar.\n\n\n\n\n                    Pulsa [ENTER] para continuar.");
        event_manager_->emit<PlayText>(pt);
      }
    }    
  } else if (e1_player && collision_copy.e0.component<Lever>()) {
    auto lever = collision_copy.e0.component<Lever>();
    if(timerLever >= 5.0f){
      timerLever = 0.0f;
      if(!lever->activated){
        Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/defaults/lever.wav", false, 0.7f);
        lever->activated = true;
        e0_player->levers_activated++;
        PlayText pt("Palanca activada!\n\nRecuerda que son DOS las palancas que tienes que activar para enfrentarte al boss.\n\n\n\n\n                    Pulsa [ENTER] para continuar.");
        event_manager_->emit<PlayText>(pt);
      }
      else{
        PlayText pt("Esta palanca ya esta activada.\n\nRecuerda que son DOS las palancas que tienes que activar.\n\n\n\n\n                    Pulsa [ENTER] para continuar.");
        event_manager_->emit<PlayText>(pt);
      }
    }
  }
}

void LeverSystem::update(entityx::EntityManager &es,
                                entityx::EventManager &events,
                                entityx::TimeDelta dt) {
  if(timerLever <=5.0f){
    timerLever += dt;
  }
}


void WizardAttackSystem::configure(entityx::EventManager &event_manager) {
  event_manager.subscribe<Collision>(*this);
}

void WizardAttackSystem::receive(const Collision &collision) {
  auto collision_copy = collision;
  if (!collision_copy.e0.valid() || !collision_copy.e1.valid()) {
    return;
  }
  auto e0_weapon = collision_copy.e0.component<WizardProjectile>();
  auto e1_weapon = collision_copy.e1.component<WizardProjectile>();

  if (e0_weapon &&
      collision_copy.e1.component<Ghost>()) {
    auto e1_health = collision_copy.e1.component<Health>();
    e1_health->hp -= e0_weapon->damage;

    entityx::Entity proyectil = collision.e0;
    proyectil.destroy();

    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/ghost/default/hit.wav", false, 0.7f);
    if(!collision_copy.e1.component<ThreeD>()){
      auto e1_color_animation = collision_copy.e1.component<ColorAnimation>();
      e1_color_animation->Play();
    }
    
  } else if (e1_weapon &&
             collision_copy.e0.component<Ghost>()) {

    auto e0_health = collision_copy.e0.component<Health>();
    e0_health->hp -= e1_weapon->damage;

    entityx::Entity proyectil = collision.e1;
    proyectil.destroy();
    
    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/ghost/default/hit.wav", false, 0.7f);
    if(!collision_copy.e0.component<ThreeD>()){
      auto e0_color_animation = collision_copy.e0.component<ColorAnimation>();
      e0_color_animation->Play();
    }
    // Torreta
  } else if (e0_weapon &&
             collision_copy.e1.component<Turret>()) {
    auto e1_health = collision_copy.e1.component<Health>();
    e1_health->hp -= e0_weapon->damage;

    entityx::Entity proyectil = collision.e0;
    proyectil.destroy();

    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/ghost/default/hit.wav", false, 0.7f);
    if(!collision_copy.e1.component<ThreeD>()){
      auto e1_color_animation = collision_copy.e1.component<ColorAnimation>();
      e1_color_animation->Play();
    }
  } else if (e1_weapon &&
             collision_copy.e0.component<Turret>()) {
    auto e0_health = collision_copy.e0.component<Health>();
    e0_health->hp -= e1_weapon->damage;

    entityx::Entity proyectil = collision.e1;
    proyectil.destroy();

    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/ghost/default/hit.wav", false, 0.7f);

    if(!collision_copy.e0.component<ThreeD>()){
      auto e0_color_animation = collision_copy.e0.component<ColorAnimation>();
      e0_color_animation->Play();
    };
    // Manueleth
  } else if (e0_weapon &&
             collision_copy.e1.component<Manueleth>()) {
    auto e1_health = collision_copy.e1.component<Health>();
    e1_health->hp -= e0_weapon->damage;
    auto e1_manueleth = collision_copy.e1.component<Manueleth>();
    e1_manueleth->hits += 1;

    entityx::Entity proyectil = collision.e0;
    proyectil.destroy();

    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/manueleth/default/hit.wav", false, 0.5f);
    if(!collision_copy.e1.component<ThreeD>()){
      auto e1_color_animation = collision_copy.e1.component<ColorAnimation>();
      e1_color_animation->Play();
    }
  } else if (e1_weapon &&
             collision_copy.e0.component<Manueleth>()) {
    auto e0_health = collision_copy.e0.component<Health>();
    e0_health->hp -= e1_weapon->damage;
    auto e0_manueleth = collision_copy.e0.component<Manueleth>();
    e0_manueleth->hits += 1;

    entityx::Entity proyectil = collision.e1;
    proyectil.destroy();

    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/manueleth/default/hit.wav", false, 0.5f);

    if(!collision_copy.e0.component<ThreeD>()){
      auto e0_color_animation = collision_copy.e0.component<ColorAnimation>();
      e0_color_animation->Play();
    }
    // Lancero
  } else if (e0_weapon &&
             collision_copy.e1.component<Lancer>()) {
    auto e1_health = collision_copy.e1.component<Health>();
    e1_health->hp -= e0_weapon->damage;

    entityx::Entity proyectil = collision.e0;
    proyectil.destroy();

    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/lanc/default/hit.wav", false, 0.7f);

    if(!collision_copy.e1.component<ThreeD>()){
      auto e0_color_animation = collision_copy.e0.component<ColorAnimation>();
      e0_color_animation->Play();
    }
  } else if (e1_weapon &&
             collision_copy.e0.component<Lancer>()) {
    auto e0_health = collision_copy.e0.component<Health>();
    e0_health->hp -= e1_weapon->damage;

    entityx::Entity proyectil = collision.e1;
    proyectil.destroy();

    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/lanc/default/hit.wav", false, 0.7f);

    if(!collision_copy.e0.component<ThreeD>()){
      auto e0_color_animation = collision_copy.e0.component<ColorAnimation>();
      e0_color_animation->Play();
    }
    // Masiatrix
  } else if (e0_weapon &&
             collision_copy.e1.component<Masiatrix>()) {
    auto e1_health = collision_copy.e1.component<Health>();
    e1_health->hp -= e0_weapon->damage;

    entityx::Entity proyectil = collision.e0;
    proyectil.destroy();

    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/masiatrix/default/hit.wav", false, 0.7f);
    if(!collision_copy.e1.component<ThreeD>()){
      auto e1_color_animation = collision_copy.e1.component<ColorAnimation>();
      e1_color_animation->Play();
    }
  } else if (e1_weapon &&
             collision_copy.e0.component<Masiatrix>()) {
    auto e0_health = collision_copy.e0.component<Health>();
    e0_health->hp -= e1_weapon->damage;

  entityx::Entity proyectil = collision.e1;
    proyectil.destroy();

    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/masiatrix/default/hit.wav", false, 0.7f);

    if(!collision_copy.e0.component<ThreeD>()){
      auto e0_color_animation = collision_copy.e0.component<ColorAnimation>();
      e0_color_animation->Play();
    }
    // Menax
  } else if (e0_weapon &&
             collision_copy.e1.component<Menax>()) {
    auto e1_health = collision_copy.e1.component<Health>();
    e1_health->hp -= e0_weapon->damage;

    entityx::Entity proyectil = collision.e0;
    proyectil.destroy();

    auto e1_menax = collision_copy.e1.component<Menax>();
    e1_menax->hits++;

    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/menax/default/hit.wav", false, 0.5f);
    if(!collision_copy.e1.component<ThreeD>()){
      auto e1_color_animation = collision_copy.e1.component<ColorAnimation>();
      e1_color_animation->Play();
    }
  } else if (e1_weapon &&
             collision_copy.e0.component<Menax>()) {
    auto e0_health = collision_copy.e0.component<Health>();
    e0_health->hp -= e1_weapon->damage;

    entityx::Entity proyectil = collision.e1;
    proyectil.destroy();

    auto e0_menax = collision_copy.e0.component<Menax>();
    e0_menax->hits++;
    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/menax/default/hit.wav", false, 0.5f);
    if(!collision_copy.e0.component<ThreeD>()){
      auto e0_color_animation = collision_copy.e0.component<ColorAnimation>();
      e0_color_animation->Play();
    }
  } else {
  	if (e1_weapon) {
      entityx::Entity proyectil = collision.e1;
      proyectil.destroy();
    } else if (e0_weapon) {
      entityx::Entity proyectil = collision.e0;
      proyectil.destroy();
    }
  }
}

void WizardAttackSystem::update(entityx::EntityManager &es,
                                entityx::EventManager &events,
                                entityx::TimeDelta dt) {}

bool once4 = false;
void MasiatrixAnimationSystem::update(entityx::EntityManager &es,
                                   entityx::EventManager &events,
                                   entityx::TimeDelta dt) {
  entityx::ComponentHandle<SpriteAnimation> animation;
  entityx::ComponentHandle<Physics> physics;
  entityx::ComponentHandle<Masiatrix> masiatrix;
  std::string animToPlay; 

  entityx::Entity player_entity;
  for (auto e : es.entities_with_components<Masiatrix>()) {
    player_entity = e;
  }

  for (entityx::Entity e1 :
       es.entities_with_components(animation, physics, masiatrix)) {
    if (masiatrix->is_attacking) {
      if (timer2 == 0.0) {
        // Falta el audio de ataque
        /*Engine::GetInstance().Get<AudioManager>().PlaySound(
            "assets/media/fx/masiatrix/default/attack.wav", false, 0.6f);*/
      }
      once4 = false;
      switch (masiatrix->orientation) {
        case Masiatrix::Orientation::TOP:
          animToPlay = "attacking_top";
          break;
        case Masiatrix::Orientation::DOWN:
          animToPlay = "attacking_bottom";
          break;
        case Masiatrix::Orientation::LEFT:
          animToPlay = "attacking_left";
          break;
        case Masiatrix::Orientation::RIGHT:
          animToPlay = "attacking_right";
          break;
      }
    } else if (physics->velocity.x > 0) {
      once4 = false;
      animToPlay = "moving_right";
      if (timer2 == 0.0) {
        Engine::GetInstance().Get<AudioManager>().PlaySound(
            "assets/media/fx/masiatrix/default/mov.wav", false, 0.6f);
      }
    } else if (physics->velocity.x < 0) {
      once4 = false;
      animToPlay = "moving_left";

      if (timer2 == 0.0) {
        Engine::GetInstance().Get<AudioManager>().PlaySound(
            "assets/media/fx/masiatrix/default/mov.wav", false, 0.6f);
      }
    } else if (physics->velocity.y > 0) {
      once4 = false;
      animToPlay = "moving_top";

      if (timer2 == 0.0) {
        Engine::GetInstance().Get<AudioManager>().PlaySound(
            "assets/media/fx/masiatrix/default/mov.wav", false, 0.6f);
      }
    } else if (physics->velocity.y < 0) {
      once4 = false;
      animToPlay = "moving_bottom";

      if (timer2 == 0.0) {
        Engine::GetInstance().Get<AudioManager>().PlaySound(
            "assets/media/fx/masiatrix/default/mov.wav", false, 0.6f);
      }
    } else {
      once4 = false;
      if (lastAnim.empty()) {
        animToPlay = "moving_bottom";
      } else
        animToPlay = lastOrientation;
    }
    animation->Play(animToPlay);   
    // Save last anim anyway for sound purposes
    //lastAnim = animToPlay;

  /*if (lastAnim.find("attack") != std::string::npos) {
    if (timer == 0.0) {
      Engine::GetInstance().Get<AudioManager>().PlaySound(
          "assets/media/fx/gaunt/warrior/attack.wav", false, 0.8f);
      Engine::GetInstance().Get<AudioManager>().PlaySound(
          "assets/media/fx/gaunt/default/attack_2.wav", false, 0.6f);
    }*/
    timer += dt;
    if (timer >= 0.5) {
      timer = 0.0;
    }
    // Sonido de pasos en caso de que este atacando y moviendo
    if (physics->velocity.x != 0 || physics->velocity.y != 0) {
      if (timer2 == 0.0) {
        Engine::GetInstance().Get<AudioManager>().PlaySound(
            "assets/media/fx/masiatrix/default/mov.wav", false, 0.6f);
      }
    }
  }
  timer2 += dt;
  if (timer2 >= 0.2) {
    timer2 = 0.0;
  }
}

MasiatrixInfo masiatrixInfo;

void MasiatrixIaSystem::update(entityx::EntityManager &es,
                            entityx::EventManager &events,
                            entityx::TimeDelta dt) {
  entityx::ComponentHandle<Player> p;
  entityx::ComponentHandle<Transform> t;

  glm::vec3 next_velocity;
  glm::vec3 projectile_velocity;
  glm::vec3 player_position;
  glm::vec3 masiatrix_position;
  int direction;
  es.each<Player, Transform>(
      [&](entityx::Entity entity, Player &player, Transform &player_transform) {
        player_position = player_transform.GetWorldPosition();
      });

  es.each<Masiatrix, Transform, Physics>(
      [&](entityx::Entity entity, Masiatrix &masiatrix, Transform &transform, Physics &physics) {
        if (!masiatrix.rand_initialized) {
          srand(time(0));
          masiatrix.rand_initialized = true;
          if (masiatrix.id == "A") {
            masiatrixInfo.positionA = masiatrix.original_position;
          } else if (masiatrix.id == "B") {
            masiatrixInfo.positionB = masiatrix.original_position;
          } else if (masiatrix.id == "C") {
            masiatrixInfo.positionC = masiatrix.original_position;
          } else if (masiatrix.id == "D") {
            masiatrixInfo.positionD = masiatrix.original_position;
          }
        }

        masiatrix.time_passed_attack += (dt * 1000.0f);
        masiatrix.time_passed_movement += (dt * 1000.0f);
        masiatrix_position = transform.GetWorldPosition();

        if (masiatrix.time_passed_movement >= 3000.0f) {
          masiatrix.time_passed_movement = 0.0f;

          direction = rand() % 8 + 1;

          float velocityOne = 75.0f;
          float velocityTwo = 50.0f;
          if(entity.component<ThreeD>()){
            float velocityOne = 20.0f;
            float velocityTwo = 12.0f;
          }

          switch (direction) {
            case 1 : next_velocity = glm::vec3(0.0f, velocityOne, 0.0f); break;
            case 2 : next_velocity = glm::vec3(velocityTwo, velocityTwo, 0.0f); break;
            case 3 : next_velocity = glm::vec3(velocityOne, 0.0f, 0.0f); break;
            case 4 : next_velocity = glm::vec3(velocityTwo, -velocityTwo, 0.0f); break;
            case 5 : next_velocity = glm::vec3(0.0f, -velocityTwo, 0.0f); break;
            case 6 : next_velocity = glm::vec3(-velocityTwo, -velocityTwo, 0.0f); break;
            case 7 : next_velocity = glm::vec3(-velocityOne, 0.0f, 0.0f); break;
            case 8 : next_velocity = glm::vec3(-velocityTwo, velocityTwo, 0.0f); break;
          }
          physics.velocity = next_velocity;
        }

        const float distancia = std::sqrt(
            std::pow(std::abs(player_position.x - masiatrix_position.x), 2) +
            std::pow(std::abs(player_position.y - masiatrix_position.y), 2));

        float safeDistance = 45.0f;
        if(entity.component<ThreeD>()){
          safeDistance = 17.0f;
        }

        if (distancia >= safeDistance) {
          if (masiatrix.time_passed_attack >= 1000.0f) {
            masiatrix.time_passed_attack = 0.0f;

            glm::vec3 vector_player_masiatrix(player_position.x - masiatrix_position.x,
                                       player_position.y - masiatrix_position.y,
                                       0.0f);
            glm::vec3 vector_masiatrix_v(0.0f, 1.0f, 0.0f);

            float angle_rad =
            std::atan2(vector_player_masiatrix.y - vector_masiatrix_v.y,
                       vector_player_masiatrix.x - vector_masiatrix_v.x);

            
            if(!entity.component<ThreeD>()){
              projectile_velocity = glm::normalize(player_position -
                                      masiatrix_position) * 115.0f;
              EntityFactory2D().MakeEnemyProjectile(es, masiatrix_position, angle_rad,
                                               projectile_velocity, "masiatrix");
            }
            else{
              projectile_velocity = glm::normalize(player_position -
                                      masiatrix_position) * 40.0f;
              EntityFactory3D().MakeEnemyProjectile(es, masiatrix_position, angle_rad,
                                               projectile_velocity, "masiatrix");
            }           
        }
      }    
  });
}

void MasiatrixBossFight::update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) {}

void MasiatrixBossFight::configure(entityx::EventManager &event_manager) {
  event_manager.subscribe<MasiatrixNextPhase>(*this);
}

void MasiatrixBossFight::receive(const MasiatrixNextPhase &nextPhase) {
  actual_phase = nextPhase.phase;
  glm::vec3 positioning;

  switch (actual_phase) {
    case 1 :
      if(!nextPhase.three_d){
        EntityFactory2D().MakeMasiatrix(*em, masiatrixInfo.positionA, "A", false);
        EntityFactory2D().MakeMasiatrix(*em, masiatrixInfo.positionB, "B", false);
        EntityFactory2D().MakeMasiatrix(*em, masiatrixInfo.positionC, "C", false);
        EntityFactory2D().MakeMasiatrix(*em, masiatrixInfo.positionD, "D", true);
      }
      else{
        EntityFactory3D().MakeMasiatrix(*em, masiatrixInfo.positionA, "A", false);
        EntityFactory3D().MakeMasiatrix(*em, masiatrixInfo.positionB, "B", false);
        EntityFactory3D().MakeMasiatrix(*em, masiatrixInfo.positionC, "C", false);
        EntityFactory3D().MakeMasiatrix(*em, masiatrixInfo.positionD, "D", true);
      }

      positioning = masiatrixInfo.positionC;
      em->each<Player, Transform>(
      [&](entityx::Entity entity, Player &player, Transform &player_transform) {
        if(nextPhase.three_d){
          glm::vec3 new_position = glm::vec3(positioning.x,positioning.y - 10.0f,7);
          player_transform.SetLocalPosition(new_position);
        }
        else{
          glm::vec3 new_position = glm::vec3(positioning.x,positioning.y - 25.0f,positioning.z);
          player_transform.SetLocalPosition(new_position);       
        }        
      });
    break;
    case 2 :
      if(!nextPhase.three_d){
        EntityFactory2D().MakeMasiatrix(*em, masiatrixInfo.positionA, "A", false);
        EntityFactory2D().MakeMasiatrix(*em, masiatrixInfo.positionB, "B", false);
        EntityFactory2D().MakeMasiatrix(*em, masiatrixInfo.positionC, "C", true);
        EntityFactory2D().MakeMasiatrix(*em, masiatrixInfo.positionD, "D", false);
      }
      else{
        EntityFactory3D().MakeMasiatrix(*em, masiatrixInfo.positionA, "A", false);
        EntityFactory3D().MakeMasiatrix(*em, masiatrixInfo.positionB, "B", false);
        EntityFactory3D().MakeMasiatrix(*em, masiatrixInfo.positionC, "C", true);
        EntityFactory3D().MakeMasiatrix(*em, masiatrixInfo.positionD, "D", false);
      }

      positioning = masiatrixInfo.positionC;
      em->each<Player, Transform>(
      [&](entityx::Entity entity, Player &player, Transform &player_transform) {
        if(nextPhase.three_d){
          glm::vec3 new_position = glm::vec3(positioning.x,positioning.y - 10.0f,7);
          player_transform.SetLocalPosition(new_position);
        }
        else{
          glm::vec3 new_position = glm::vec3(positioning.x,positioning.y - 60.0f,positioning.z);
          player_transform.SetLocalPosition(new_position);
        }
      });
    break;
  } 
}

int enemies_alive = 0;
void MenaxIaSystem::update(entityx::EntityManager &es,
                           entityx::EventManager &events,
                           entityx::TimeDelta dt) {
  enemies_alive = 0;
  glm::vec3 player_position;
  es.each<Player, Transform>(
      [&](entityx::Entity entity, Player &player, Transform &transform) {
        player_position = transform.GetWorldPosition();
      });

  es.each<Ghost>(
      [&](entityx::Entity entity, Ghost &ghost) {
        enemies_alive++;
      });
  es.each<Turret>(
      [&](entityx::Entity entity, Turret &turret) {
        enemies_alive++;
      });

  glm::vec3 new_velocity(0.0f, 0.0f, 0.0f);
  es.each<Menax, Transform, Physics>([&](entityx::Entity entity, Menax &menax, Transform &menax_transform, Physics &physics) {
    glm::vec3 menax_position;

    if (menax.comportamiento == Menax::Comportamiento::WAIT) {
      physics.velocity = glm::vec3(0.0f,0.0f,0.0f);
      if (enemies_alive == 0 && menax.spawn_enemies == false) {
        es.each<Player, Transform>(
          [&](entityx::Entity entity, Player &player, Transform &transform) {
            transform.SetLocalPosition(glm::vec3(menax.original_position.x, menax.original_position.y - 175.0f ,menax.original_position.z));
          });
        menax_transform.SetLocalPosition(glm::vec3(menax.original_position.x,menax.original_position.y - 75.0f,menax.original_position.z));
        menax.hits = 0;
        menax.comportamiento = Menax::Comportamiento::ATTACK;
        menax.timer = 0.0f;
      }
    } else {
      if (menax.hits >= 10) {
        menax_transform.SetLocalPosition(menax.original_position);
        menax.spawn_enemies = true;
        menax.hits = 0;
        menax.comportamiento = Menax::Comportamiento::WAIT;
      } else {
        // Movernos hacia Player y provocar animacion
        glm::vec3 new_velocity(0.0f, 0.0f, 0.0f);
        new_velocity = glm::normalize(player_position -
                                      menax_transform.GetWorldPosition()) * 60.0f;
        physics.velocity = new_velocity;
      }
    }
  });
}

int total_spawned = 0;
bool have_to_spawn = false;
void SpawnSystem::update(entityx::EntityManager &es,
                           entityx::EventManager &events,
                           entityx::TimeDelta dt) {
  es.each<Menax>(
      [&](entityx::Entity entity, Menax &menax) {
        have_to_spawn = menax.spawn_enemies;
      });

  es.each<Spawn, Transform>(
      [&](entityx::Entity entity, Spawn &spawn, Transform &transform) {     
        if (have_to_spawn) {
          spawn.timer += (dt * 500.0f);
          if (spawn.timer >= 500.0f) {
            if (spawn.spawned == 4) {
              spawn.timer = 0.0f;
              EntityFactory2D().MakeTurret(es,glm::vec3(transform.GetWorldPosition().x,transform.GetWorldPosition().y - 10.0f,transform.GetWorldPosition().z), 500.0f);
              spawn.spawned++;
              total_spawned++;
            } else {
              spawn.timer = 0.0f;
              EntityFactory2D().MakeGhost(es,glm::vec3(transform.GetWorldPosition().x,transform.GetWorldPosition().y - 10.0f,transform.GetWorldPosition().z));
              spawn.spawned++;
              total_spawned++;
            }      
          }
          if (spawn.spawned == 5 && total_spawned == 10) {
            have_to_spawn = false;
            spawn.timer = 0.0f;
            spawn.spawned=0;
            total_spawned = 0;
            es.each<Menax>(
              [&](entityx::Entity entity, Menax &menax) {
                menax.spawn_enemies = false;
            }); 
          }
        }
  });
}

bool finished = false;
void MenaxAnimationSystem::update(entityx::EntityManager &es,
                           entityx::EventManager &events,
                           entityx::TimeDelta dt) {
  entityx::ComponentHandle<Menax> menax;
  entityx::ComponentHandle<Transform> transform;
  entityx::ComponentHandle<Physics> physics;
  entityx::ComponentHandle<SpriteAnimation> animation;
  
  std::string animToPlay;
  std::string animToPlayHit;

  for (entityx::Entity e0 : es.entities_with_components(
           menax, transform, physics, animation)) {
    
    if (menax->comportamiento == Menax::Comportamiento::WAIT) {
        Engine::GetInstance().Get<AudioManager>().PlaySound(
            "assets/media/fx/menax/default/laugh.wav", false, 0.5f);
        animToPlay = "waiting";
        animation->Play(animToPlay);
        animToPlayHit = "dontDoDamage";
        menax->hitBox.component<SpriteAnimation>()->Play(animToPlayHit);
    } else{
      if (physics->velocity.y < 0 || physics->velocity.y > 0 || physics->velocity.x > 0 || physics->velocity.x < 0) {
        menax->timer += (dt*175.0f);
        animToPlay = "moving_bottom";
        animation->Play(animToPlay);
        animToPlayHit = "dontDoDamage";
        menax->hitBox.component<SpriteAnimation>()->Play(animToPlayHit);
        if (menax->timer >= 350.0f) {
          Engine::GetInstance().Get<AudioManager>().PlaySound(
            "assets/media/fx/menax/default/mov.wav", false, 1);
          menax->hitBox.component<MenaxHitBox>()->stomp = true;
          animToPlayHit = "doDamage";
          menax->hitBox.component<SpriteAnimation>()->Play(animToPlayHit);
          if (menax->timer >= 525.0f) {
            menax->timer = 0.0f;
            menax->hitBox.component<MenaxHitBox>()->stomp = false;
            menax->hitBox.component<MenaxHitBox>()->damaged = false;
            animToPlayHit = "dontDoDamage";
            menax->hitBox.component<SpriteAnimation>()->Play(animToPlayHit);
          }   
        }
      } 
    } 
  }
}

void MenaxAttackSystem::configure(entityx::EventManager &event_manager) {
  event_manager.subscribe<Collision>(*this);
}

void MenaxAttackSystem::receive(const Collision &collision) {
  auto collision_copy = collision;
  if (!collision_copy.e0.valid() || !collision_copy.e1.valid()) {
    return;
  }
  auto e0_projectile = collision_copy.e0.component<MenaxHitBox>();
  auto e1_projectile = collision_copy.e1.component<MenaxHitBox>();

  if (e0_projectile && e0_projectile->stomp && !e0_projectile->damaged && collision_copy.e1.component<Player>()){
    e0_projectile->damaged = true;
    auto e1_health = collision_copy.e1.component<Health>();
    e1_health->hp -= e0_projectile->damage;

    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/gaunt/default/hit.wav", false, 1);
    if(!collision_copy.e1.component<ThreeD>()){
      auto e1_color_animation = collision_copy.e1.component<ColorAnimation>();
      e1_color_animation->Play();
    }

  } else if (e1_projectile && e1_projectile->stomp && !e1_projectile->damaged && collision_copy.e0.component<Player>()) {
    e1_projectile->damaged = true;
    auto e0_health = collision_copy.e0.component<Health>();
    e0_health->hp -= e1_projectile->damage;

    Engine::GetInstance().Get<AudioManager>().PlaySound(
        "assets/media/fx/gaunt/default/hit.wav", false, 1);
    if(!collision_copy.e0.component<ThreeD>()){
      auto e0_color_animation = collision_copy.e0.component<ColorAnimation>();
      e0_color_animation->Play();
    }
  } 
}
void MenaxAttackSystem::update(entityx::EntityManager &es,
                                entityx::EventManager &events,
                                entityx::TimeDelta dt) {}