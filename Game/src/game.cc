#include "game.h"
#include <engine/core/audio_manager.h>
#include <engine/core/engine.h>
#include "game_ui.h"
#include "main_menu.h"
#include "main_menu_background.h"

#include "floor_factory.h"

using namespace engine::core;

Game::Game()
    : current_state_(State::kMainMenu), next_state_(State::kNull), scenes_() {
  events.subscribe<StartGame>(*this);
  scenes_.emplace_back(new MainMenuBackground());
  scenes_.emplace_back(new MainMenu(this));
  Engine::GetInstance().Get<AudioManager>().PlaySound(
      "assets/media/music/gauntleto_theme_v2.wav", true, 1);
}

void Game::Update(entityx::TimeDelta dt) {
  if (next_state_ != State::kNull) {
    switch (next_state_) {
      case State::kMainMenu:
        break;
      case State::kOptionsMenu:
        break;
      case State::kFloor1:
      Engine::GetInstance().Get<AudioManager>().
        StopAllSounds();
      //Engine::GetInstance().Get<AudioManager>().
      //  PlaySound("assets/media/music/level_one_v2.wav",true, 0.3);
        scenes_.clear();
        scenes_.push_back(FloorFactory::MakeFloor1("assets/castle/floor1.tmx"));
        scenes_.push_back(std::make_unique<GameUi>(this));
        break;
      case State::kExit:
        break;
      case State::kNull:
        break;
      default:
        break;
    }
    next_state_ = State::kNull;
  }
  for (int i = 0; i < scenes_.size(); ++i) {
    scenes_[i]->Update(dt);
  }
}

void Game::receive(const StartGame& event) { next_state_ = State::kFloor1; }
