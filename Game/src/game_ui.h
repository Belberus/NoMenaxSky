#ifndef GAME_UI_H_
#define GAME_UI_H_

#include <engine/core/scene.h>

#include "game.h"
class GameUi : public engine::core::Scene,public entityx::Receiver<GameUi> {
 public:
  GameUi(Game *parent_scene);
  void Update(entityx::TimeDelta dt) override;
  void receive(const Health &health);
  void receive(const Energy &energy);
  void receive(const Player &player);
  private:
 	float init_x = 0;
	float init_pos = 0;
	float init_x_nrg = 0;
	float init_pos_nrg = 0;
};
 
#endif  // GAME_UI_H_