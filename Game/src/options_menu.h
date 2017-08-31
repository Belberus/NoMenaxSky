#ifndef OPTIONS_MENU_H_
#define OPTIONS_MENU_H_

#include <engine/core/scene.h>
#include <entityx/entityx.h>

#include "game.h"
#include "events.h"

class OptionsMenu : public engine::core::Scene {
 public:
  OptionsMenu(engine::core::Scene *parent_scene);
  void Update(entityx::TimeDelta dt) override;

  int modo, musica, efectos;

 private:
  engine::core::Scene *parent_scene_;
};
#endif  // OPTIONS_MENU_H_
