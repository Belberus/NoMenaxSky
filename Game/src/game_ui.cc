#include "game_ui.h"

#include <engine/components/common/camera.h>
#include <engine/components/common/transform.h>
#include <engine/components/two_d/sprite.h>
#include <engine/core/engine.h>
#include <engine/core/resource_manager.h>
#include <engine/systems/two_d/sprite_renderer.h>

#include "components.h"

using namespace engine::core;
using namespace engine::systems::two_d;
using namespace engine::components::two_d;
using namespace engine::components::common;
using namespace engine::components::two_d;
using namespace engine::utils;

GameUi::GameUi(Game* parent_scene) 
  : parent_scene_(parent_scene) {

  // adding entities
  auto gui_texture_atlas =
      Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
          "assets/spritesheets/gui.png");

  auto camera = entities.create();
  camera.assign<Transform>(glm::vec3(960.0f / 2.0f, 540.0f / 2.0f, 1.0f));
  camera.assign<Camera>(960.0f, 540.0f, 0.1f, 1000.0f);
  auto health_bar = entities.create();
  health_bar.assign<D3Mode>();
  health_bar.assign<Transform>(glm::vec3(100.0f, 520.0f, 0.0f), nullptr,
                               glm::vec3(17.0f, 17.0f, 1.0f));
  health_bar.assign<Sprite>(
      gui_texture_atlas,
      Rectangle(glm::vec2(3.0f, 20.0f), glm::vec2(10.0f, 1.0f)));

  parent_scene->events.subscribe<Health>(*this);
  parent_scene->events.subscribe<Energy>(*this);
  parent_scene->events.subscribe<Player>(*this);

  auto stamina_bar = entities.create();
  stamina_bar.assign<Transform>(glm::vec3(300.0f, 520.0f, 0.0f), nullptr,
                               glm::vec3(17.0f, 17.0f, 1.0f));
  stamina_bar.assign<Sprite>(
      gui_texture_atlas,
      Rectangle(glm::vec2(3.0f, 17.1f), glm::vec2(10.0f, 1.0f)));
  stamina_bar.assign<D2Mode>();

  auto key_bar = entities.create();
  key_bar.assign<Transform>(glm::vec3(500.0f, 520.0f, 0.0f), nullptr,
                               glm::vec3(0));
  key_bar.assign<Sprite>(
      gui_texture_atlas,
      Rectangle(glm::vec2(3.0f, 3.0f), glm::vec2(16.0f, 8.0f)));
  key_bar.assign<MenuImage>();

  // adding systems
  systems.add<SpriteRenderer>();
  systems.configure();
}

GameUi::~GameUi() {
  parent_scene_->events.unsubscribe<Health>(*this);
  parent_scene_->events.unsubscribe<Energy>(*this);
  parent_scene_->events.unsubscribe<Player>(*this);
}

void GameUi::Update(entityx::TimeDelta dt) {
  systems.update<SpriteRenderer>(dt);
}

void GameUi::receive(const Energy &energy){
  entities.each<Transform, D2Mode>(
    [&](entityx::Entity stamina_bar, Transform &transform,
      D2Mode &d2){
      auto scale = transform.GetLocalScale();
      auto position = transform.GetLocalPosition();
      if(init_x_nrg == 0){
        init_x_nrg = scale.x;
      }
      if(init_pos_nrg == 0){
        init_pos_nrg = position.x;
      }
      float result = init_x_nrg * (energy.energy / energy.init_nrg);
      if(result > 0){
        scale.x = result;
      }
      else scale.x = result;
      transform.SetLocalScale(scale);
      float newpos = init_pos_nrg - (170 - ((170*(energy.energy / energy.init_nrg))))/2.0f;
      transform.SetLocalPosition(glm::vec3(newpos,position.y,position.z));
    });
}

void GameUi::receive(const Health& health) {
  entities.each<Transform, D3Mode>(
    [&](entityx::Entity health_bar, Transform &transform,
      D3Mode &wo){
      auto scale = transform.GetLocalScale();
      auto position = transform.GetLocalPosition();
      if(init_x == 0){
        init_x = scale.x;
      }
      if(init_pos == 0){
        init_pos = position.x;
      }
      float result = init_x * (health.hp / health.init_hp);
      if(result > 0){
        scale.x = result;
      }
      else scale.x = 0;
      transform.SetLocalScale(scale);
      float newpos = init_pos - (170 - ((170*(health.hp / health.init_hp))))/2.0f;
      transform.SetLocalPosition(glm::vec3(newpos,position.y,position.z));
    });
}


void GameUi::receive(const Player& player){
  entities.each<Transform, MenuImage>(
    [&](entityx::Entity key_bar, Transform &transform,
      MenuImage &mi){
        if(player.key){
          transform.SetLocalScale(glm::vec3(2));
        }
        else{
          transform.SetLocalScale(glm::vec3(0));
        }
    });
}
