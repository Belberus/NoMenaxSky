#pragma once
#include <entityx/entityx.h>

struct DeathMessage {
  entityx::Entity entity;
  DeathMessage(entityx::Entity e);
};
