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
#include "components.h"
#include "text.h"
#include "text_factory.h"

#include "floor_factory.h"

#include <fstream>
#include <string>

using namespace engine::core;
using namespace std;

Game::Game()
    : new_game(true), current_state_(State::kMainMenu), next_state_(State::kNull), scenes_() {
  std::string filename = "assets/config/opciones.txt";
  std::ofstream outfile;
  outfile.open(filename, std::ofstream::out | std::ofstream::trunc);
  outfile << "1 1 1" << std::endl;
  outfile.close();
  text_to_play = "Bienvenido.\nEste es Gauntleto, esta furioso porque el malvado Lord Menax le ha robado sus tartas.\nWASD haran que Gauntleto se mueva.\nLas flechas de direccion haran que Gauntleto ataque.\nSi ademas de las flechas pulsas espacio, se defendera.\nCorre a detener al malvado Lord Menax y sus secuaces!\nPulsa [ENTER] para continuar.";
    
  events.subscribe<CharSelect>(*this);
  events.subscribe<OptionMenu>(*this);
  events.subscribe<BackToMainMenu>(*this);
  events.subscribe<StartGame>(*this);
  events.subscribe<Death>(*this);
  events.subscribe<PauseMenuEvent>(*this);
  events.subscribe<BackToGame>(*this);
  events.subscribe<StartLevel2>(*this);
  events.subscribe<PlayText>(*this);

  scenes_.emplace_back(new MainMenuBackground());
  scenes_.emplace_back(new MainMenu(this));
  Engine::GetInstance().Get<AudioManager>().PlaySound(
      "assets/media/music/gauntleto_theme_v2.wav", true, 0.7f);
}

void Game::Update(entityx::TimeDelta dt) {
  if (next_state_ != State::kNull) {
    switch (next_state_) {
      case State::kMainMenu:
        new_game = true;
        scenes_.emplace_back(new MainMenuBackground());
        scenes_.emplace_back(new MainMenu(this));
        Engine::GetInstance().Get<AudioManager>().PlaySound(
            "assets/media/music/gauntleto_theme_v2.wav", true, 0.7f);
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
        Engine::GetInstance().Get<AudioManager>().StopMusic();
        break;
      case State::kPauseMenu:
        scenes_.push_back(std::make_unique<PauseMenu>(this));
        break;
      case State::kText:
        scenes_.push_back(std::make_unique<Text>(this,text_to_play, text_to_play.substr(0,4)));
        scenes_.front()->events.emit<PauseGameEvent>();
        break;
      case State::kFloor1:
        if (new_game){
          new_game = false;
          Engine::GetInstance().Get<AudioManager>().StopAllSounds();
          Engine::GetInstance().Get<AudioManager>().
            PlaySound("assets/media/music/level_one_v2.wav",true, 0.3f);
          scenes_.clear();
          if (character.role == Characters::Role::KNIGHT) {
            scenes_.push_back(
             FloorFactory::MakeFloorOne2D("assets/castle/floor1.tmx", this, "knight"));
          } else scenes_.push_back(
             FloorFactory::MakeFloorOne2D("assets/castle/floor1.tmx", this, "wizard"));
          
          scenes_.push_back(std::make_unique<GameUi>(this));
          //text_to_play = "Bienvenido.\nEste es Gauntleto, esta furioso porque el malvado Lord Menax le ha robado sus tartas.\nWASD haran que Gauntleto se mueva.\nLas flechas de direccion haran que Gauntleto ataque.\nSi ademas de las flechas pulsas espacio, se defendera.\nCorre a detener al malvado Lord Menax y sus secuaces!\nPulsa [ENTER] para continuar.";
          //scenes_.push_back(std::make_unique<Text>(this,text_to_play));
          //PlayText pt("Bienvenido.\nEste es Gauntleto, esta furioso porque el malvado Lord Menax le ha robado sus tartas.\nWASD haran que Gauntleto se mueva.\nLas flechas de direccion haran que Gauntleto ataque.\nSi ademas de las flechas pulsas espacio, se defendera.\nCorre a detener al malvado Lord Menax y sus secuaces!\nPulsa [ENTER] para continuar.");
          //next_state_ = State::kText;
          //scenes_.front()->events.emit<PlayText>(pt);
          scenes_.push_back(std::make_unique<Text>(this,text_to_play,"bienvenido"));
          scenes_.front()->events.emit<PauseGameEvent>();
        } else {
          scenes_.pop_back(); 
          scenes_.front()->events.emit<BackToGame>();
        }
        break;
      case State::kFloor2:
         Engine::GetInstance().Get<AudioManager>().StopMusic();
         scenes_.clear();
         if (character.role == Characters::Role::KNIGHT) {
            scenes_.push_back(
             FloorFactory::MakeFloorOne2D("assets/castle/floor2.tmx", this, "knight"));
          } else scenes_.push_back(
             FloorFactory::MakeFloorOne2D("assets/castle/floor2.tmx", this, "wizard"));
         scenes_.push_back(std::make_unique<GameUi>(this));
        break;
      case State::kFloor3:
           Engine::GetInstance().Get<AudioManager>().StopAllSounds();
           // Engine::GetInstance().Get<AudioManager>().
           //  PlaySound("assets/media/music/level_one_v2.wav",true, 0.3);
           scenes_.clear();
           // scenes_.push_back(FloorFactory3D::MakeFloor1(this));
           if (character.role == Characters::Role::KNIGHT) {
            scenes_.push_back(
             FloorFactory::MakeFloorOne2D("assets/castle/floor3.tmx", this, "knight"));
          } else scenes_.push_back(
             FloorFactory::MakeFloorOne2D("assets/castle/floor3.tmx", this, "wizard"));
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

void Game::receive(const StartGame& event, const Characters &c) { 
  next_state_ = State::kFloor1;
  character = c;
}
  
void Game::receive(const CharSelect& event) { 
  next_state_ = State::kCharSelMenu; }

void Game::receive(const StartLevel2& event) { next_state_ = State::kFloor2; }

void Game::receive(const OptionMenu& event) {
  next_state_ = State::kOptionsMenu;
}

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

void Game::receive(const PlayText& event){
  text_to_play = event.text;
  next_state_ = State::kText;  
}