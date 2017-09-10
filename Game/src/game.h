#ifndef GAME_H_
#define GAME_H_

#include <memory>
#include <vector>

#include <engine/core/scene.h>

#include "events.h"
#include "components.h"

class Game : public engine::core::Scene, public entityx::Receiver<Game> {
 public:
  enum class State {
    kMainMenu,
    kOptionsMenu,
    kPauseMenu,
    kCharSelMenu,
    kDeathMenu,
    kText,
    kFloor1,
    kFloor2,
    kFloor3,
    kExit,
    kNull
  };
  Game();
  void Update(entityx::TimeDelta dt) override;
  void receive(const StartGame &event);
  void receive(const StartLevel1 &event);
  void receive(const StartLevel2 &event);
  void receive(const StartLevel3 &event);
  void receive(const OptionMenu &event);
  void receive(const BackToMainMenu &event);
  void receive(const CharSelect &event);
  void receive(const Death &event);
  void receive(const PauseMenuEvent &event);
  void receive(const BackToGame &event);
  void receive(const PlayText &event);
  void receive(const SetThreeD &setThreeD);
  int getLevel();
 private:
  State current_state_;
  State next_state_;
  std::vector<std::unique_ptr<engine::core::Scene>> scenes_;
  std::string character;
  bool new_game;
  bool new_game2;
  bool new_game3;
  std::string text_to_play;
  int level;
  bool three_d;
};
#endif  // GAME_H_
