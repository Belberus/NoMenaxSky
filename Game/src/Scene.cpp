#include "Scene.h"
#include "Actor.h"
#include <cmath>
#include <glm/glm.hpp>
#include <iostream>

void Scene::addActor(Actor *actor) { actors.push_back(actor); }

void Scene::update(Window &window, Shaders &shaders) {
  for (auto a : actors) {
    a->update(window, *this, shaders);
  }
}

void Scene::checkCollision(Actor &actor) {
  for (auto a : actors) {
    glm::vec3 posA = a->getPosition();
    glm::vec2 sizeA = a->getSize();
    glm::vec3 posB = actor.getPosition();
    glm::vec2 sizeB = actor.getSize();
    if (posA.x != posB.x || posA.y != posB.y) {
      if (posA.x < posB.x + sizeB.x && posA.x + sizeA.x > posB.x &&
          posA.y < posB.y + sizeB.y && posA.y + sizeA.y > posB.y) {
        int depthX = 0;
        int depthY = 0;

        if (posB.x > posA.x) {
          depthX = posA.x + sizeA.x - posB.x;
        } else {
          depthX -= posB.x + sizeB.x - posA.x;
        }

        if (posB.y > posA.y) {
          depthY = posA.y + sizeA.y - posB.y;
        } else {
          depthY -= posB.y + sizeB.y - posA.y;
        }

        if (std::abs(depthX) < std::abs(depthY)) {
          posB.x += depthX;
        } else if (std::abs(depthX) > std::abs(depthY)) {
          posB.y += depthY;
        } else {
          posB.x += depthX;
          posB.y += depthY;
        }
        actor.setPosition(posB);
      }
    }
  }
}
