#pragma once
#include <entityx/entityx.h>
#include <irrKlang.h>
#include <stdio.h>

using namespace irrklang;

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

struct DeathMessage {
  entityx::Entity entity;
  DeathMessage(entityx::Entity e);
};

struct AttackMessage {
  entityx::Entity entity;
  AttackMessage(entityx::Entity e, ISoundEngine* engine);
};
