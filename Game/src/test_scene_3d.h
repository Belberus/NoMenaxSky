#pragma once

#include <engine/components/common/camera.h>
#include <engine/components/common/transform.h>
#include <engine/components/three_d/model.h>
#include <engine/core/scene.h>
#include <engine/systems/three_d/model_renderer.h>
#include <glm/gtc/quaternion.hpp>

#include "components.h"
#include "systems_3d.h"

using namespace engine::core;
using namespace engine::systems::three_d;
using namespace engine::components::three_d;
using namespace engine::components::common;

class TestScene3D : public engine::core::Scene {
 public:
  TestScene3D() {
    engine::core::Engine::GetInstance().EnableDepthTest(
        engine::core::Engine::DepthTest::kLess);
    systems.add<ModelRenderer>();
    systems.add<CameraFollowPlayerSystem>();
    systems.configure();
    auto camera = entities.create();
    Transform camera_transform(glm::vec3(0.0f, 00.0f, 50.0f));
    // glm::quat camera_rot =
    //   glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // camera_transform.SetLocalOrientation(camera_rot);
    camera.assign<Transform>(camera_transform);
    camera.assign<Camera>(glm::radians(90.0f), 1600.0f, 900.0f, 0.1f, 1000.0f);
    auto model = entities.create();
    model.assign<Player>(Player::Orientation::DOWN);
    Transform t(glm::vec3(0.0f, 0.0f, 0.0f) /*, nullptr,
                glm::vec3(2.0f, 2.0f, 2.0f)*/);
    glm::quat rot;
    rot = glm::rotate(rot, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    t.SetLocalOrientation(rot);
    model.assign<Transform>(t);
    model.assign<Model>("assets/test/untitled.fbx");
    entities.each<Model>([&](entityx::Entity &entity, Model &model) {
      model.PlayAnimation("cab|walk", true);
    });
  }
  void Update(entityx::TimeDelta dt) {
    // systems.update<CameraFollowPlayerSystem>(dt);
    systems.update<ModelRenderer>(dt);
  }
};