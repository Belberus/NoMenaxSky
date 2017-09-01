#include "floor_factory_3d.h"

#include <engine/components/common/camera.h>
#include <engine/components/common/physics.h>
#include <engine/components/common/transform.h>
#include <engine/components/three_d/model.h>
#include <engine/components/two_d/aabb_collider.h>
#include <engine/core/engine.h>

#include "entity_factory_3d.h"

std::unique_ptr<Floor3D> FloorFactory3D::MakeFloor1() {
  engine::core::Engine::GetInstance().EnableDepthTest(
      engine::core::Engine::DepthTest::kLess);
  std::unique_ptr<Floor3D> floor(std::make_unique<Floor3D>());
  // create the floor model
  auto floor_model = floor->entities.create();
  floor_model.assign<engine::components::three_d::Model>(
      "assets/3d/castillo/planta1/planta1.dae");
  engine::components::common::Transform floor_transform(
      glm::vec3(0.0f, 0.0f, 0.0f));
  glm::quat floor_model_rotation;
  floor_model_rotation = glm::rotate(floor_model_rotation, glm::radians(180.0f),
                                     glm::vec3(0.0f, 0.0f, 1.0f));
  floor_transform.SetLocalOrientation(floor_model_rotation);
  floor_model.assign<engine::components::common::Transform>(floor_transform);
  // create the camera
  auto camera = floor->entities.create();
  camera.assign<engine::components::common::Camera>(400.0f, 300.0f, 0.1f,
                                                    1000.0f);
  engine::components::common::Transform camera_transform(
      glm::vec3(0.0f, 0.0f, 100.0f));
  camera.assign<engine::components::common::Transform>(camera_transform);
  // create the player
  EntityFactory3D::MakeKnight(floor->entities, glm::vec3(0.0f, 0.0f, 0.0f));
  // creating some colliders
  auto c1 = floor->entities.create();
  c1.assign<engine::components::common::Transform>(glm::vec3(40, 62, 0));
  c1.assign<engine::components::two_d::AABBCollider>(glm::vec2(0, 0),
                                                     glm::vec2(7, 7));
  auto c2 = floor->entities.create();
  c2.assign<engine::components::common::Transform>(glm::vec3(18, 62, 0));
  c2.assign<engine::components::two_d::AABBCollider>(glm::vec2(0, 0),
                                                     glm::vec2(7, 7));
  auto c3 = floor->entities.create();
  c3.assign<engine::components::common::Transform>(glm::vec3(-20, 62, 0));
  c3.assign<engine::components::two_d::AABBCollider>(glm::vec2(0, 0),
                                                     glm::vec2(7, 7));
  auto c4 = floor->entities.create();
  c4.assign<engine::components::common::Transform>(glm::vec3(-42, 62, 0));
  c4.assign<engine::components::two_d::AABBCollider>(glm::vec2(0, 0),
                                                     glm::vec2(7, 7));
  auto c5 = floor->entities.create();
  c5.assign<engine::components::common::Transform>(glm::vec3(40, 33, 0));
  c5.assign<engine::components::two_d::AABBCollider>(glm::vec2(0, 0),
                                                     glm::vec2(7, 7));
  auto c6 = floor->entities.create();
  c6.assign<engine::components::common::Transform>(glm::vec3(18, 62, 0));
  c6.assign<engine::components::two_d::AABBCollider>(glm::vec2(0, 0),
                                                     glm::vec2(7, 7));
  auto c7 = floor->entities.create();
  c7.assign<engine::components::common::Transform>(glm::vec3(-20, 62, 0));
  c7.assign<engine::components::two_d::AABBCollider>(glm::vec2(0, 0),
                                                     glm::vec2(7, 7));
  auto c8 = floor->entities.create();
  c8.assign<engine::components::common::Transform>(glm::vec3(-42, 62, 0));
  c8.assign<engine::components::two_d::AABBCollider>(glm::vec2(0, 0),
                                                     glm::vec2(7, 7));
  return floor;
}
