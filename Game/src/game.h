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
  void receive(const StartGame &event, const Characters &c);
  void receive(const StartLevel2 &event);
  void receive(const OptionMenu &event);
  void receive(const BackToMainMenu &event);
  void receive(const CharSelect &event);
  void receive(const Death &event);
  void receive(const PauseMenuEvent &event);
  void receive(const BackToGame &event);
  void receive(const PlayText &event);
 private:
  State current_state_;
  State next_state_;
  std::vector<std::unique_ptr<engine::core::Scene>> scenes_;
  Characters character;
  bool new_game;
  std::string text_to_play;
};
#endif  // GAME_H_
