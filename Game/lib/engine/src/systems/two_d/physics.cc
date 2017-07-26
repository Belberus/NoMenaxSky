#include "engine/systems/two_d/physics.h"

#include <cmath>

#include <algorithm>
#include <iterator>

#include <glm/gtc/matrix_transform.hpp>

#include "engine/components/common/physics.h"
#include "engine/components/common/transform.h"
#include "engine/events/collision.h"

#include <iomanip>

void engine::systems::two_d::Physics::update(entityx::EntityManager &es,
                                             entityx::EventManager &events,
                                             entityx::TimeDelta dt) {
  entityx::ComponentHandle<engine::components::common::Transform> transform0;
  entityx::ComponentHandle<engine::components::two_d::AABBCollider> collider0;
  entityx::ComponentHandle<engine::components::common::Physics> physics0;

  auto collisions = DoCollisionDetection(es, dt);
  std::copy_if(collisions.begin(), collisions.end(),
               std::back_inserter(collisions_),
               [&](const ContactInfo &info) -> bool {
                 return std::find(collisions_.begin(), collisions_.end(),
                                  info) == collisions_.end();
               });

  auto info = GetFirstCollision(collisions);
  static int iter = 0;
  if (info.tmin <= 1.0f) {
    if (info.touching) {
      auto normal = -1.0f * info.normal;
      auto phy = info.e0.component<engine::components::common::Physics>();
      if (phy->velocity != glm::vec3(0, 0, 0) &&
          glm::dot(glm::vec3(normal, 0.0f), phy->velocity) < 0.0f) {
        phy->velocity -= glm::dot(glm::vec3(normal, 0.0f), phy->velocity) *
                         glm::vec3(normal, 0);
      }
      phy = info.e1.component<engine::components::common::Physics>();
      if (phy && phy->velocity != glm::vec3(0, 0, 0) &&
          glm::dot(glm::vec3(info.normal, 0.0f), phy->velocity) < 0.0f) {
        phy->velocity -= glm::dot(glm::vec3(info.normal, 0.0f), phy->velocity) *
                         glm::vec3(info.normal, 0);
      }
    } else {
      auto pre_transform0 =
          info.e0.component<engine::components::common::Transform>()
              ->GetWorldPosition();
      auto pre_transform1 =
          info.e1.component<engine::components::common::Transform>()
              ->GetWorldPosition();
      for (auto e : es.entities_with_components(physics0, transform0)) {
        transform0->SetLocalPosition(transform0->GetLocalPosition() +
                                     physics0->velocity * dt * info.tmin);
      }
      auto phy = info.e0.component<engine::components::common::Physics>();
      auto normal = -1.0f * info.normal;
      phy->velocity -= glm::dot(glm::vec3(normal, 0.0f), phy->velocity) *
                       glm::vec3(normal, 0);
      phy = info.e1.component<engine::components::common::Physics>();
      if (phy) {
        phy->velocity -= glm::dot(glm::vec3(info.normal, 0.0f), phy->velocity) *
                         glm::vec3(info.normal, 0);
      }
    }
    update(es, events, dt * (1.0f - info.tmin));
  } else {
    for (auto e : es.entities_with_components(physics0, transform0)) {
      transform0->SetLocalPosition(transform0->GetLocalPosition() +
                                   physics0->velocity * dt);
    }
    iter = 0;
    SendCollisionEvent(events, collisions_);
    collisions_.clear();
  }
}

bool engine::systems::two_d::Physics::TestAABBAABB(
    const engine::components::two_d::AABBCollider &a,
    const engine::components::two_d::AABBCollider &b, bool &touching,
    glm::vec2 &normal) {
  auto md = MinkowskiDifference(a, b);
  glm::vec2 md_vertices[4] = {
      md.center - md.half_size,                                // bottom left
      md.center + glm::vec2(-md.half_size.x, md.half_size.y),  // top left
      md.center + md.half_size,                                // top right
      md.center + glm::vec2(md.half_size.x, -md.half_size.y)   // bottom right
  };
  auto testo = md.center - md.half_size;

  glm::vec2 origin(0.0f, 0.0f);
  // check if the aabb's touch and what edge do they share
  touching = true;
  if (std::fabs(glm::length(md_vertices[0] - origin) +
                glm::length(origin - md_vertices[1]) -
                glm::length(md_vertices[0] - md_vertices[1])) <= 0.0001f) {
    normal = glm::vec2(-1.0f, 0.0f);
  } else if (std::fabs(glm::length(md_vertices[1] - origin) +
                       glm::length(origin - md_vertices[2]) -
                       glm::length(md_vertices[1] - md_vertices[2])) <=
             0.0001f) {
    normal = glm::vec2(0.0f, 1.0f);
  } else if (std::fabs(glm::length(md_vertices[2] - origin) +
                       glm::length(origin - md_vertices[3]) -
                       glm::length(md_vertices[2] - md_vertices[3])) <=
             0.0001f) {
    normal = glm::vec2(1.0f, 0.0f);
  } else if (std::fabs(glm::length(md_vertices[3] - origin) +
                       glm::length(origin - md_vertices[0]) -
                       glm::length(md_vertices[3] - md_vertices[0])) <=
             0.0001f) {
    normal = glm::vec2(0.0f, -1.0f);
  } else {
    // overlapping
    touching = false;
    return TestPointAABB(glm::vec2(0, 0), MinkowskiDifference(a, b));
  }
  return true;
}

bool engine::systems::two_d::Physics::TestPointAABB(
    const glm::vec2 &p, const engine::components::two_d::AABBCollider &aabb) {
  return p.x >= aabb.center.x && p.x <= aabb.center.x + aabb.half_size.x &&
         p.y >= aabb.center.y && p.y <= aabb.center.y + aabb.half_size.y;
}

bool engine::systems::two_d::Physics::TestMovingAABBAABB(
    const engine::components::two_d::AABBCollider &a,
    const engine::components::two_d::AABBCollider &b, glm::vec2 va,
    glm::vec2 vb, float dt, float &tfirst, glm::vec2 &normal) {
  auto relative_displacement = (vb - va) * dt;
  auto md = MinkowskiAddition(a, b);
  return TestSegmentAABB(b.center, relative_displacement, md, tfirst, normal);
}

bool engine::systems::two_d::Physics::HasTriggerCollider(entityx::Entity &e) {
  auto collider = e.component<engine::components::two_d::AABBCollider>();
  return collider && collider->trigger;
}

engine::systems::two_d::Physics::ContactInfo
engine::systems::two_d::Physics::GetFirstCollision(
    std::vector<ContactInfo> &collisions) {
  ContactInfo min_time_collision;
  for (auto &collision : collisions) {
    if (!HasTriggerCollider(collision.e0) &&
        !HasTriggerCollider(collision.e1) && collision < min_time_collision &&
        (IsCollidingContact(collision.e0, collision.e1,
                            glm::vec3(-1.0f * collision.normal, 0.0f)) ||
         IsCollidingContact(collision.e1, collision.e0,
                            glm::vec3(collision.normal, 0.0f)) ||
         !collision.touching)) {
      min_time_collision = collision;
    }
  }
  return min_time_collision;
}

void engine::systems::two_d::Physics::SendCollisionEvent(
    entityx::EventManager &events, const std::vector<ContactInfo> &collisions) {
  for (const auto &c : collisions) {
    events.emit<engine::events::Collision>(c.e0, c.e1);
  }
}

std::vector<engine::systems::two_d::Physics::ContactInfo>
engine::systems::two_d::Physics::DoCollisionDetection(
    entityx::EntityManager &es, entityx::TimeDelta dt) {
  std::vector<ContactInfo> collisions;
  entityx::ComponentHandle<engine::components::common::Transform> transform0;
  entityx::ComponentHandle<engine::components::two_d::AABBCollider> collider0;
  entityx::ComponentHandle<engine::components::common::Physics> physics0;
  entityx::ComponentHandle<engine::components::common::Transform> transform1;
  entityx::ComponentHandle<engine::components::two_d::AABBCollider> collider1;
  entityx::ComponentHandle<engine::components::common::Physics> physics1;

  auto moving_colliders =
      es.entities_with_components<engine::components::two_d::AABBCollider,
                                  engine::components::common::Physics>();
  auto static_colliders =
      es.entities_with_components<engine::components::two_d::AABBCollider>();
  for (auto e0It = moving_colliders.begin(); e0It != moving_colliders.end();
       ++e0It) {
    collider0 = (*e0It).component<engine::components::two_d::AABBCollider>();
    transform0 = (*e0It).component<engine::components::common::Transform>();
    physics0 = (*e0It).component<engine::components::common::Physics>();
    auto world_collider0 = LocalToWorldCollider(*collider0, *transform0);
    for (auto e1It = std::next(e0It); e1It != moving_colliders.end(); ++e1It) {
      collider1 = (*e1It).component<engine::components::two_d::AABBCollider>();
      transform1 = (*e1It).component<engine::components::common::Transform>();
      physics1 = (*e1It).component<engine::components::common::Physics>();
      auto world_collider1 = LocalToWorldCollider(*collider1, *transform1);
      float tfirst;
      glm::vec2 normal;
      bool touching;
      if (TestAABBAABB(world_collider0, world_collider1, touching, normal)) {
        ContactInfo info;
        info.e0 = *e0It;
        info.e1 = *e1It;
        info.tmin = 0.0f;
        info.normal = normal;
        info.touching = touching;
        collisions.push_back(info);
      } else if (TestMovingAABBAABB(world_collider0, world_collider1,
                                    physics0->velocity, physics1->velocity, dt,
                                    tfirst, normal)) {
        collisions.emplace_back((*e0It), (*e1It), tfirst, normal);
      }
    }
    for (auto e1It = static_colliders.begin(); e1It != static_colliders.end();
         ++e1It) {
      if (!(*e1It).has_component<engine::components::common::Physics>()) {
        collider1 =
            (*e1It).component<engine::components::two_d::AABBCollider>();
        transform1 = (*e1It).component<engine::components::common::Transform>();
        auto world_collider1 = LocalToWorldCollider(*collider1, *transform1);
        float tfirst;
        glm::vec2 normal;
        bool touching;
        if (TestAABBAABB(world_collider0, world_collider1, touching, normal)) {
          ContactInfo info;
          info.e0 = *e0It;
          info.e1 = *e1It;
          info.tmin = 0.0f;
          info.normal = normal;
          info.touching = touching;
          collisions.push_back(info);
        } else if (TestMovingAABBAABB(world_collider0, world_collider1,
                                      physics0->velocity, glm::vec2(0, 0), dt,
                                      tfirst, normal)) {
          collisions.emplace_back((*e0It), (*e1It), tfirst, normal);
        }
      }
    }
  }
  return collisions;
}

void engine::systems::two_d::Physics::DoCollisionResponse(
    entityx::EntityManager &es, const ContactInfo &info) {}

bool engine::systems::two_d::Physics::TestSegmentAABB(
    const glm::vec2 &p, glm::vec2 &d,
    const engine::components::two_d::AABBCollider &aabb, float &tmin,
    glm::vec2 &normal) {
  tmin = std::numeric_limits<float>::min();
  float tmax = std::numeric_limits<float>::max();
  int what_plane = 0;
  static const float EPSILON = 0.00001f;
  for (int i = 0; i < 2; i++) {
    if (std::fabs(d[i]) < EPSILON) {
      if (p[i] < aabb.center[i] - aabb.half_size[i] ||
          p[i] > aabb.center[i] + aabb.half_size[i]) {
        return false;
      }
    } else {
      float ood = 1.0f / d[i];
      float t1 = (aabb.center[i] - aabb.half_size[i] - p[i]) * ood;
      float t2 = (aabb.center[i] + aabb.half_size[i] - p[i]) * ood;
      if (t1 > t2) {
        std::swap(t1, t2);
      }
      if (t1 > tmin) {
        tmin = t1;
        what_plane = i;
      }
      if (t2 < tmax) {
        tmax = t2;
      }
      if (tmin > tmax) {
        return false;
      }
    }
  }
  if (tmin >= 1.0f || tmax <= 0.0f) {
    return false;
  }
  if (what_plane == 0) {
    normal.y = 0.0f;
    normal.x = d[0] > 0.0f ? -1.0f : 1.0f;
  } else {
    normal.x = 0.0f;
    normal.y = d[1] > 0.0f ? -1.0f : 1.0f;
  }
  return true;
}

engine::components::two_d::AABBCollider
engine::systems::two_d::Physics::MinkowskiDifference(
    const engine::components::two_d::AABBCollider &a,
    const engine::components::two_d::AABBCollider &b) {
  glm::vec2 center = a.center - a.half_size - (b.center + b.half_size) +
                     a.half_size + b.half_size;
  glm::vec2 size = a.half_size + b.half_size;
  return engine::components::two_d::AABBCollider(center, size);
}

engine::components::two_d::AABBCollider
engine::systems::two_d::Physics::MinkowskiAddition(
    const engine::components::two_d::AABBCollider &a,
    const engine::components::two_d::AABBCollider &b) {
  glm::vec2 size = a.half_size + b.half_size;
  return engine::components::two_d::AABBCollider(a.center, size);
}

engine::components::two_d::AABBCollider
engine::systems::two_d::Physics::LocalToWorldCollider(
    const engine::components::two_d::AABBCollider &collider,
    const engine::components::common::Transform &transform) {
  auto new_position =
      transform.GetWorldMatrix() * glm::vec4(collider.center, 0, 1);
  return engine::components::two_d::AABBCollider(new_position,
                                                 collider.half_size);
}

bool engine::systems::two_d::Physics::IsCollidingContact(
    entityx::Entity e0, entityx::Entity e1, const glm::vec3 &collision_normal) {
  auto e0_phy = e0.component<engine::components::common::Physics>();
  if (e0_phy) {
    return glm::dot(collision_normal, e0_phy->velocity) < 0.0f;
  } else {
    return false;
  }
}

engine::systems::two_d::Physics::ContactInfo::ContactInfo()
    : e0(),
      e1(),
      tmin(std::numeric_limits<float>::max()),
      normal(),
      touching(false) {}

engine::systems::two_d::Physics::ContactInfo::ContactInfo(
    const entityx::Entity &e0, const entityx::Entity &e1, float tmin,
    const glm::vec2 &normal)
    : e0(e0), e1(e1), tmin(tmin), normal(normal), touching(false) {}

bool engine::systems::two_d::Physics::ContactInfo::operator<(
    const ContactInfo &info) {
  return tmin < info.tmin;
}

bool engine::systems::two_d::Physics::ContactInfo::operator==(
    const ContactInfo &info) {
  return (e0 == info.e0 && e1 == info.e1) || (e1 == info.e0 && e0 == info.e1);
}
