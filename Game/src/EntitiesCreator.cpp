#include "EntitiesCreator.h"
#include "Systems.h"
#include <irrKlang.h>
#include <stdio.h>
using namespace irrklang;

void EntitiesCreator::addEntityKnight(entityx::EntityManager &entities, glm::vec3 position) {
  entityx::Entity player = entities.create();
  player.assign<Physics>(glm::vec2(0, 0));
  player.assign<Position>(position);
  player.assign<Player>();
  player.assign<Body>(position, glm::vec2(40, 40));
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

void EntitiesCreator::addEntityDeep(entityx::EntityManager &entities,
                                    glm::vec3 position) {
  entityx::Entity deep = entities.create();
  deep.assign<Position>(position);
  deep.assign<Body>(position, glm::vec2(200, 200));
  deep.assign<Graphics>(Texture("assets/elementos_mapa/agujero1_20x20.png"),
                        glm::vec2(200, 200));
}

void EntitiesCreator::addEntityWalls(entityx::EntityManager &entities) {
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
  leftWall.assign<RoomLimit>();
  rightWall.assign<RoomLimit>();
  topWall.assign<RoomLimit>();
  bottomWall.assign<RoomLimit>();
}

void EntitiesCreator::addEntityGhost(entityx::EntityManager &entities,
                                     glm::vec3 position) {
  // Inicializa Ghost y da valores iniciales a las variables
  entityx::Entity ghost = entities.create();
  ghost.assign<Position>(position);                // posicion inicial
  ghost.assign<Body>(position, glm::vec2(40, 40)); // posicion del body y tamaño
  ghost.assign<Health>(100);                       // vida
  ghost.assign<Physics>(glm::vec2(0, 0));          // físicas
  ghost.assign<Ghost>();

  std::vector<std::string> mov_right_str;
  mov_right_str.push_back("assets/Enemigo_Fantasma/right/right1.png");
  mov_right_str.push_back("assets/Enemigo_Fantasma/right/right2.png");

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

void EntitiesCreator::addEntityMenu(entityx::EntityManager &entities) {
  entityx::Entity menu = entities.create();
  menu.assign<Position>(glm::vec3(0, 0, 0));

  std::vector<std::string> menu_animation;
  menu_animation.push_back("assets/menu/gif/gif4.png");
  menu_animation.push_back("assets/menu/gif/gif3.png");
  menu_animation.push_back("assets/menu/gif/gif2.png");
  menu_animation.push_back("assets/menu/gif/gif1.png");
  menu.assign<MenuAnimation>(
      std::shared_ptr<AnimationClip>(new AnimationClip(menu_animation, 50)));
  menu.assign<Graphics>(Texture("assets/menu/gif/gif1.png"),
                        glm::vec2(960, 540));

  entityx::Entity titulo = entities.create();
  titulo.assign<Position>(glm::vec3(130, 375, 0));
  titulo.assign<Graphics>(Texture("assets/menu/titulo.png"),
                          glm::vec2(700, 150));

  entityx::Entity option1 = entities.create();
  option1.assign<Position>(glm::vec3(220, 240, 0));
  option1.assign<Graphics>(Texture("assets/menu/jugar.png"),
                           glm::vec2(500, 50));

  entityx::Entity option2 = entities.create();
  option2.assign<Position>(glm::vec3(220, 150, 0));
  option2.assign<Graphics>(Texture("assets/menu/opciones.png"),
                           glm::vec2(500, 50));

  entityx::Entity option3 = entities.create();
  option3.assign<Position>(glm::vec3(210, 50, 0));
  option3.assign<Graphics>(Texture("assets/menu/salir.png"),
                           glm::vec2(520, 70));

  entityx::Entity logo = entities.create();
  logo.assign<Position>(glm::vec3(850, 10, 0));
  logo.assign<Graphics>(Texture("assets/menu/patan_games.png"),
                        glm::vec2(100, 100));

  entityx::Entity menuArrow = entities.create();
  menuArrow.assign<Position>(glm::vec3(325, 247, 0));
  menuArrow.assign<Graphics>(Texture("assets/menu/ppc_front.png"),
                             glm::vec2(40, 40));
  menuArrow.assign<ArrowMenu>(ArrowMenu::Option::JUGAR);
}

void EntitiesCreator::addEntityOptions(entityx::EntityManager &entities) {
  entityx::Entity menu = entities.create();
  menu.assign<Position>(glm::vec3(0, 0, 0));

  std::vector<std::string> menu_animation;
  menu_animation.push_back("assets/menu/gif/gif4.png");
  menu_animation.push_back("assets/menu/gif/gif3.png");
  menu_animation.push_back("assets/menu/gif/gif2.png");
  menu_animation.push_back("assets/menu/gif/gif1.png");
  menu.assign<MenuAnimation>(
      std::shared_ptr<AnimationClip>(new AnimationClip(menu_animation, 50)));
  menu.assign<Graphics>(Texture("assets/menu/gif/gif1.png"),
                        glm::vec2(960, 540));

  entityx::Entity titulo = entities.create();
  titulo.assign<Position>(glm::vec3(130, 375, 0));
  titulo.assign<Graphics>(Texture("assets/menu/titulo.png"),
                          glm::vec2(700, 150));

  entityx::Entity option1 = entities.create();
  option1.assign<Position>(glm::vec3(220, 240, 0));
  option1.assign<Graphics>(Texture("assets/menu/opciones.png"),
                           glm::vec2(500, 50));

  entityx::Entity option2 = entities.create();
  option2.assign<Position>(glm::vec3(220, 150, 0));
  option2.assign<Graphics>(Texture("assets/menu/opciones.png"),
                           glm::vec2(500, 50));

  entityx::Entity option3 = entities.create();
  option3.assign<Position>(glm::vec3(210, 50, 0));
  option3.assign<Graphics>(Texture("assets/menu/salir.png"),
                           glm::vec2(520, 70));

  entityx::Entity logo = entities.create();
  logo.assign<Position>(glm::vec3(850, 10, 0));
  logo.assign<Graphics>(Texture("assets/menu/patan_games.png"),
                        glm::vec2(100, 100));

  entityx::Entity optionsArrow = entities.create();
  optionsArrow.assign<Position>(glm::vec3(325, 247, 0));
  optionsArrow.assign<Graphics>(Texture("assets/menu/ppc_front.png"),
                             glm::vec2(40, 40));
  optionsArrow.assign<ArrowOptions>(ArrowOptions::Option::MUSIC_VOL);
}

void EntitiesCreator::addEntityDoorBottom(entityx::EntityManager &entities, glm::vec3 position){
	entityx::Entity puertaAbajo = entities.create();
	puertaAbajo.assign<Position>(position);
	puertaAbajo.assign<Door>(position, glm::vec2(100, 18), 2);
	puertaAbajo.assign<Graphics>(
      Texture("assets/escenario/habitacion/puerta_abajo.png"),
      glm::vec2(100, 18));
}

void EntitiesCreator::addEntityDoorTop(entityx::EntityManager &entities, glm::vec3 position){
	entityx::Entity door = entities.create();
	door.assign<Position>(position);
	door.assign<Door>(position, glm::vec2(40, 40), 1);
	door.assign<Graphics>(
      Texture("assets/escenario/habitacion/puerta2_1_16x14.png"),
      glm::vec2(40, 40));
}

void EntitiesCreator::addEntityRoom1(entityx::EntityManager &entities) {
  addEntityWalls(entities);
  addEntityDoorBottom(entities,glm::vec3(425, 0, 0));
  addEntityDeep(entities, glm::vec3(385, 100, 0));
  addEntityDeep(entities, glm::vec3(690, 250, 0));
  addEntityDeep(entities, glm::vec3(75, 250, 0));
  addEntityGhost(entities, glm::vec3(385, 400, 0));
  addEntityGhost(entities, glm::vec3(125, 40, 0));
  addEntityGhost(entities, glm::vec3(560, 40, 0));
  addEntityKnight(entities, glm::vec3(475, 35, 0));
}

void EntitiesCreator::addEntityWallsStarter(entityx::EntityManager &entities) {
  entityx::Entity starterRoom = entities.create();
  starterRoom.assign<Position>(glm::vec3(250, 100, 0));
  starterRoom.assign<Graphics>(
      Texture("assets/escenario/habitacion/starterRoom.png"),
      glm::vec2(500, 250));
  entityx::Entity leftWall = entities.create();
  entityx::Entity rightWall = entities.create();
  entityx::Entity topWall = entities.create();
  entityx::Entity bottomWall = entities.create();
  leftWall.assign<Body>(glm::vec2(250, 100), glm::vec2(40, 250));
  rightWall.assign<Body>(glm::vec2(710, 100), glm::vec2(40, 250));
  topWall.assign<Body>(glm::vec2(250, 300), glm::vec2(500, 40));
  bottomWall.assign<Body>(glm::vec2(250, 100), glm::vec2(500, 35));

  entityx::Entity prision1 = entities.create();
  entityx::Entity prision2 = entities.create();
  entityx::Entity prision3 = entities.create();
  entityx::Entity prision4 = entities.create();
  prision1.assign<Position>(glm::vec3(290, 150, 0));
  prision1.assign<Body>(glm::vec2(290,220),glm::vec2(70,60));
  prision1.assign<Graphics>(
      Texture("assets/escenario/habitacion/prision.png"),
      glm::vec2(75, 200));
  prision2.assign<Position>(glm::vec3(360, 150, 0));
  prision2.assign<Body>(glm::vec2(360,220),glm::vec2(70,60));
  prision2.assign<Graphics>(
      Texture("assets/escenario/habitacion/prision.png"),
      glm::vec2(75, 200));
  prision3.assign<Position>(glm::vec3(570, 150, 0));
  prision3.assign<Body>(glm::vec2(570,220),glm::vec2(70,60));
  prision3.assign<Graphics>(
      Texture("assets/escenario/habitacion/prision.png"),
      glm::vec2(75, 200));
  prision4.assign<Position>(glm::vec3(640, 150, 0));
  prision4.assign<Body>(glm::vec2(640,220),glm::vec2(70,60));
  prision4.assign<Graphics>(
      Texture("assets/escenario/habitacion/prision.png"),
      glm::vec2(75, 200));
}

void EntitiesCreator::addEntityStarterRoom1(entityx::EntityManager &entities) {
  addEntityWallsStarter(entities);
  addEntityDoorTop(entities,glm::vec3(480,267,0));
  addEntityKnight(entities, glm::vec3(300, 150, 0));
}

