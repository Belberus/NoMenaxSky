#include "options_menu.h"

#include <engine/components/common/camera.h>
#include <engine/components/common/transform.h>

#include "systems.h"

using namespace engine::components::common;

OptionsMenu::OptionsMenu(engine::core::Scene *parent_scene) 
	: parent_scene_(parent_scene) {
  
  auto camera = entities.create();
  camera.assign<Transform>(glm::vec3(960.0f / 2.0f, 540.0f / 2.0f, 1));
  camera.assign<Camera>(960.0f, 540.0f, 0.1f, 1000.0f);

  systems.add<OptionsInputSystem>();
  systems.configure();

}

void OptionsMenu::Update(entityx::TimeDelta dt) {
  systems.update<OptionsInputSystem>(dt);
}
