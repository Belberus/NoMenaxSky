#include "systems_3d.h"

#include <engine/components/common/camera.h>
#include <engine/components/common/physics.h>
#include <engine/components/common/transform.h>
#include <engine/components/three_d/model.h>
#include <glm/glm.hpp>

#include "components.h"

const float CameraFollowPlayerSystem::kCameraSpeed = 4.0f;

void CameraFollowPlayerSystem::update(entityx::EntityManager &es,
                                      entityx::EventManager &events,
                                      entityx::TimeDelta dt) {
  glm::vec3 player_pos;
  es.each<engine::components::common::Transform, Player>(
      [&](entityx::Entity entity,
          engine::components::common::Transform &transform,
          Player &player) { player_pos = transform.GetWorldPosition(); });
  es.each<engine::components::common::Transform,
          engine::components::common::Camera>(
      [&](entityx::Entity entity,
          engine::components::common::Transform &transform,
          engine::components::common::Camera &camera) {
        auto old_camera_pos = transform.GetLocalPosition();
        auto new_camera_pos =
            glm::mix(old_camera_pos, player_pos, dt * kCameraSpeed);
        new_camera_pos.z = old_camera_pos.z;
        transform.SetLocalPosition(new_camera_pos);
      });
}

KnighAnimationSystem3D::KnighAnimationSystem3D()
    : current_state_(State::kIdle) {}

void KnighAnimationSystem3D::update(entityx::EntityManager &es,
                                    entityx::EventManager &events,
                                    entityx::TimeDelta dt) {
  entityx::ComponentHandle<Shield> shield;
  for (auto e : es.entities_with_components(shield)) {
    break;
  }

  es.each<engine::components::common::Physics,
          engine::components::three_d::Model, Player, KnightAttack>(
      [&](entityx::Entity, engine::components::common::Physics &physics,
          engine::components::three_d::Model &model, Player &player,
          KnightAttack &attack) {
        switch (current_state_) {
          case State::kIdle:
            if (attack.is_attacking) {
              current_state_ = State::kAttacking;
              model.PlayAnimation("cab|attack", true);
            } else if (shield->active) {
              current_state_ = State::kShieldUp;
              model.PlayAnimation("cab|shield_up", false);
            } else if (physics.velocity != glm::vec3(0.0f, 0.0f, 0.0f)) {
              current_state_ = State::kMoving;
              model.PlayAnimation("cab|walk", true);
            }
            break;
          case State::kAttacking:
            if (!attack.is_attacking) {
              current_state_ = State::kIdle;
              model.PlayAnimation("cab|idle", true);
            }
            break;
          case State::kMoving:
            if (physics.velocity == glm::vec3(0.0f, 0.0f, 0.0f)) {
              current_state_ = State::kIdle;
              model.PlayAnimation("cab|idle", true);
            } else if (attack.is_attacking) {
              current_state_ = State::kMovingAttacking;
              model.PlayAnimation("cab|walk_attack", true);
            } else if (shield->active) {
              current_state_ = State::kMovingDefending;
              model.PlayAnimation("cab|walk_shield_up", true);
            }
            break;
          case State::kMovingAttacking:
            if (physics.velocity == glm::vec3(0.0f, 0.0f, 0.0f)) {
              current_state_ = State::kIdle;
              model.PlayAnimation("cab|idle", true);
            } else if (!attack.is_attacking) {
              current_state_ = State::kMoving;
              model.PlayAnimation("cab|walk", true);
            }
            break;
          case State::kMovingDefending:
            if (!shield->active) {
              current_state_ = State::kShieldDown;
              model.PlayAnimation("cab|shield_down", false);
            }
            break;
          case State::kShieldDown:
            if (physics.velocity == glm::vec3(0.0f, 0.0f, 0.0f)) {
              current_state_ = State::kIdle;
              model.PlayAnimation("cab|idle", true);
            } else {
              current_state_ = State::kMoving;
              model.PlayAnimation("cab|walk", true);
            }
            break;
          case State::kShieldUp:
            if (!shield->active) {
              current_state_ = State::kShieldDown;
              model.PlayAnimation("cab|shield_down", false);
            }
            break;
        }

      });
}

WizardAnimationSystem3D::WizardAnimationSystem3D()
    : current_state_(State::kIdle) {}

void WizardAnimationSystem3D::update(entityx::EntityManager &es,
                                     entityx::EventManager &events,
                                     entityx::TimeDelta dt) {
  es.each<engine::components::common::Physics,
          engine::components::three_d::Model, Player, Wizard>(
      [&](entityx::Entity, engine::components::common::Physics &physics,
          engine::components::three_d::Model &model, Player &player,
          Wizard &attack) {
        switch (current_state_) {
          case State::kIdle:
            if (attack.is_attacking) {
              current_state_ = State::kAttacking;
              model.PlayAnimation("wiz|attack", true);
            } else if (physics.velocity != glm::vec3(0.0f, 0.0f, 0.0f)) {
              current_state_ = State::kMoving;
              model.PlayAnimation("wiz|walk", true);
            }
            break;
          case State::kAttacking:
            if (!attack.is_attacking) {
              if (physics.velocity == glm::vec3(0.0f, 0.0f, 0.0f)) {
                current_state_ = State::kIdle;
                model.PlayAnimation("wiz|idle", true);
              } else {
                current_state_ = State::kIdle;
                model.PlayAnimation("wiz|walk", true);
              }
            }
            break;
          case State::kMoving:
            if (physics.velocity == glm::vec3(0.0f, 0.0f, 0.0f)) {
              current_state_ = State::kIdle;
              model.PlayAnimation("wiz|idle", true);
            } if (attack.is_attacking) {
              current_state_ = State::kAttacking;
              model.PlayAnimation("wiz|attack", true);
            }
            break;
        }

      });
}