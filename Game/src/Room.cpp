#include "Room.h"
#include "Systems.h"
#include "../lib/irrKlang/include/irrKlang.h"
#include <stdio.h>
using namespace irrklang;

void Room::addEntityKnight(entityx::EntityManager &entities) {
  entityx::Entity player = entities.create();
  player.assign<Physics>(glm::vec2(0, 0));
  player.assign<Position>(glm::vec3(100, 100, 0));
  player.assign<Player>();
  player.assign<Body>(glm::vec2(100, 100), glm::vec2(40, 40));
  player.assign<KnightAttack>(100, KnightAttack::Orientation::UP);

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
      std::shared_ptr<AnimationClip>(new AnimationClip(atk_n_top_str, 100)),
      std::shared_ptr<AnimationClip>(new AnimationClip(atk_n_down_str, 100)),
      std::shared_ptr<AnimationClip>(new AnimationClip(atk_n_left_str, 100)),
      std::shared_ptr<AnimationClip>(new AnimationClip(atk_n_right_str, 100)),
      std::shared_ptr<AnimationClip>(new AnimationClip(mov_top_str, 100)),
      std::shared_ptr<AnimationClip>(new AnimationClip(mov_down_str, 100)),
      std::shared_ptr<AnimationClip>(new AnimationClip(mov_left_str, 100)),
      std::shared_ptr<AnimationClip>(new AnimationClip(mov_right_str, 100)));
  player.assign<Graphics>(Texture("assets/pp_caballero/static/ppc_front.png"),
                          glm::vec2(40, 40));
}

void Room::addEntityDeep(entityx::EntityManager &entities, glm::vec3 position, glm::vec2 bodyPosition) {
  entityx::Entity deep = entities.create();
  deep.assign<Position>(position);
  deep.assign<Body>(bodyPosition, glm::vec2(40, 40));
  deep.assign<Graphics>(Texture("assets/elementos_mapa/agujero1_20x20.png"),
                          glm::vec2(40, 40));
}

void Room::addEntityRoom(entityx::EntityManager &entities) {
  entityx::Entity room = entities.create();
  room.assign<Position>(glm::vec3(0, 0, 0));
  room.assign<Graphics>(
      Texture("assets/escenario/habitacion/habitacion_transparente.png"),
      glm::vec2(960, 540));
  entityx::Entity leftWall = entities.create();
  entityx::Entity rightWall = entities.create();
  entityx::Entity topWall = entities.create();
  entityx::Entity bottomWall = entities.create();
  leftWall.assign<Body>(glm::vec2(0, 0), glm::vec2(20, 540));
  rightWall.assign<Body>(glm::vec2(940, 0), glm::vec2(20, 540));
  topWall.assign<Body>(glm::vec2(0, 520), glm::vec2(960, 20));
  bottomWall.assign<Body>(glm::vec2(0, 0), glm::vec2(960, 20));
}

void Room::addEntityGhost(entityx::EntityManager &entities, glm::vec3 position,
                          glm::vec2 body) {
  // Inicializa Ghost y da valores iniciales a las variables
  entityx::Entity ghost = entities.create();
  ghost.assign<Position>(position);            // posicion inicial
  ghost.assign<Body>(body, glm::vec2(10, 29)); // posicion del body y tamaño
  ghost.assign<Health>(100);                   // vida
  ghost.assign<Physics>(glm::vec2(0, 0));      // físicas
  ghost.assign<Ghost>();

  std::vector<std::string> mov_right_str;
  mov_right_str.push_back("assets/Enemigo_Fantasma/right/right1.png");
  mov_right_str.push_back("assets/Enemgio_Fantasma/right/right2.png");

  std::vector<std::string> mov_left_str;
  mov_left_str.push_back("assets/Enemigo_Fantasma/left/left1.png");
  mov_left_str.push_back("assets/Enemigo_Fantasma/left/left2.png");

  std::vector<std::string> mov_top_str;
  mov_top_str.push_back("assets/Enemigo_Fantasma/back/back1.png");
  mov_top_str.push_back("assets/Enemigo_Fantasma/back/back2.png");

  std::vector<std::string> mov_down_str;
  mov_down_str.push_back("assets/Enemigo_Fantasma/front/front1.png");
  mov_down_str.push_back("assets/Enemigo_Fantasma/front/front2.png");

  ghost.assign<GhostAnimation>(
      std::shared_ptr<AnimationClip>(new AnimationClip(mov_top_str, 300)),
      std::shared_ptr<AnimationClip>(new AnimationClip(mov_down_str, 300)),
      std::shared_ptr<AnimationClip>(new AnimationClip(mov_left_str, 300)),
      std::shared_ptr<AnimationClip>(new AnimationClip(mov_right_str, 300)));
  ghost.assign<Graphics>(Texture("assets/Enemigo_Fantasma/front/front1.png"),
                         glm::vec2(40, 40));
}



void Room::addEntityMenu(entityx::EntityManager &entities) {
  entityx::Entity menu = entities.create();
  menu.assign<Position>(glm::vec3(0, 0, 0));

  std::vector<std::string> menu_animation;
  menu_animation.push_back("assets/menu/gif/gif4.png");
  menu_animation.push_back("assets/menu/gif/gif3.png");
  menu_animation.push_back("assets/menu/gif/gif2.png");
  menu_animation.push_back("assets/menu/gif/gif1.png");
  menu.assign<MenuAnimation>(
      std::shared_ptr<AnimationClip>(new AnimationClip(menu_animation, 50)));
  menu.assign<Graphics>(
      Texture("assets/menu/gif/gif1.png"),
      glm::vec2(960, 540));

  entityx::Entity titulo = entities.create();
  titulo.assign<Position>(glm::vec3(130, 375, 0));
  titulo.assign<Graphics>(
      Texture("assets/menu/titulo.png"),
      glm::vec2(700, 150));

  entityx::Entity option1 = entities.create();
  option1.assign<Position>(glm::vec3(220, 240, 0));
  option1.assign<Graphics>(
      Texture("assets/menu/jugar.png"),
      glm::vec2(500, 50));

  entityx::Entity option2 = entities.create();
  option2.assign<Position>(glm::vec3(220, 150, 0));
  option2.assign<Graphics>(
      Texture("assets/menu/opciones.png"),
      glm::vec2(500, 50));

  entityx::Entity option3 = entities.create();
  option3.assign<Position>(glm::vec3(210, 50, 0));
  option3.assign<Graphics>(
      Texture("assets/menu/salir.png"),
      glm::vec2(520, 70));

  entityx::Entity logo = entities.create();
  logo.assign<Position>(glm::vec3(850, 10, 0));
  logo.assign<Graphics>(
      Texture("assets/menu/patan_games.png"),
      glm::vec2(100, 100));

  entityx::Entity menuArrow = entities.create();
  menuArrow.assign<Position>(glm::vec3(325, 247, 0));
  menuArrow.assign<Graphics>(
      Texture("assets/menu/ppc_front.png"),
      glm::vec2(40, 40));
  menuArrow.assign<ArrowMenu>(ArrowMenu::Option::JUGAR);

}

void Room::addEntityRoom(Window &window, Shaders &shaders){
  ISoundEngine* engine = createIrrKlangDevice();
  engine->play2D("../media/tune2.wav",true);

  addEntityRoom(entities);
  addEntityGhost(entities, glm::vec3(165, 350, 0), glm::vec2(175, 360));
  addEntityGhost(entities, glm::vec3(265, 250, 0), glm::vec2(275, 260));
  addEntityGhost(entities, glm::vec3(240, 350, 0), glm::vec2(250, 360));
  addEntityGhost(entities, glm::vec3(280, 250, 0), glm::vec2(290, 260));
  addEntityGhost(entities, glm::vec3(165, 300, 0), glm::vec2(175, 310));
  addEntityGhost(entities, glm::vec3(265, 100, 0), glm::vec2(275, 110));
  addEntityGhost(entities, glm::vec3(180, 200, 0), glm::vec2(190, 210));
  addEntityGhost(entities, glm::vec3(140, 120, 0), glm::vec2(150, 130));
  addEntityGhost(entities, glm::vec3(250, 350, 0), glm::vec2(260, 360));
  addEntityGhost(entities, glm::vec3(265, 300, 0), glm::vec2(275, 310));

  addEntityDeep(entities,glm::vec3(150,150,0),glm::vec2(150,150));
  addEntityDeep(entities,glm::vec3(150,200,0),glm::vec2(150,200));
  addEntityDeep(entities,glm::vec3(150,250,0),glm::vec2(150,250));
  addEntityDeep(entities,glm::vec3(150,300,0),glm::vec2(150,300));
  addEntityDeep(entities,glm::vec3(150,350,0),glm::vec2(150,350));

  addEntityKnight(entities);
}

Room::Room(Window &window, Shaders &shaders) {
	addEntityMenu(entities);

  systems.add<GraphicsSystem>(shaders);
  systems.add<KnightAnimationSystem>();
  systems.add<GhostAnimationSystem>();
  systems.add<PlayerInputSystem>(window);
  systems.add<MenuInputSystem>(window);
  systems.add<PhysicsSystem>();
  systems.add<CollisionSystem>();
  systems.add<MenuAnimationSystem>();
  systems.configure();
}

void Room::update(entityx::TimeDelta dt) {
  systems.update<PlayerInputSystem>(dt);
  systems.update<MenuInputSystem>(dt);
  systems.update<KnightAnimationSystem>(dt);
  systems.update<GhostAnimationSystem>(dt);
  systems.update<PhysicsSystem>(dt);
  systems.update<CollisionSystem>(dt);
  systems.update<MenuAnimationSystem>(dt);
  systems.update<GraphicsSystem>(dt);

}
