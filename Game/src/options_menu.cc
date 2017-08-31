#include "options_menu.h"

#include <engine/components/common/camera.h>
#include <engine/components/common/transform.h>
#include <engine/components/two_d/sprite_animation.h>
#include <engine/components/two_d/sprite.h>
#include <engine/components/two_d/texture.h>
#include <engine/core/engine.h>
#include <engine/core/resource_manager.h>
#include <engine/systems/two_d/sprite_animator.h>
#include <engine/systems/two_d/sprite_renderer.h>

#include "entity_factory.h"
#include "systems.h"
#include "components.h"

#include <fstream>

using namespace engine::components::common;
using namespace engine::components::two_d;
using namespace engine::core;


OptionsMenu::OptionsMenu(engine::core::Scene *parent_scene) 
	: parent_scene_(parent_scene) {

  auto camera = entities.create();
  camera.assign<Transform>(glm::vec3(960.0f / 2.0f, 540.0f / 2.0f, 1));
  camera.assign<Camera>(960.0f, 540.0f, 0.1f, 1000.0f);

  auto menu_canvas = entities.create();
  menu_canvas.assign<Transform>(glm::vec3(960.0f / 2.0f, 100.0f, 0.0f));
  auto menu_canvas_transform = &(*menu_canvas.component<Transform>());

  entityx::Entity exit_option = entities.create();
  exit_option.assign<Transform>(glm::vec3(0, 0, 0), menu_canvas_transform);
  auto tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/menu/salir.png");
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

  //////////////////////////////////////////////////////////////////
  ////////////////////   GAME OPTIONS //////////////////////////////
  //////////////////////////////////////////////////////////////////

  // TO DO: Si estan a 1 es ON y 2D, 0 son OFF y 3D
  //    y en systems cuando sale guardarlo en un fichero, y cuando cierra
  // 	el juego borrar dicho fichero
  std::string filename = "assets/config/prueba.txt";

  GameOptions opciones_iniciales = {
  		GameOptions::Modo::TWO_D, 
  		GameOptions::Musica::MUSIC_ON,
  		GameOptions::Efectos::FX_ON 
  	};

  std::ifstream infile(filename.c_str());
  bool exist = (infile.good()) ? true : false;

  modo = 1;  	// 2D = 1	3D = 0
  musica = 1; 	// ON = 1	OFF = 0
  efectos = 1;	// ON = 1	OFF = 0

  if (exist){

  	std::fstream file(filename.c_str(), std::ios_base::in);

  	file >> modo >> musica >> efectos;

  	file.close();

  	//getchar();

  	if (!modo) opciones_iniciales.modo = GameOptions::Modo::THREE_D;
  	if (!musica) opciones_iniciales.musica = GameOptions::Musica::MUSIC_OFF;
  	if (!efectos) opciones_iniciales.efectos = GameOptions::Efectos::FX_OFF;
  }

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
  } else {
  	tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/option_menu/2d_noseleccionado.png");
  	tex2 = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/option_menu/3d_seleccionado.png");
  }

  option_2d.assign<Sprite>(tex);
  option_3d.assign<Sprite>(tex2);

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
  } else {
  	tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/option_menu/off_seleccionado.png");
  	tex2 = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/option_menu/on_noseleccionado.png");
  }

  // EFECTOS
  option_on_music.assign<Sprite>(tex);
  option_off_music.assign<Sprite>(tex2);
 

  entityx::Entity option_on_fx = entities.create();
  option_on_fx.assign<Transform>(glm::vec3(-50, 70, 0), menu_canvas_transform);
  
  entityx::Entity option_off_fx = entities.create();
  option_off_fx.assign<Transform>(glm::vec3(100, 70, 0), menu_canvas_transform);

  if (efectos) { 
  	tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/option_menu/on_seleccionado.png");
  	tex2 = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/option_menu/off_noseleccionado.png");
  } else {
  	tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/option_menu/on_noseleccionado.png");
  	tex2 = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/option_menu/off_seleccionado.png");
  }
 
  option_on_fx.assign<Sprite>(tex);
  option_off_fx.assign<Sprite>(tex2);

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
  
 
  systems.update<OptionsInputSystem>(dt);
  systems.update<engine::systems::two_d::SpriteAnimator>(dt);
  systems.update<engine::systems::two_d::SpriteRenderer>(dt);
}

