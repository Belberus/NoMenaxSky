#include "pause_menu.h"

#include <engine/components/common/camera.h>
#include <engine/components/common/transform.h>
#include <engine/components/two_d/sprite.h>
#include <engine/core/audio_manager.h>
#include <engine/components/two_d/texture.h>
#include <engine/core/engine.h>
#include <engine/core/resource_manager.h>
#include <engine/systems/two_d/sprite_animator.h>
#include <engine/systems/two_d/sprite_renderer.h>

#include "entity_factory.h"
#include "systems.h"
#include "components.h"
#include "events.h"

using namespace engine::components::common;
using namespace engine::components::two_d;
using namespace engine::core;

PauseMenu::PauseMenu(Game* parent_scene) {
	parent_scene_ = parent_scene;

	parent_scene->events.subscribe<BackToMainMenu>(*this);
	parent_scene->events.subscribe<BackToGame>(*this);


}

void PauseMenu::Update(entityx::TimeDelta dt) {}

void PauseMenu::receive(const BackToMainMenu& event) {

}

void PauseMenu::receive(const BackToGame& event) {

}