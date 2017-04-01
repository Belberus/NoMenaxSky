#define _USE_MATH_DEFINES
#include "Systems.h"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

bool KnightAnimationSystem::getNext(
    entityx::ComponentHandle<KnightAnimation> knightAnimation,
    entityx::ComponentHandle<Graphics> graphics, entityx::TimeDelta dt,
    std::shared_ptr<AnimationClip> whatClip) {
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
  if (knightAnimation->index % whatClip->clip.size() == 0) {
    return true;
  } else {
    return false;
  }
}

void KnightAnimationSystem::update(entityx::EntityManager &es,
                                   entityx::EventManager &events,
                                   entityx::TimeDelta dt) {
  entityx::ComponentHandle<KnightAnimation> knightAnimation;
  entityx::ComponentHandle<Physics> physics;
  entityx::ComponentHandle<Graphics> graphics;
  entityx::ComponentHandle<KnightAttack> attack;
  for (entityx::Entity e1 : es.entities_with_components(
           knightAnimation, physics, graphics, attack)) {
    if (attack->isAttacking) {
      std::shared_ptr<AnimationClip> which;
      switch (attack->orientation) {
      case KnightAttack::Orientation::UP:
        which = knightAnimation->atk_n_top;
        break;
      case KnightAttack::Orientation::DOWN:
        which = knightAnimation->atk_n_down;
        break;
      case KnightAttack::Orientation::LEFT:
        which = knightAnimation->atk_n_left;
        break;
      case KnightAttack::Orientation::RIGHT:
        which = knightAnimation->atk_n_right;
        break;
      }
      attack->isAttacking = !getNext(knightAnimation, graphics, dt, which);
    } else if (physics->velocity.x > 0) {
      !getNext(knightAnimation, graphics, dt, knightAnimation->mov_right);
    } else if (physics->velocity.x < 0) {
      !getNext(knightAnimation, graphics, dt, knightAnimation->mov_left);
    } else if (physics->velocity.y > 0) {
      !getNext(knightAnimation, graphics, dt, knightAnimation->mov_top);
    } else if (physics->velocity.y < 0) {
      !getNext(knightAnimation, graphics, dt, knightAnimation->mov_down);
    }
  }
}

bool GhostAnimationSystem::getNext(
    entityx::ComponentHandle<GhostAnimation> ghostAnimation,
    entityx::ComponentHandle<Graphics> graphics, entityx::TimeDelta dt,
    std::shared_ptr<AnimationClip> whatClip) {
  if (ghostAnimation->which != whatClip) {
    ghostAnimation->index = 0;
    ghostAnimation->time = 0;
    ghostAnimation->which = whatClip;
    graphics->texture =
        whatClip->clip[ghostAnimation->index++ % whatClip->clip.size()];
  } else {
    ghostAnimation->time += dt;
    if (ghostAnimation->time >= whatClip->timePerFrame / 1000.0) {
      graphics->texture =
          whatClip->clip[ghostAnimation->index++ % whatClip->clip.size()];
      ghostAnimation->time = 0;
    }
  }
  if (ghostAnimation->index % whatClip->clip.size() == 0) {
    return true;
  } else {
    return false;
  }
}

void GhostAnimationSystem::update(entityx::EntityManager &es,
                                  entityx::EventManager &events,
                                  entityx::TimeDelta dt) {
   // Buscamos la entidad del jugador, en función de su posición moveremos al fantasma
  entityx::ComponentHandle<Player> player;
  entityx::ComponentHandle<Physics> physics_player;
  entityx::ComponentHandle<KnightAttack> attack;
  entityx::ComponentHandle<Position> position_player;

  for (entityx::Entity e :
              es.entities_with_components(player, physics_player, attack, position_player)){

  }

  // para todos los fantasmas, hacer que se muevan hacia el jugador
  entityx::ComponentHandle<GhostAnimation> ghostAnimation;
  entityx::ComponentHandle<Graphics> graphics;
  entityx::ComponentHandle<Position> position_ghost;
  entityx::ComponentHandle<Physics> physics_ghost;
#define SPEED_GHOST 50 // pixels por segundo
  for (entityx::Entity e1 :
       es.entities_with_components(ghostAnimation, physics_ghost, graphics, position_ghost)) {
    glm::vec2 v;
    if (position_player->position.y > position_ghost->position.y) {
      v.y += SPEED_GHOST;
    }
    if (position_player->position.y  < position_ghost->position.y ) {
      v.y += -SPEED_GHOST;
    }
    if (position_player->position.x > position_ghost->position.x) {
      v.x += SPEED_GHOST;
    }
    if (position_player->position.x < position_ghost->position.x) {
      v.x += -SPEED_GHOST;
    }
    physics_ghost->velocity = decompose(v);

    if (position_player->position.y > position_ghost->position.y +10) {
      getNext(ghostAnimation, graphics, dt, ghostAnimation->mov_top);
    } else if (position_player->position.y  < position_ghost->position.y) {
      getNext(ghostAnimation, graphics, dt, ghostAnimation->mov_down);
    } 
  }
}

bool CollisionSystem::areColliding(Body const &body1, Body const &body2) {
  if (body1.position.x < body2.position.x + body2.length.x &&
      body1.position.x + body1.length.x > body2.position.x &&
      body1.position.y < body2.position.y + body2.length.y &&
      body1.position.y + body1.length.y > body2.position.y) {
    return true;
  } else {
    return false;
  }
}

glm::vec2 CollisionSystem::depthOfCollision(Body const &body1,
                                            Body const &body2) {
  double depthX = 0;
  double depthY = 0;
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

void CollisionSystem::resolveCollision(Body &body, Position &pos,
                                       glm::vec2 const &depth) {
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

void CollisionSystem::update(entityx::EntityManager &es,
                             entityx::EventManager &events,
                             entityx::TimeDelta dt) {
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
GraphicsSystem::GraphicsSystem(Shaders &shaders) : shaders(shaders) {}
void GraphicsSystem::update(entityx::EntityManager &es,
                            entityx::EventManager &events,
                            entityx::TimeDelta dt) {
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(graphics->quad), graphics->quad,
                 GL_STATIC_DRAW);
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

PlayerInputSystem::PlayerInputSystem(Window &window) : window(window) {}

void PlayerInputSystem::update(entityx::EntityManager &es,
                               entityx::EventManager &events,
                               entityx::TimeDelta dt) {
  entityx::ComponentHandle<Player> player;
  entityx::ComponentHandle<Physics> physics;
  entityx::ComponentHandle<KnightAttack> attack;
#define SPEED 300 // pixels por segundo
  for (entityx::Entity entity :
       es.entities_with_components(player, physics, attack)) {
    glm::vec2 v;
    if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_W) == GLFW_PRESS) {
      v.y += SPEED;
    }
    if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_S) == GLFW_PRESS) {
      v.y += -SPEED;
    }
    if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_A) == GLFW_PRESS) {
      v.x += -SPEED;
    }
    if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_D) == GLFW_PRESS) {
      v.x += SPEED;
    }
    physics->velocity = decompose(v);

    if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_UP) == GLFW_PRESS) {
      attack->orientation = KnightAttack::Orientation::UP;
      attack->isAttacking = true;
    }
    if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_DOWN) == GLFW_PRESS) {
      attack->orientation = KnightAttack::Orientation::DOWN;
      attack->isAttacking = true;
    }
    if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_LEFT) == GLFW_PRESS) {
      attack->orientation = KnightAttack::Orientation::LEFT;
      attack->isAttacking = true;
    }
    if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
      attack->orientation = KnightAttack::Orientation::RIGHT;
      attack->isAttacking = true;
    }
  }
}

glm::vec2 decompose(const glm::vec2 &v) {
  if (v.x != 0 && v.y != 0) {
    return glm::vec2(v.x * std::cos(M_PI_4), v.y * std::sin(M_PI_4));
  } else {
    return v;
  }
}

void PhysicsSystem::update(entityx::EntityManager &es,
                           entityx::EventManager &events,
                           entityx::TimeDelta dt) {
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

void HealthSystem::update(entityx::EntityManager &es,
                          entityx::EventManager &events,
                          entityx::TimeDelta dt) {
  entityx::ComponentHandle<Health> health;
  for (entityx::Entity entity : es.entities_with_components(health)) {
    if (health->health <= 0) {
      events.emit<DeathMessage>(entity);
      entity.destroy();
    };
  }
}

void DeathListener::configure(entityx::EventManager &event_manager) {
  event_manager.subscribe<DeathMessage>(*this);
}

void DeathListener::update(entityx::EntityManager &entities,
                           entityx::EventManager &events,
                           entityx::TimeDelta dt) {}

void DeathListener::receive(const DeathMessage &deathMessage) {}
