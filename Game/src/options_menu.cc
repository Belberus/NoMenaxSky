#include "options_menu.h"

#include <engine/components/common/camera.h>
#include <engine/components/common/transform.h>
#include <engine/components/two_d/sprite_animation.h>
#include <engine/components/two_d/sprite.h>
#include <engine/core/audio_manager.h>
#include <engine/components/two_d/texture.h>
#include <engine/core/engine.h>
#include <engine/core/resource_manager.h>
#include <engine/systems/two_d/sprite_animator.h>
#include <engine/systems/two_d/sprite_renderer.h>

#include "entity_factory.h"
#include "systems.h"
#include "components.h"
#include "events.h"

#include <fstream>

using namespace engine::components::common;
using namespace engine::components::two_d;
using namespace engine::core;


OptionsMenu::OptionsMenu(engine::core::Scene *parent_scene) 
  : parent_scene_(parent_scene) {


  events.subscribe<BackToMainMenu>(*this);

  auto camera = entities.create();
  camera.assign<Transform>(glm::vec3(960.0f / 2.0f, 540.0f / 2.0f, 1));
  camera.assign<Camera>(960.0f, 540.0f, 0.1f, 1000.0f);

  auto menu_canvas = entities.create();
  menu_canvas.assign<Transform>(glm::vec3(960.0f / 2.0f, 100.0f, 0.0f));
  auto menu_canvas_transform = &(*menu_canvas.component<Transform>());

  entityx::Entity exit_option = entities.create();
  exit_option.assign<Transform>(glm::vec3(0, 0, 0), menu_canvas_transform);
  auto tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/menu/volver.png");
  exit_option.assign<Sprite>(tex);

  entityx::Entity game_mode = entities.create();
  game_mode.assign<Transform>(glm::vec3(-250, 210, 0), menu_canvas_transform);
  tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/option_menu/modo.png");
  game_mode.assign<Sprite>(tex);

  entityx::Entity game_music = entities.create();
  game_music.assign<Transform>(glm::vec3(-250, 140, 0), menu_canvas_transform);
  tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/option_menu/musica.png");
  game_music.assign<Sprite>(tex);

  entityx::Entity game_fx = entities.create();
  game_fx.assign<Transform>(glm::vec3(-250, 70, 0), menu_canvas_transform);
  tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/option_menu/efectos.png");
  game_fx.assign<Sprite>(tex);

  // title
  entityx::Entity title = entities.create();
  title.assign<Transform>(glm::vec3(0, 300, 0), menu_canvas_transform,
                          glm::vec3(0.3, 0.3, 1));
  tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/option_menu/opciones.png");
  title.assign<Sprite>(tex);

  //////////////////////////////////////////////////////////////////
  ////////////////////   GAME OPTIONS //////////////////////////////
  //////////////////////////////////////////////////////////////////

  // TO DO: Si estan a 1 es ON y 2D, 0 son OFF y 3D
  //    y en systems cuando sale guardarlo en un fichero, y cuando cierra
  //  el juego borrar dicho fichero
  std::string filename = "assets/config/opciones.txt";

  GameOptions opciones_iniciales = {
      GameOptions::Modo::TWO_D, 
      GameOptions::Musica::MUSIC_ON,
      GameOptions::Efectos::FX_ON 
    };

  std::fstream file(filename.c_str(), std::ios_base::in);

  file >> modo >> musica >> efectos;

  file.close();

  if (!modo) opciones_iniciales.modo = GameOptions::Modo::THREE_D;
  if (!musica){
    opciones_iniciales.musica = GameOptions::Musica::MUSIC_OFF;
    Engine::GetInstance().Get<AudioManager>().SetVolumeMusic(0.0f);
  } 
  else Engine::GetInstance().Get<AudioManager>().SetVolumeMusic(1);
  if (!efectos){
    opciones_iniciales.efectos = GameOptions::Efectos::FX_OFF;
    Engine::GetInstance().Get<AudioManager>().SetVolumeFX(0.0f);
  } 
  else Engine::GetInstance().Get<AudioManager>().SetVolumeMusic(1);

  std::shared_ptr<Texture> tex2 = nullptr;

  /// MODO DE JUEGO
  entityx::Entity option_2d = entities.create();
  option_2d.assign<Transform>(glm::vec3(-50, 210, 0), menu_canvas_transform);

  entityx::Entity option_3d = entities.create();
  option_3d.assign<Transform>(glm::vec3(100, 210, 0), menu_canvas_transform);
  
  if (modo) { 
    tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/option_menu/2d_seleccionado.png");
    tex2 = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/option_menu/3d_noseleccionado.png");
    SetThreeD std(false);
    parent_scene_->events.emit<SetThreeD>(std);
  } else {
    tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/option_menu/2d_noseleccionado.png");
    tex2 = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/option_menu/3d_seleccionado.png");
    SetThreeD std(true);
    parent_scene_->events.emit<SetThreeD>(std);
  }

  option_2d.assign<Sprite>(tex);
  option_3d.assign<Sprite>(tex2);

  option_2d.assign<D2Mode>();
  option_3d.assign<D3Mode>();

  // MUSICA
  entityx::Entity option_on_music = entities.create();
  option_on_music.assign<Transform>(glm::vec3(-50, 140, 0), menu_canvas_transform);

  entityx::Entity option_off_music = entities.create();
  option_off_music.assign<Transform>(glm::vec3(100, 140, 0), menu_canvas_transform);

  if (musica) { 
    tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/option_menu/on_seleccionado.png");
    tex2 = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/option_menu/off_noseleccionado.png");
    Engine::GetInstance().Get<AudioManager>().SetVolumeMusic(1);
  } else {
    tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/option_menu/on_noseleccionado.png");
    tex2 = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/option_menu/off_seleccionado.png");
    Engine::GetInstance().Get<AudioManager>().SetVolumeMusic(0.0f);
  }

  
  option_on_music.assign<Sprite>(tex);
  option_off_music.assign<Sprite>(tex2);
 
  option_on_music.assign<MusicOn>();
  option_off_music.assign<MusicOff>();

  // EFECTOS
  entityx::Entity option_on_fx = entities.create();
  option_on_fx.assign<Transform>(glm::vec3(-50, 70, 0), menu_canvas_transform);
  
  entityx::Entity option_off_fx = entities.create();
  option_off_fx.assign<Transform>(glm::vec3(100, 70, 0), menu_canvas_transform);

  if (efectos) { 
    tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/option_menu/on_seleccionado.png");
    tex2 = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/option_menu/off_noseleccionado.png");
    Engine::GetInstance().Get<AudioManager>().SetVolumeFX(1);
  } else {
    tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/option_menu/on_noseleccionado.png");
    tex2 = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/option_menu/off_seleccionado.png");
    Engine::GetInstance().Get<AudioManager>().SetVolumeFX(0.0f);
  }
 
  option_on_fx.assign<Sprite>(tex);
  option_off_fx.assign<Sprite>(tex2);

  option_on_fx.assign<FXOn>();
  option_off_fx.assign<FXOff>();

  // ARROW
  entityx::Entity menuArrow = entities.create();
  if (modo){
    menuArrow.assign<Transform>(glm::vec3(-90, 210, 0), menu_canvas_transform,
                              glm::vec3(2, 2, 1));
  } else {
    menuArrow.assign<Transform>(glm::vec3(60, 210, 0), menu_canvas_transform,
                              glm::vec3(2, 2, 1));
  }
  
  tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/menu/ppc_front.png");
  menuArrow.assign<Sprite>(tex);

  menuArrow.assign<ArrowOptions>(ArrowOptions::Option::MODE);

  menuArrow.assign<GameOptions>(opciones_iniciales);

  ///////////////////////////////////////////////////////////////
  ////////////////////// END GAME OPTIONS ///////////////////////
  ///////////////////////////////////////////////////////////////

  systems.add<engine::systems::two_d::SpriteAnimator>();
  systems.add<engine::systems::two_d::SpriteRenderer>();
  systems.add<OptionsInputSystem>();
  systems.configure();

}

void OptionsMenu::Update(entityx::TimeDelta dt) {
  
  entityx::ComponentHandle<GameOptions> options;

  entityx::ComponentHandle<Sprite> sprite;
  entityx::ComponentHandle<D2Mode> modo_2d;
  entityx::ComponentHandle<D3Mode> modo_3d;
  entityx::ComponentHandle<MusicOn> music_on;
  entityx::ComponentHandle<MusicOff> music_off;
  entityx::ComponentHandle<FXOn> fx_on;
  entityx::ComponentHandle<FXOff> fx_off;

  for (entityx::Entity e :
      entities.entities_with_components(options)) {
    
    if ((options->modo == GameOptions::Modo::TWO_D) && modo == 0){
      // cambiar sprite en modo, resaltar 2d, transparentar 3d
      for (entityx::Entity e2 :
        entities.entities_with_components(sprite, modo_2d)){
        e2.replace<Sprite>(Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
                    "assets/option_menu/2d_seleccionado.png"));
      }

      for (entityx::Entity e3 :
        entities.entities_with_components(sprite, modo_3d)){
        e3.replace<Sprite>(Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
                    "assets/option_menu/3d_noseleccionado.png"));
      }
      //std::cout << "swapped" << std::endl;
      SetThreeD std(false);
      parent_scene_->events.emit<SetThreeD>(std);
      modo = 1;
    }

    if ((options->modo == GameOptions::Modo::THREE_D) && modo == 1){
      // cambiar sprite en modo, resaltar 2d, transparentar 3d
      for (entityx::Entity e2 :
        entities.entities_with_components(sprite, modo_2d)){
        e2.replace<Sprite>(Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
                    "assets/option_menu/2d_noseleccionado.png"));
      }

      for (entityx::Entity e3 :
        entities.entities_with_components(sprite, modo_3d)){
        e3.replace<Sprite>(Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
                    "assets/option_menu/3d_seleccionado.png"));
      }
      //std::cout << "swapped 2" << std::endl;
      SetThreeD std(true);
      parent_scene_->events.emit<SetThreeD>(std);
      modo = 0;
    }

    if ((options->musica == GameOptions::Musica::MUSIC_ON) && musica == 0){
      for (entityx::Entity e2 :
        entities.entities_with_components(sprite, music_on)){
        e2.replace<Sprite>(Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
                    "assets/option_menu/on_seleccionado.png"));
        Engine::GetInstance().Get<AudioManager>().SetVolumeMusic(1);
      }

      for (entityx::Entity e3 :
        entities.entities_with_components(sprite, music_off)){
        e3.replace<Sprite>(Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
                    "assets/option_menu/off_noseleccionado.png"));
        Engine::GetInstance().Get<AudioManager>().SetVolumeMusic(1);
      }
      
      musica = 1;
    }

    if ((options->musica == GameOptions::Musica::MUSIC_OFF) && musica == 1){
      for (entityx::Entity e2 :
        entities.entities_with_components(sprite, music_on)){
        e2.replace<Sprite>(Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
                    "assets/option_menu/on_noseleccionado.png"));
        Engine::GetInstance().Get<AudioManager>().SetVolumeMusic(0.0f);
      }

      for (entityx::Entity e3 :
        entities.entities_with_components(sprite, music_off)){
        e3.replace<Sprite>(Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
                    "assets/option_menu/off_seleccionado.png"));
        Engine::GetInstance().Get<AudioManager>().SetVolumeMusic(0.0f);
      }
      
      musica = 0;
    }

    if ((options->efectos == GameOptions::Efectos::FX_ON) && efectos == 0){
      for (entityx::Entity e2 :
        entities.entities_with_components(sprite, fx_on)){
        e2.replace<Sprite>(Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
                    "assets/option_menu/on_seleccionado.png"));
        Engine::GetInstance().Get<AudioManager>().SetVolumeFX(1);
      }

      for (entityx::Entity e3 :
        entities.entities_with_components(sprite, fx_off)){
        e3.replace<Sprite>(Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
                    "assets/option_menu/off_noseleccionado.png"));
        Engine::GetInstance().Get<AudioManager>().SetVolumeFX(1);
      }
      
      efectos = 1;
    }

    if ((options->efectos == GameOptions::Efectos::FX_OFF) && efectos == 1){
      for (entityx::Entity e2 :
        entities.entities_with_components(sprite, fx_on)){
        e2.replace<Sprite>(Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
                    "assets/option_menu/on_noseleccionado.png"));
        Engine::GetInstance().Get<AudioManager>().SetVolumeFX(0.0f);
      }

      for (entityx::Entity e3 :
        entities.entities_with_components(sprite, fx_off)){
        e3.replace<Sprite>(Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
                    "assets/option_menu/off_seleccionado.png"));
        Engine::GetInstance().Get<AudioManager>().SetVolumeFX(0.0f);
      }
      
      efectos = 0;
    }
  } 


  systems.update<OptionsInputSystem>(dt);
  systems.update<engine::systems::two_d::SpriteAnimator>(dt);
  systems.update<engine::systems::two_d::SpriteRenderer>(dt);
}


void OptionsMenu::receive(const BackToMainMenu& back_to_main) {
  parent_scene_->events.emit<BackToMainMenu>(back_to_main);
}

