#include "pause_menu.h"

#include <engine/components/common/camera.h>
#include <engine/components/common/transform.h>
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

using namespace engine::components::common;
using namespace engine::components::two_d;
using namespace engine::core;

PauseMenu::PauseMenu(Game* parent_scene) 
	: parent_scene_(parent_scene) {

	events.subscribe<BackToMainMenu>(*this);
	events.subscribe<BackToGame>(*this);
	events.subscribe<MuteMusic>(*this);
	events.subscribe<MuteFx>(*this);

	// adding entities
	auto camera = entities.create();
	camera.assign<Transform>(glm::vec3(960.0f / 2.0f, 540.0f / 2.0f, 1));
	camera.assign<Camera>(960.0f, 540.0f, 0.1f, 1000.0f);

	auto menu_canvas = entities.create();
	menu_canvas.assign<Transform>(glm::vec3(960.0f / 2.0f, 100.0f, 0.0f));
	auto menu_canvas_transform = &(*menu_canvas.component<Transform>());

	// exit option
	entityx::Entity exit_option = entities.create();
	exit_option.assign<Transform>(glm::vec3(0, 00, 0), menu_canvas_transform);
	auto tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
	  "assets/menu/volver.png");
	exit_option.assign<Sprite>(tex);

	// mute/unmute music option
	music_option_ = entities.create();
	music_option_.assign<Transform>(glm::vec3(0, 70, 0), menu_canvas_transform);
	if(!Engine::GetInstance().Get<AudioManager>().getMusicMute()){
		tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
	  "assets/menu/mute_music.png");
		music_option_.assign<Sprite>(tex);
	}
	else{
		tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
	  "assets/menu/unmute_music.png");
		music_option_.assign<Sprite>(tex);
	}

	// mute/unmute fx option
	fx_option_ = entities.create();
	fx_option_.assign<Transform>(glm::vec3(0, 140, 0), menu_canvas_transform);
	if(!Engine::GetInstance().Get<AudioManager>().getFxMute()){
		std::cout << "FX no muted" << std::endl;
		tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
	  "assets/menu/mute_fx.png");
		fx_option_.assign<Sprite>(tex);
	}
	else{
		tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
	  "assets/menu/unmute_fx.png");
		fx_option_.assign<Sprite>(tex);
	}

	// play option
	entityx::Entity play_option = entities.create();
	play_option.assign<Transform>(glm::vec3(0, 210, 0), menu_canvas_transform);
	tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
	  "assets/menu/continuar.png");
	play_option.assign<Sprite>(tex);

	// Cambiar por juego pausado
	entityx::Entity title = entities.create();
	title.assign<Transform>(glm::vec3(0, 320, 0), menu_canvas_transform,
	                      glm::vec3(0.3, 0.3, 1));
	tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
	  "assets/menu/pausa.png");
	title.assign<Sprite>(tex);

	// arrow
	entityx::Entity menuArrow = entities.create();
	menuArrow.assign<Transform>(glm::vec3(-90, 210, 0), menu_canvas_transform,
	                          glm::vec3(2, 2, 1));
	tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
	  "assets/menu/ppc_front.png");
	menuArrow.assign<Sprite>(tex);
	menuArrow.assign<PauseOptions>(PauseOptions::Option::CONTINUAR);

	// adding systems
	systems.add<engine::systems::two_d::SpriteRenderer>();
	systems.add<PauseInputSystem>();
	systems.configure();


}

void PauseMenu::Update(entityx::TimeDelta dt) {
	systems.update<PauseInputSystem>(dt);
  	systems.update<engine::systems::two_d::SpriteRenderer>(dt);
}

void PauseMenu::receive(const BackToMainMenu& event) {
	Engine::GetInstance().Get<AudioManager>().StopAllSounds();
	parent_scene_->events.emit<BackToMainMenu>(event);
}

void PauseMenu::receive(const BackToGame& event) {
	parent_scene_->events.emit<BackToGame>(event);
}

void PauseMenu::receive(const MuteMusic& muteMusic){
	std::cout << "mute music" << std::endl;
	if(Engine::GetInstance().Get<AudioManager>().getMusicMute()){ //Estaba muted
		Engine::GetInstance().Get<AudioManager>()
		.SetVolumeMusic(1.0f);
		auto tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
	      "assets/menu/mute_music.png");
	  	music_option_.replace<Sprite>(tex);
	}
	else{
		Engine::GetInstance().Get<AudioManager>()
		.SetVolumeMusic(0.0f);
		auto tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
	      "assets/menu/unmute_music.png");
	  	music_option_.replace<Sprite>(tex);
	}
}

void PauseMenu::receive(const MuteFx& muteFx){
	
	if(Engine::GetInstance().Get<AudioManager>().getFxMute()){ //Estaba muted
		std::cout << "unmute fx" << std::endl;
		Engine::GetInstance().Get<AudioManager>()
		.SetVolumeFX(1.0f);
		auto tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
	      "assets/menu/mute_fx.png");
	  	fx_option_.replace<Sprite>(tex);
	}
	else{
		std::cout << "mute fx" << std::endl;
		Engine::GetInstance().Get<AudioManager>()
		.SetVolumeFX(0.0f);
		auto tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
	      "assets/menu/unmute_fx.png");
	  	fx_option_.replace<Sprite>(tex);
	}
}