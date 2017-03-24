#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <entityx/entityx.h>
#include "Shaders.h"
#include "Window.h"

#include <glm/glm.hpp>
#include "Texture.h"

#include "Systems.h"

class Scene : public entityx::EntityX {
 public:
  Scene(Window &window, Shaders &shaders) {
    entityx::Entity player = entities.create();
    player.assign<Physics>(glm::vec2(0, 0));
    player.assign<Position>(glm::vec3(0, 0, 0));
    player.assign<Player>();
    player.assign<Body>(glm::vec2(0, 0), glm::vec2(70, 70));

    std::vector<std::string> mov_right_str;
    mov_right_str.push_back("assets/pp_caballero/mov_right/ppc_step1.png");
    mov_right_str.push_back("assets/pp_caballero/mov_right/ppc_step2.png");

    std::vector<std::string> mov_left_str;
    mov_left_str.push_back("assets/pp_caballero/mov_left/ppc_step1.png");
    mov_left_str.push_back("assets/pp_caballero/mov_left/ppc_step2.png");

    std::vector<std::string> mov_top_str;
    mov_top_str.push_back("assets/pp_caballero/mov_top/ppc_step1.png");
    mov_top_str.push_back("assets/pp_caballero/mov_top/ppc_step2.png");

    std::vector<std::string> mov_down_str;
    mov_down_str.push_back("assets/pp_caballero/mov_down/ppc_step1.png");
    mov_down_str.push_back("assets/pp_caballero/mov_down/ppc_step2.png");

    std::vector<std::string> atk_n_right_str;
    atk_n_right_str.push_back("assets/pp_caballero/atk_n_right/ppc_n_atk1.png");
    atk_n_right_str.push_back("assets/pp_caballero/atk_n_right/ppc_n_atk2.png");
    atk_n_right_str.push_back("assets/pp_caballero/atk_n_right/ppc_n_atk3.png");

    std::vector<std::string> atk_n_left_str;
    atk_n_left_str.push_back("assets/pp_caballero/atk_n_left/ppc_n_atk1.png");
    atk_n_left_str.push_back("assets/pp_caballero/atk_n_left/ppc_n_atk2.png");
    atk_n_left_str.push_back("assets/pp_caballero/atk_n_left/ppc_n_atk3.png");

    std::vector<std::string> atk_n_top_str;
    atk_n_top_str.push_back("assets/pp_caballero/atk_n_top/ppc_n_atk1.png");
    atk_n_top_str.push_back("assets/pp_caballero/atk_n_top/ppc_n_atk2.png");
    atk_n_top_str.push_back("assets/pp_caballero/atk_n_top/ppc_n_atk3.png");

    std::vector<std::string> atk_n_down_str;
    atk_n_down_str.push_back("assets/pp_caballero/atk_n_down/ppc_n_atk1.png");
    atk_n_down_str.push_back("assets/pp_caballero/atk_n_down/ppc_n_atk2.png");
    atk_n_down_str.push_back("assets/pp_caballero/atk_n_down/ppc_n_atk3.png");

    player.assign<KnightAnimation>(
        std::shared_ptr<AnimationClip>(new AnimationClip(atk_n_top_str, 300)),
        std::shared_ptr<AnimationClip>(new AnimationClip(atk_n_down_str, 300)),
        std::shared_ptr<AnimationClip>(new AnimationClip(atk_n_left_str, 300)),
        std::shared_ptr<AnimationClip>(new AnimationClip(atk_n_right_str, 300)),
        std::shared_ptr<AnimationClip>(new AnimationClip(mov_top_str, 300)),
        std::shared_ptr<AnimationClip>(new AnimationClip(mov_down_str, 300)),
        std::shared_ptr<AnimationClip>(new AnimationClip(mov_left_str, 300)),
        std::shared_ptr<AnimationClip>(new AnimationClip(mov_right_str, 300)));

    GLuint vao;
    GLuint ebo;
    GLuint buf;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &ebo);
    glGenBuffers(1, &buf);
    player.assign<Graphics>(
        Texture("assets/pp_caballero/atk_n_down/ppc_n_atk1.png"), vao, ebo, buf,
        glm::vec2(70, 70));

    entityx::Entity enemy = entities.create();
    enemy.assign<Position>(glm::vec3(400, 400, 0));
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &ebo);
    glGenBuffers(1, &buf);
    enemy.assign<Graphics>(Texture("assets/sombra.png"), vao, ebo, buf,
                           glm::vec2(100, 100));
    enemy.assign<Body>(glm::vec2(400, 400), glm::vec2(100, 100));

    systems.add<GraphicsSystem>(shaders);
    systems.add<KnightAnimationSystem>();
    systems.add<PlayerInputSystem>(window);
    systems.add<PhysicsSystem>();
    systems.add<CollisionSystem>();
    systems.configure();
  }

  void update(entityx::TimeDelta dt) {
    systems.update<PlayerInputSystem>(dt);
    systems.update<KnightAnimationSystem>(dt);
    systems.update<PhysicsSystem>(dt);
    systems.update<CollisionSystem>(dt);
    systems.update<GraphicsSystem>(dt);
  }
};

int main() {
  Window window("Gauntleto", 960, 540);
  Shaders shaders;
  Scene scene(window, shaders);
  double t1_f = glfwGetTime();
  int frames = 0;
  entityx::TimeDelta dt = 0;
  while (!window.isClosed()) {
    // TODO: mover calculo dt a la clase Window
    entityx::TimeDelta t1 = glfwGetTime();
    window.clear();
    window.pollEvents();
    scene.update(dt);
    window.swapBuffers();
    entityx::TimeDelta t2 = glfwGetTime();
    dt = t2 - t1;
  }
}
