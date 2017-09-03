#include "game.h"
#include <engine/core/audio_manager.h>
#include <engine/core/engine.h>
#include "game_ui.h"
#include "main_menu.h"
#include "main_menu_background.h"
#include "options_menu.h"
#include "character_selection_menu.h"
#include "death_menu.h"
#include "pause_menu.h"

#include "floor_factory.h"

#include <fstream>

using namespace engine::core;

Game::Game()
    : new_game(true), current_state_(State::kMainMenu), next_state_(State::kNull), scenes_() {
  std::string filename = "assets/config/opciones.txt";
  std::ofstream outfile;
  outfile.open(filename, std::ofstream::out | std::ofstream::trunc);
  outfile << "1 1 1" << std::endl;
  outfile.close();
    
  events.subscribe<CharSelect>(*this);
  events.subscribe<OptionMenu>(*this);
  events.subscribe<BackToMainMenu>(*this);
  events.subscribe<StartGame>(*this);
  events.subscribe<Death>(*this);
  events.subscribe<PauseMenuEvent>(*this);
  events.subscribe<BackToGame>(*this);

  scenes_.emplace_back(new MainMenuBackground());
  scenes_.emplace_back(new MainMenu(this));
  Engine::GetInstance().Get<AudioManager>().PlaySound(
      "assets/media/music/gauntleto_theme_v2.wav", true, 1);
}

void Game::Update(entityx::TimeDelta dt) {
  if (next_state_ != State::kNull) {
    switch (next_state_) {
      case State::kMainMenu:
        new_game = true;
        scenes_.emplace_back(new MainMenuBackground());
        scenes_.emplace_back(new MainMenu(this));
        Engine::GetInstance().Get<AudioManager>().PlaySound(
            "assets/media/music/gauntleto_theme_v2.wav", true, 1);
        break;
      case State::kOptionsMenu:
        scenes_.pop_back();
        scenes_.push_back(std::make_unique<OptionsMenu>(this));
        break;
      case State::kCharSelMenu:
        scenes_.pop_back();
        scenes_.push_back(std::make_unique<CharacterSelectionMenu>(this));
        break;
      case State::kDeathMenu:
        scenes_.pop_back(); // Sacamos el floor
        scenes_.pop_back(); // Sacamos la UI
        scenes_.push_back(std::make_unique<DeathMenu>(this));
      case State::kPauseMenu:
        scenes_.push_back(std::make_unique<PauseMenu>(this));
        break;
      case State::kFloor1:
        if (new_game){
          new_game = false;
          Engine::GetInstance().Get<AudioManager>().StopMusic();
          Engine::GetInstance().Get<AudioManager>().
            PlaySound("assets/media/music/level_one_v2.wav",true, 0.3f);
          scenes_.clear();
          //scenes_.push_back(
          //    FloorFactory::MakeFloorOne3D("test/untitled.tmx", this));
          /*scenes_.push_back(
              FloorFactory::MakeFloorOne3D("test/untitled.tmx", this));
          */ 
          scenes_.push_back(
             FloorFactory::MakeFloorOne2D("assets/castle/floor1.tmx", this));
          scenes_.push_back(std::make_unique<GameUi>(this));
        } else {
          scenes_.pop_back();
          scenes_.front()->events.emit<BackToGame>();
        }
        
        break;
      case State::kFloor2:
        /* Engine::GetInstance().Get<AudioManager>().StopAllSounds();
         // Engine::GetInstance().Get<AudioManager>().
         //  PlaySound("assets/media/music/level_one_v2.wav",true, 0.3);
         scenes_.clear();
         // scenes_.push_back(FloorFactory3D::MakeFloor1(this));
         scenes_.push_back(
             FloorFactory::MakeFloor2("assets/castle/floor2.tmx", this));
         scenes_.push_back(std::make_unique<GameUi>(this));*/
        break;
      case State::kFloor3:
        /*   Engine::GetInstance().Get<AudioManager>().StopAllSounds();
           // Engine::GetInstance().Get<AudioManager>().
           //  PlaySound("assets/media/music/level_one_v2.wav",true, 0.3);
           scenes_.clear();
           // scenes_.push_back(FloorFactory3D::MakeFloor1(this));
           scenes_.push_back(
               FloorFactory::MakeFloor3("assets/castle/floor3.tmx", this));
           scenes_.push_back(std::make_unique<GameUi>(this));*/
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

void Game::receive(const StartGame& event) { 
  next_state_ = State::kFloor1; }
  
void Game::receive(const CharSelect& event) { 
  next_state_ = State::kCharSelMenu; }

void Game::receive(const OptionMenu& event) { 
  next_state_ = State::kOptionsMenu; }

void Game::receive(const BackToMainMenu& event) {
  next_state_ = State::kMainMenu; }

void Game::receive(const Death& event){
  next_state_ = State::kDeathMenu; }

void Game::receive(const PauseMenuEvent& event) {
  next_state_ = State::kPauseMenu;
}

void Game::receive(const BackToGame& event) {
  next_state_ = State::kFloor1;
}
