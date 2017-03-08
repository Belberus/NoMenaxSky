#include "Warrior.h"
#include "Scene.h"
#include "Shaders.h"
#include "Texture.h"
#include "Window.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define SPEED 4
void WarriorInputComponent::update(Window &window, Actor &actor) {
  glm::vec2 velocity;
  if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_W) == GLFW_PRESS) {
    velocity.y = SPEED;
  }
  if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_S) == GLFW_PRESS) {
    velocity.y = -SPEED;
  }
  if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_A) == GLFW_PRESS) {
    velocity.x = -SPEED;
  }
  if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_D) == GLFW_PRESS) {
    velocity.x = SPEED;
  }
  actor.setVelocity(velocity);
}

WarriorGraphicsComponent::WarriorGraphicsComponent() {
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &ebo);
  glGenBuffers(1, &buf);
}

void WarriorGraphicsComponent::update(Actor &actor, Shaders &shaders) {
  actor.getTexture()->load();
  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, buf);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT),
                        (GLvoid *)0);
  glEnableVertexAttribArray(0);
  glm::mat4 model;
  model = glm::translate(model, glm::vec3(actor.getPosition()));
  model = glm::scale(model, glm::vec3(actor.getSize(), 1.0f));
  shaders.useProgram();
  glUniformMatrix4fv(glGetUniformLocation(shaders.getProgram(), "model"), 1,
                     GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(
      glGetUniformLocation(shaders.getProgram(), "projection"), 1, GL_FALSE,
      glm::value_ptr(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)));
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

WarriorGraphicsComponent::~WarriorGraphicsComponent() {
  glDeleteBuffers(1, &ebo);
  glDeleteBuffers(1, &buf);
  glDeleteVertexArrays(1, &vao);
}

WarriorPhysicsComponent::WarriorPhysicsComponent(glm::vec2 position,
                                                 glm::vec2 size)
    : PhysicsComponent(position, size) {}

void WarriorPhysicsComponent::update(Actor &actor, Scene &scene) {
  glm::vec3 position = actor.getPosition();
  position.x += actor.getVelocity().x;
  position.y += actor.getVelocity().y;
  actor.setPosition(position);
  scene.checkCollision(actor);
}
