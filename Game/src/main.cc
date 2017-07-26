#include <engine/core/engine.h>
#include <engine/core/scene_manager.h>

#include <engine/core/audio_manager.h>

#include "game.h"

using namespace engine::core;

int main() {
  Engine::Init({960, 540, "No Menax Sky", false, false, true});
  Game game;
  Engine::GetInstance().Get<SceneManager>().Play(game);
}
