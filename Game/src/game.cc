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
    : three_d(false), new_game(true), new_game2(true), new_game3(true), current_state_(State::kMainMenu), next_state_(State::kNull), scenes_() {
  text_to_play = "Este es Gauntleto, esta furioso porque el malvado Lord Menax y sus secuaces\nle han robado todas las tartas.\nRecorre el castillo, encuentra a Menax y a sus secuaces y vencelos para\nrecuperar las tartas!\n\nPara poder enfrentarte al boss de este nivel, deberas encontrar la llave.\nBusca en los dos cofres, uno debe contenerla.\n                    Pulsa [ENTER] para continuar.";
  level = 1;

  std::string filename = "assets/config/opciones.txt";

  std::fstream file(filename.c_str(), std::ios_base::in);

  int modo, musica, efectos;

  file >> modo >> musica >> efectos;
  file.close();

  if (!modo) {
    three_d = true;
  }

  if (!musica){
    Engine::GetInstance().Get<AudioManager>().SetVolumeMusic(0.0f);
  } 
  else Engine::GetInstance().Get<AudioManager>().SetVolumeMusic(1);

  if (!efectos){
    Engine::GetInstance().Get<AudioManager>().SetVolumeFX(0.0f);
  } 
  else Engine::GetInstance().Get<AudioManager>().SetVolumeMusic(1);

  events.subscribe<CharSelect>(*this);
  events.subscribe<OptionMenu>(*this);
  events.subscribe<BackToMainMenu>(*this);
  events.subscribe<StartGame>(*this);
  events.subscribe<Death>(*this);
  events.subscribe<PauseMenuEvent>(*this);
  events.subscribe<BackToGame>(*this);
  events.subscribe<StartLevel1>(*this);
  events.subscribe<StartLevel2>(*this);
  events.subscribe<StartLevel3>(*this);
  events.subscribe<PlayText>(*this);
  events.subscribe<SetThreeD>(*this);
  scenes_.emplace_back(new MainMenuBackground());
  scenes_.emplace_back(new MainMenu(this));
  Engine::GetInstance().Get<AudioManager>().PlaySound(
      "assets/media/music/gauntleto_theme_v2.wav", true, 0.7f);
}

void Game::Update(entityx::TimeDelta dt) {
  if (next_state_ != State::kNull) {
    current_state_ = next_state_;
    switch (next_state_) {
      case State::kMainMenu:
        scenes_.clear();
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
        scenes_.push_back(std::make_unique<DeathMenu>(this, character));
        Engine::GetInstance().Get<AudioManager>().StopMusic();
        break;
      case State::kPauseMenu:
        scenes_.push_back(std::make_unique<PauseMenu>(this,three_d));
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
          if(!three_d){
            scenes_.push_back(
              FloorFactory::MakeFloorOne2D("assets/castle/floor1.tmx", this, character));
          }
          else{
            scenes_.push_back(FloorFactory::MakeFloorOne3D("daigual",this, character));
          }
          scenes_.push_back(std::make_unique<GameUi>(this));
          scenes_.push_back(std::make_unique<Text>(this,text_to_play,"bienvenido"));
          SetThreeD std(true);
          scenes_.front()->events.emit<PauseGameEvent>();
          LevelEvent lt(1);
          scenes_.front()->events.emit<LevelEvent>(lt);
        } else {
          scenes_.pop_back(); 
          scenes_.front()->events.emit<BackToGame>();
        }
        break;
      case State::kFloor2:
        if(new_game2){
          new_game2 = false;
          Engine::GetInstance().Get<AudioManager>().StopMusic();
          Engine::GetInstance().Get<AudioManager>().
            PlaySound("assets/media/music/level_one_v2.wav",true, 0.3f);
          scenes_.clear();
          if(!three_d){
            scenes_.push_back(
              FloorFactory::MakeFloorTwo2D("assets/castle/floor2.tmx", this, character));
          }
          else{
            scenes_.push_back(FloorFactory::MakeFloorTwo3D("daigual",this, character));
          }
          scenes_.push_back(std::make_unique<GameUi>(this));
          text_to_play = "Cuidado por donde pisas! Este nivel del castillo esta lleno de trampas.\n\nPara acceder a la habitacion del boss deberas buscar dos palancas por las\n\nhabitaciones y activarlas.\n\n\n                    Pulsa [ENTER] para continuar.";
          scenes_.push_back(std::make_unique<Text>(this,text_to_play,"bienvenido2"));
          scenes_.front()->events.emit<PauseGameEvent>();
          LevelEvent lt(2);
          scenes_.front()->events.emit<LevelEvent>(lt);
        }
        else{
          scenes_.pop_back(); 
          scenes_.front()->events.emit<BackToGame>();
        }        
        break;
      case State::kFloor3:
        if(new_game3){
          new_game3 = false;
          Engine::GetInstance().Get<AudioManager>().StopAllSounds();
          Engine::GetInstance().Get<AudioManager>().
           PlaySound("assets/media/music/level_one_v2.wav",true, 0.3);
          scenes_.clear();
          if(three_d){
            scenes_.push_back(
               FloorFactory::MakeFloorThree2D("assets/castle/floor3.tmx", this, character));
          }
          else{
            scenes_.push_back(FloorFactory::MakeFloorThree3D("daigual",this, character));
          }
          scenes_.push_back(std::make_unique<GameUi>(this));
          text_to_play = "El ultimo nivel del castillo, el nivel de Lord Menax.\n\nPreparate para enfrentarte a el, sera un duro combate.\n\n\n\n\n                    Pulsa [ENTER] para continuar.";
          scenes_.push_back(std::make_unique<Text>(this,text_to_play,"bienvenido3"));
          scenes_.front()->events.emit<PauseGameEvent>();
          LevelEvent lt(3);
          scenes_.front()->events.emit<LevelEvent>(lt);
        }
        else{
          scenes_.pop_back(); 
          scenes_.front()->events.emit<BackToGame>();
        }
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
  if(level == 1){
    new_game = true;
    next_state_ = State::kFloor1;
  }
  else if(level == 2){
    new_game2 = true;
    next_state_ = State::kFloor2;
  }
  else if(level == 3){
    new_game3 = true;
    next_state_= State::kFloor3;
  }
  else next_state_ = State::kNull; //nunca deberia llegar!
  character = event.text;
}
  
void Game::receive(const CharSelect& event) { 
  next_state_ = State::kCharSelMenu; }

void Game::receive(const StartLevel1& event) { 
  level = 1;
  new_game = true;
  next_state_ = State::kFloor1; 
}

void Game::receive(const StartLevel2& event) { 
  level = 2;
  new_game2 = true;
  next_state_ = State::kFloor2; 
}

void Game::receive(const StartLevel3& event) {
  level = 3;
  new_game3 = true;
  next_state_ = State::kFloor3;
}

void Game::receive(const OptionMenu& event) {
  next_state_ = State::kOptionsMenu;
}

void Game::receive(const BackToMainMenu& event) {
  level = 1;
  next_state_ = State::kMainMenu;
}

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

void Game::receive(const SetThreeD& setThreeD){
  three_d = setThreeD.three_d;  
}

int Game::getLevel(){
  return level;
}