#ifndef ENGINE_SYSTEMS_TWO_D_PHYSICS_H_
#define ENGINE_SYSTEMS_TWO_D_PHYSICS_H_
#include <unordered_set>
#include <vector>

#include <entityx/entityx.h>

#include "engine/components/common/transform.h"
#include "engine/components/two_d/aabb_collider.h"
#include "engine/events/ignore_collision.h"

namespace engine {
namespace systems {
namespace two_d {
class Physics : public entityx::System<Physics>,
                public entityx::Receiver<Physics> {
 public:
  void configure(entityx::EventManager &event_manager) override;
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
  void receive(const engine::events::IgnoreCollision &ignore_collision);
  void receive(const entityx::EntityDestroyedEvent &entity_destroyed);

 private:
  /// A helper classed that holds information regarding a collision.
  struct ContactInfo {
    entityx::Entity e0;
    entityx::Entity e1;
    float tmin;
    glm::vec2 normal;  // the direction is from e0 to e1
    bool touching;
    ContactInfo();
    ContactInfo(const entityx::Entity &e0, const entityx::Entity &e1,
                float tmin, const glm::vec2 &normal);
    bool operator<(const ContactInfo &info) const;
    bool operator==(const ContactInfo &inf) const;
  };

  /// Pair of entites where the order is not important so (e1, e0) == (e0, e1)
  struct UnorderdPair {
    entityx::Entity e0;
    entityx::Entity e1;
    UnorderdPair(entityx::Entity e0, entityx::Entity e1) : e0(e0), e1(e1) {}
    bool Contains(const entityx::Entity &e) const { return e0 == e || e1 == e; }
    bool operator==(const UnorderdPair &rhs) const {
      return (e0 == rhs.e0 && e1 == rhs.e1) || (e0 == rhs.e1 && e1 == rhs.e0);
    }
    bool operator!=(const UnorderdPair &rhs) const { return !(*this == rhs); }
  };

  struct UnorderdPairHasher {
    // HACK: probably not a good hash function
    std::size_t operator()(const UnorderdPair &unorderd_pair) const {
      return unorderd_pair.e0.id().id() * unorderd_pair.e0.id().id() + 0x42;
    }
  };

  /// Checks if the entity has a trigger type collider.
  /// @param e entity to check
  /// @return true if the entity has a collider that is a trigger
  static bool HasTriggerCollider(entityx::Entity &e);

  /// Get the first collision that is going to occur.
  /// @param collisions all the collisions that are going to occur
  /// @return ContactInfo that happens first, that is, his tmin is the minimum
  ContactInfo GetFirstCollision(std::vector<ContactInfo> &collisions);

  /// Sends collision events.
  /// @param events EventManager that will send the events
  /// @param collision all the info about the collisions that occured
  static void SendCollisionEvent(entityx::EventManager &events,
                                 const std::vector<ContactInfo> &collisions);

  /// Detects all the collisions that are going to happen during a periof of
  /// time
  /// @param es EntityManager that has the info the entitites
  /// @param dt period of time in seconds  in which we are intersetd to see if a
  /// collision happens
  /// @return vector with all the info about the collisions that are going to
  /// occure during dt
  static std::vector<ContactInfo> DoCollisionDetection(
      entityx::EntityManager &es, entityx::TimeDelta dt);

  static void DoCollisionResponse(entityx::EntityManager &es,
                                  const ContactInfo &info);

  /// Tests if a line segments intersects an AABB.
  /// @param p starting point of the segment
  /// @param d direction and length of the segment
  /// @param aabb AABB used to test against
  /// @param tmin first time of intersection
  /// @param normal the normal vector of the first edge of the AABB that was hit
  /// @return true if the segment intersects the AABB
  static bool TestSegmentAABB(
      const glm::vec2 &p, glm::vec2 &d,
      const engine::components::two_d::AABBCollider &aabb, float &tmin,
      glm::vec2 &normal);

  /// Performs a Minkowski "difference".
  /// @param a first AABB
  /// @param b second AABB
  /// @return a + (-b)
  static engine::components::two_d::AABBCollider MinkowskiDifference(
      const engine::components::two_d::AABBCollider &a,
      const engine::components::two_d::AABBCollider &b);

  /// Performs a Minkowski addition on two AABB.
  /// @param a first AABB
  /// @param b second AABB. This AABB will be treated as if it is center is at
  /// the origin
  /// @return Minkowski addition of the two AABB
  static engine::components::two_d::AABBCollider MinkowskiAddition(
      const engine::components::two_d::AABBCollider &a,
      const engine::components::two_d::AABBCollider &b);

  /// Tests if two AABB are intersecting.
  /// @param a fist AABB
  /// @param b second AABB
  /// @return true if the two AABB are intersecting, false otherwise
  static bool TestAABBAABB(const engine::components::two_d::AABBCollider &a,
                           const engine::components::two_d::AABBCollider &b,
                           bool &touching, glm::vec2 &normal);

  /// Tests if a point is inside an AABB.
  /// @param p point to test
  /// @param aabb collider to test
  /// @return true if the point is inside the AABB, false otherwise
  static bool TestPointAABB(
      const glm::vec2 &p, const engine::components::two_d::AABBCollider &aabb);

  /// Tests if two moving AABB are going to collide.
  /// @param a first collider
  /// @param b second collider
  /// @param va velocity of the first collider
  /// @param vb velocity of the second collider
  /// @param dt period of time in seconds that the AABB are moving
  /// @param tfirst first time of contact
  /// @param normal conctact normal
  /// @return true if the two AABB are going to colide during the period of time
  /// dt, false otherwise
  static bool TestMovingAABBAABB(
      const engine::components::two_d::AABBCollider &a,
      const engine::components::two_d::AABBCollider &b, glm::vec2 va,
      glm::vec2 vb, float dt, float &tfirst, glm::vec2 &normal);

  /// Transform a AABBCollider that's in local space to world space
  /// @param collider collider to transform
  /// @param transrom transform object
  static engine::components::two_d::AABBCollider LocalToWorldCollider(
      const engine::components::two_d::AABBCollider &collider,
      const engine::components::common::Transform &transform);

  static bool IsCollidingContact(entityx::Entity e0, entityx::Entity e1,
                                 const glm::vec3 &collision_normal);

  std::vector<ContactInfo> collisions_;
  std::unordered_set<UnorderdPair, UnorderdPairHasher> ignored_entities_;
};
}  // namespace two_d
}  // namespace systems
}  // namespace engine
#endif  // ENGINE_SYSTEMS_TWO_D_PHYSICS_H_
