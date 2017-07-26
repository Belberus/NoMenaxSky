#ifndef CHARACTER_SELECTION_MENU_H_
#define CHARACTER_SELECTION_MENU_H_
#include <engine/core/scene.h>
class CharacterSelectionMenu : public engine::core::Scene {
 public:
  void Update(entityx::TimeDelta dt) override;
};

#endif  // CHARACTER_SELECTION_MENU_H_