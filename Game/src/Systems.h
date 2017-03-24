#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Components.h"
#include "Window.h"
#include "Shaders.h"

class KnightAnimationSystem : public entityx::System<KnightAnimationSystem> {
  void getNext(entityx::ComponentHandle<KnightAnimation> knightAnimation,
               entityx::ComponentHandle<Graphics> graphics,
               entityx::TimeDelta dt, std::shared_ptr<AnimationClip> whatClip) {
    if (knightAnimation->which != whatClip) {
      knightAnimation->index = 0;
      knightAnimation->time = 0;
      knightAnimation->which = whatClip;
      graphics->texture =
          whatClip->clip[knightAnimation->index++ % whatClip->clip.size()];
    } else {
      knightAnimation->time += dt;
      if (knightAnimation->time >= whatClip->timePerFrame / 1000.0) {
        graphics->texture =
            whatClip->clip[knightAnimation->index++ % whatClip->clip.size()];
        knightAnimation->time = 0;
      }
    }
  }

 public:
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override {
    entityx::ComponentHandle<KnightAnimation> knightAnimation;
    entityx::ComponentHandle<Physics> physics;
    entityx::ComponentHandle<Graphics> graphics;
    for (entityx::Entity e1 :
         es.entities_with_components(knightAnimation, physics, graphics)) {
      if (physics->velocity.x > 0) {
        getNext(knightAnimation, graphics, dt, knightAnimation->mov_right);
      } else if (physics->velocity.x < 0) {
        getNext(knightAnimation, graphics, dt, knightAnimation->mov_left);
      } else if (physics->velocity.y > 0) {
        getNext(knightAnimation, graphics, dt, knightAnimation->mov_top);
      } else if (physics->velocity.y < 0) {
        getNext(knightAnimation, graphics, dt, knightAnimation->mov_down);
      }
    }
  }
};

class CollisionSystem : public entityx::System<CollisionSystem> {
  bool areColliding(Body const &body1, Body const &body2) {
    auto pos1 = body1.position;
    auto pos2 = body2.position;
    auto length1 = body1.length;
    auto length2 = body2.length;
    if (body1.position.x < body2.position.x + body2.length.x &&
        body1.position.x + body1.length.x > body2.position.x &&
        body1.position.y < body2.position.y + body2.length.y &&
        body1.position.y + body1.length.y > body2.position.y) {
      return true;
    } else {
      return false;
    }
  }

  glm::vec2 depthOfCollision(Body const &body1, Body const &body2) {
    int depthX = 0;
    int depthY = 0;
    if (body1.position.x > body2.position.x) {
      depthX = body2.position.x + body2.length.x - body1.position.x;
    } else {
      depthX -= body1.position.x + body1.length.x - body2.position.x;
    }
    if (body1.position.y > body2.position.y) {
      depthY = body2.position.y + body2.length.y - body1.position.y;
    } else {
      depthY -= body1.position.y + body1.length.y - body2.position.y;
    }
    return glm::vec2(depthX, depthY);
  }

  void resolveCollision(Body &body, Position &pos, glm::vec2 const &depth) {
    if (std::abs(depth.x) < std::abs(depth.y)) {
      body.position.x += depth.x;
      pos.position.x += depth.x;
    } else if (std::abs(depth.x) > std::abs(depth.y)) {
      body.position.y += depth.y;
      pos.position.y += depth.y;
    } else {
      body.position.x += depth.x;
      body.position.y += depth.y;
      pos.position.x += depth.x;
      pos.position.x += depth.y;
    }
  }

 public:
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override {
    entityx::ComponentHandle<Body> body1;
    entityx::ComponentHandle<Body> body2;
    entityx::ComponentHandle<Position> pos;
    entityx::ComponentHandle<Physics> phy;

    for (entityx::Entity e1 : es.entities_with_components(body1, pos, phy)) {
      for (entityx::Entity e2 : es.entities_with_components(body2)) {
        if (e2 != e1 && areColliding(*body1, *body2)) {
          glm::vec2 depth = depthOfCollision(*body1, *body2);
          resolveCollision(*body1, *pos, depth);
        }
      }
    }
  }
};

class GraphicsSystem : public entityx::System<GraphicsSystem> {
  Shaders &shaders;

 public:
  GraphicsSystem(Shaders &shaders) : shaders(shaders) {}
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override {
    entityx::ComponentHandle<Graphics> graphics;
    entityx::ComponentHandle<Position> position;
    for (entityx::Entity entity :
         es.entities_with_components(graphics, position)) {
      graphics->texture.load();
      glBindVertexArray(graphics->vao);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, graphics->ebo);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(graphics->indices),
                   graphics->indices, GL_STATIC_DRAW);
      glBindBuffer(GL_ARRAY_BUFFER, graphics->buf);
      glBufferData(GL_ARRAY_BUFFER, sizeof(graphics->quad),
                   graphics->quad, GL_STATIC_DRAW);
      glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT),
                            (GLvoid *)0);
      glEnableVertexAttribArray(0);
      glm::mat4 model;
      model = glm::translate(model, glm::vec3(position->position));
      model = glm::scale(model, glm::vec3(graphics->size, 1.0f));
      shaders.useProgram();
      glUniformMatrix4fv(glGetUniformLocation(shaders.getProgram(), "model"), 1,
                         GL_FALSE, glm::value_ptr(model));
      glUniformMatrix4fv(
          glGetUniformLocation(shaders.getProgram(), "projection"), 1, GL_FALSE,
          glm::value_ptr(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)));
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
  }
};

class PlayerInputSystem : public entityx::System<PlayerInputSystem> {
  Window &window;

 public:
  PlayerInputSystem(Window &window) : window(window) {}
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override {
    entityx::ComponentHandle<Player> player;
    entityx::ComponentHandle<Physics> physics;
#define SPEED 300  // pixels por segundo
    for (entityx::Entity entity :
         es.entities_with_components(player, physics)) {
      if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_W) == GLFW_PRESS) {
        physics->velocity.y = SPEED;
      }
      if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_S) == GLFW_PRESS) {
        physics->velocity.y = -SPEED;
      }
      if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_A) == GLFW_PRESS) {
        physics->velocity.x = -SPEED;
      }
      if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_D) == GLFW_PRESS) {
        physics->velocity.x = SPEED;
      }

      if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_SPACE) == GLFW_PRESS) {
      }
    }
  }
};

class PhysicsSystem : public entityx::System<PhysicsSystem> {
 public:
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override {
    entityx::ComponentHandle<Position> position;
    entityx::ComponentHandle<Physics> physics;
    for (entityx::Entity entity :
         es.entities_with_components(position, physics)) {
      position->position.x += physics->velocity.x * dt;
      position->position.y += physics->velocity.y * dt;
      if (entity.has_component<Body>()) {
        auto body = entity.component<Body>();
        body->position.x += physics->velocity.x * dt;
        body->position.y += physics->velocity.y * dt;
      }
      physics->velocity.x = 0;
      physics->velocity.y = 0;
    }
  }
};
