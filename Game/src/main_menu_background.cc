#include "main_menu_background.h"

#include <engine/components/common/camera.h>
#include <engine/components/common/transform.h>
#include <engine/components/two_d/sprite_animation.h>
#include <engine/components/two_d/texture.h>
#include <engine/core/engine.h>
#include <engine/core/resource_manager.h>
#include <engine/systems/two_d/sprite_animator.h>
#include <engine/systems/two_d/sprite_renderer.h>

#include "entity_factory.h"

using namespace engine::core;
using namespace engine::components::common;
using namespace engine::components::two_d;

MainMenuBackground::MainMenuBackground() {
  // adding entities
  auto menu = entities.create();
  menu.assign<Transform>(glm::vec3(960.0f / 2.0f, 540.0f / 2.0f, 0), nullptr,
                         glm::vec3(960.0f / 500.0f, 540.0f / 477.0f, 1.0f));

  auto camera = entities.create();
  camera.assign<Transform>(glm::vec3(960.0f / 2.0f, 540.0f / 2.0f, 1));
  camera.assign<Camera>(960.0f, 540.0f, 0.1f, 1000.0f);

  std::vector<engine::utils::Rectangle> rect;
  rect.emplace_back(glm::vec2(0, 0), glm::vec2(500, 477));
  rect.emplace_back(glm::vec2(500, 0), glm::vec2(500, 477));
  rect.emplace_back(glm::vec2(1000, 0), glm::vec2(500, 477));
  rect.emplace_back(glm::vec2(1500, 0), glm::vec2(500, 477));
  auto texture = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/menu/background_menu.png");
  SpriteAnimation::AnimationClip background_anim("background_menu", texture,
                                                 rect, 100.0f);
  SpriteAnimation anim({background_anim});
  menu.assign<SpriteAnimation>(std::move(anim));
  menu.assign<Sprite>(texture);

  auto logo = entities.create();
  logo.assign<Transform>(glm::vec3(910, 50, 0), nullptr,
                         glm::vec3(0.1, 0.1, 1));
  auto logo_tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/menu/patan_games.png");
  logo.assign<Sprite>(logo_tex);

  // adding systems
  systems.add<engine::systems::two_d::SpriteAnimator>();
  systems.add<engine::systems::two_d::SpriteRenderer>();
  systems.configure();
}

void MainMenuBackground::Update(entityx::TimeDelta dt) {
  systems.update<engine::systems::two_d::SpriteAnimator>(dt);
  systems.update<engine::systems::two_d::SpriteRenderer>(dt);
}
