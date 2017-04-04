#define _USE_MATH_DEFINES
#include <GL/gl3w.h>
#include "Systems.h"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <irrKlang.h>
#include <stdio.h>
#include <time.h>
#include <GLFW/glfw3.h>

#define SPEED_GHOST 150 // pixels por segundo
#define SPEED 300


using namespace irrklang;
using namespace std;

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

ISoundEngine* engine = createIrrKlangDevice();




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
      
      //engine->play2D("assets/media/fx/ahh.wav");
      
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
  //engine->drop();
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
  /* initialize random seed: */
  srand (time(NULL));

   // Buscamos la entidad del jugador, en función de su posición moveremos al fantasma
  entityx::ComponentHandle<Player> player;
  entityx::ComponentHandle<Physics> physics_player;
  entityx::ComponentHandle<KnightAttack> attack;
  entityx::ComponentHandle<Position> position_player;
  engine->setSoundVolume(0.1);

  for (entityx::Entity e :
              es.entities_with_components(player, physics_player, attack, position_player)){

  }

  // para todos los fantasmas, hacer que se muevan hacia el jugador
  entityx::ComponentHandle<GhostAnimation> ghostAnimation;
  entityx::ComponentHandle<Graphics> graphics;
  entityx::ComponentHandle<Position> position_ghost;
  entityx::ComponentHandle<Physics> physics_ghost;
  entityx::ComponentHandle<Body> body_ghost;
  entityx::ComponentHandle<Body> body;
  
  int currentSec = (int)(glfwGetTime()*100.0);

  for (entityx::Entity e1 :
       es.entities_with_components(ghostAnimation, physics_ghost, graphics, position_ghost, body_ghost)) {
    glm::vec2 v;
    for (entityx::Entity e2: 
           es.entities_with_components(body)){
        if (position_player->position.y > position_ghost->position.y) {
          if((currentSec%200==99 || (currentSec%200==198)) ){
            //engine->play2D("assets/media/fx/talk.wav");
          }
          v.y = SPEED_GHOST;
        }
        if (position_player->position.y  < position_ghost->position.y ) {
          if((currentSec%200==99 || (currentSec%200==198))){
            //engine->play2D("assets/media/fx/talk.wav");
          }
          v.y = -SPEED_GHOST;
        }
        if (position_player->position.x > position_ghost->position.x) {
          if((currentSec%200==99 || (currentSec%200==198))){
            //engine->play2D("assets/media/fx/talk.wav");
          }
          v.x = SPEED_GHOST;
        }
        if (position_player->position.x < position_ghost->position.x) {
          if((currentSec%200==99 || (currentSec%200==198))){
            //engine->play2D("assets/media/fx/talk.wav");
          }
          v.x = -SPEED_GHOST;
        }
    }/* for */
    physics_ghost->velocity = decompose(v);

    if (position_player->position.y > position_ghost->position.y +10) {
      getNext(ghostAnimation, graphics, dt, ghostAnimation->mov_top);
    } else if (position_player->position.y  < position_ghost->position.y) {
      getNext(ghostAnimation, graphics, dt, ghostAnimation->mov_down);
    } 
  }
}

bool areColliding(Body const &body1, Body const &body2) {
  if (body1.position.x < body2.position.x + body2.length.x &&
      body1.position.x + body1.length.x > body2.position.x &&
      body1.position.y < body2.position.y + body2.length.y &&
      body1.position.y + body1.length.y > body2.position.y) {
    return true;
  } else {
    return false;
  }
}

glm::vec2 depthOfCollision(Body const &body1,
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

// FIXME: arreglar la resolucion de colisiones en general
void resolveCollision(Body &body, Position &pos,
                                       glm::vec2 const &depth) {
  
    if (std::abs(depth.x) <= std::abs(depth.y)) {
        body.position.x += depth.x;
        pos.position.x += depth.x;
    }
    else {
        body.position.y += depth.y;
        pos.position.y += depth.y;
    }
}

void resolveCollisionGhostKnight(Body &body1, Body &body2, 
              Position &pos1, Position &pos2, glm::vec2 const &depth){
  if (std::abs(depth.x) <= std::abs(depth.y)) { // se chocan en el eje x, al empujarte los fantasmas, el fantasma nunca llegara a estar entre el caballero y la pared de la izquierda
    if (20 + body2.length.x < body2.position.x + body2.length.x < 940 - body2.length.x){ // el caballero cabe entre la pared y el fantasma
        body1.position.x += depth.x;
        pos1.position.x += depth.x;
    } 
  } else {
    if (20 + body2.length.y < body2.position.y + body2.length.y < 520 - body2.length.y){
      body1.position.y += depth.y;
      pos1.position.y += depth.y;
    }
  }


}

// Resolver colisiones del guerrero con otras cosas
void CollisionSystem::update(entityx::EntityManager &es,
                             entityx::EventManager &events,
                             entityx::TimeDelta dt) {
  entityx::ComponentHandle<Body> body1;
  entityx::ComponentHandle<Body> body2;
  entityx::ComponentHandle<Position> pos;
  entityx::ComponentHandle<KnightAnimation> kanim;
  entityx::ComponentHandle<Physics> phy;

  for (entityx::Entity e1 : es.entities_with_components(body1, pos, phy, kanim)) {
    for (entityx::Entity e2 : es.entities_with_components(body2)) { // resolvemos con paredes, agujeros...
      if (e2 != e1 && areColliding(*body1, *body2) && !e2.has_component<GhostAnimation>()) {
        glm::vec2 depth = depthOfCollision(*body1, *body2);
        resolveCollision(*body1, *pos, depth);
      }
    }
  }
}

// Resolver colisiones del fantasma con otras cosas
void CollisionGhostSystem::update(entityx::EntityManager &es,
                             entityx::EventManager &events,
                             entityx::TimeDelta dt) {
  entityx::ComponentHandle<Body> body1;
  entityx::ComponentHandle<Body> body2;
  entityx::ComponentHandle<Position> pos1;
  entityx::ComponentHandle<Position> pos2;
  entityx::ComponentHandle<GhostAnimation> ghost;
  entityx::ComponentHandle<KnightAnimation> kanim;
  entityx::ComponentHandle<Physics> phy;
  entityx::ComponentHandle<RoomLimit> isLimit;

  for (entityx::Entity e1 : es.entities_with_components(body1, pos1, phy, ghost)) {
    for (entityx::Entity e3 : es.entities_with_components(body2, pos2, kanim)){ // resolvemos colision con caballero
      if (e1 != e3 && areColliding(*body1, *body2)){
        glm::vec2 depth = depthOfCollision(*body1, *body2);
        resolveCollisionGhostKnight(*body1, *body2, *pos1, *pos2, depth);
      }
    }

    for (entityx::Entity e2 : es.entities_with_components(body2, ghost)){
      if (e1!=e2 && areColliding(*body1,*body2)){
        glm::vec2 depth = depthOfCollision(*body1, *body2);
        resolveCollision(*body1, *pos1, depth);
      }
    }

    for (entityx::Entity e2 : es.entities_with_components(body2, isLimit)){
      if (e1!=e2 && areColliding(*body1,*body2)){
        glm::vec2 depth = depthOfCollision(*body1, *body2);
        resolveCollision(*body1, *pos1, depth);
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
/*
void playSound(string route, int volume){
  ISound* snd = engine->play2D("assets/media/fx/step2.wav",false,false,true);

}*/


bool MenuAnimationSystem::getNext(
    entityx::ComponentHandle<MenuAnimation> menuAnimation,
    entityx::ComponentHandle<Graphics> graphics, entityx::TimeDelta dt,
    std::shared_ptr<AnimationClip> whatClip) {
  if (menuAnimation->which != whatClip) {
    menuAnimation->index = 0;
    menuAnimation->time = 0;
    menuAnimation->which = whatClip;
    graphics->texture =
        whatClip->clip[menuAnimation->index++ % whatClip->clip.size()];
  } else {
    menuAnimation->time += dt;
    if (menuAnimation->time >= whatClip->timePerFrame / 1000.0) {
      graphics->texture =
          whatClip->clip[menuAnimation->index++ % whatClip->clip.size()];
      menuAnimation->time = 0;
    }
  }
  if (menuAnimation->index % whatClip->clip.size() == 0) {
    return true;
  } else {
    return false;
  }
}

void MenuAnimationSystem::update(entityx::EntityManager &es,
                                   entityx::EventManager &events,
                                   entityx::TimeDelta dt) {
  entityx::ComponentHandle<MenuAnimation> menuAnimation;
  entityx::ComponentHandle<Graphics> graphics;
  for (entityx::Entity e1 : es.entities_with_components(
           menuAnimation, graphics)) {
    getNext(menuAnimation, graphics, dt, menuAnimation->menu_animation);
  }
}

DoorSystem::DoorSystem(Window &window) : window(window) {}
void DoorSystem::update(entityx::EntityManager &es,
                               entityx::EventManager &events,
                               entityx::TimeDelta dt) {
	entityx::ComponentHandle<Body> body1;
  	entityx::ComponentHandle<Door> door;
  	entityx::ComponentHandle<Player> player;
  	entityx::ComponentHandle<Position> position;
  	entityx::ComponentHandle<Physics> physics;

	for (entityx::Entity e1 : es.entities_with_components(body1, position, player, physics)) {
    	glm::vec2 v = centerOfTheBody(*body1);
    	for (entityx::Entity e2 : es.entities_with_components(door)) {
 			if((v.x >= door->position.x && v.x <= (door->position.x + door->length.x)) && v.y >= door->position.y){
 				if (door->numberOfRoom == 1) {
 					events.emit<GoToRoomMessage>(2);
 				}
 			}     
    	}
  	}
}

glm::vec2 DoorSystem::centerOfTheBody(Body &body){
	return glm::vec2(body.position.x + (body.length.x / 2), body.position.y + (body.length.y /2));
}

MenuInputSystem::MenuInputSystem(Window &window) : window(window) {}

int lastSec = 0;
bool onHold = false;
void MenuInputSystem::update(entityx::EntityManager &es,
                               entityx::EventManager &events,
                               entityx::TimeDelta dt) {
  entityx::ComponentHandle<ArrowMenu> arrowMenu;
  entityx::ComponentHandle<Position> position;
  glm::vec3 pos1(325, 247, 0);
  glm::vec3 pos2(325, 157, 0);
  glm::vec3 pos3(325, 65, 0);
  int currentSec = (int)(glfwGetTime()*100.0);
  //double currentS = glfwGetTime()*100.0;
  //int test =  ((int)currentS*100) - currentSec*100;
  if(((currentSec%30)==1) || ((currentSec%30)==0)){
    onHold=false;
  }
  for (entityx::Entity entity :
       es.entities_with_components(arrowMenu, position)) {
    if(!onHold){
      onHold=true;
      if (window.isKeyPressed(GLFW_KEY_UP)) {
            switch (arrowMenu->option){
              case ArrowMenu::Option::JUGAR:
                  break;
              case ArrowMenu::Option::OPCIONES:
                    position->position = pos1;
                    arrowMenu->option = ArrowMenu::Option::JUGAR;
                    break;                  
              case ArrowMenu::Option::SALIR:
                  position->position = pos2;
                  arrowMenu->option = ArrowMenu::Option::OPCIONES;
                  break;
            } 
      }
      if (window.isKeyPressed(GLFW_KEY_DOWN)) {
            switch (arrowMenu->option){
              case ArrowMenu::Option::JUGAR:
                  position->position = pos2;
                  arrowMenu->option = ArrowMenu::Option::OPCIONES;
                  break;
              case ArrowMenu::Option::OPCIONES:
                  position->position = pos3;
                  arrowMenu->option = ArrowMenu::Option::SALIR;
                  break;
              case ArrowMenu::Option::SALIR:
                  break;
            } 
      }
      if (window.isKeyPressed(GLFW_KEY_ENTER)) {
            switch (arrowMenu->option){
              case ArrowMenu::Option::JUGAR:
                  events.emit<InitRoomMessage>();
                  break;
              case ArrowMenu::Option::OPCIONES:
                  events.emit<OptionsMessage>();
                  break;
              case ArrowMenu::Option::SALIR:
                  exit(0);
                  break;
            } 
      }
    }
  
  }
}

OptionsInputSystem::OptionsInputSystem(Window &window) : window(window) {}

bool onHold2 = false;
void OptionsInputSystem::update(entityx::EntityManager &es,
                               entityx::EventManager &events,
                               entityx::TimeDelta dt) {
  entityx::ComponentHandle<ArrowOptions> arrowOptions;
  entityx::ComponentHandle<Position> position;
  glm::vec3 pos1(325, 247, 0);
  glm::vec3 pos2(325, 157, 0);
  glm::vec3 pos3(325, 65, 0);
  int currentSec = (int)(glfwGetTime()*100.0);
  //double currentS = glfwGetTime()*100.0;
  //int test =  ((int)currentS*100) - currentSec*100;
  if(((currentSec%30)==1) || ((currentSec%30)==0)){
    onHold2=false;
  }
  for (entityx::Entity entity :
       es.entities_with_components(arrowOptions, position)) {
    if(!onHold){
      onHold=true;
      if (window.isKeyPressed(GLFW_KEY_UP)) {
            switch (arrowOptions->option){
              case ArrowOptions::Option::MUSIC_VOL:
                  break;
              case ArrowOptions::Option::FX_VOL:
                    position->position = pos1;
                    arrowOptions->option = ArrowOptions::Option::MUSIC_VOL;
                    break;                  
              case ArrowOptions::Option::SALIR:
                  position->position = pos2;
                  arrowOptions->option = ArrowOptions::Option::FX_VOL;
                  break;
            } 
      }
      if (window.isKeyPressed(GLFW_KEY_DOWN)) {
            switch (arrowOptions->option){
              case ArrowOptions::Option::MUSIC_VOL:
                  position->position = pos2;
                  arrowOptions->option = ArrowOptions::Option::FX_VOL;
                  break;
              case ArrowOptions::Option::FX_VOL:
                  position->position = pos3;
                  arrowOptions->option = ArrowOptions::Option::SALIR;
                  break;
              case ArrowOptions::Option::SALIR:
                  break;
            } 
      }
      if (window.isKeyPressed(GLFW_KEY_ENTER)) {
            switch (arrowOptions->option){
              case ArrowOptions::Option::MUSIC_VOL:
                  //VolumeManager::toggleMusic;
                  break;
              case ArrowOptions::Option::FX_VOL:
                  //VolumeManager::toggleFX;
                  break;
              case ArrowOptions::Option::SALIR:
                  events.emit<GoBackMessage>();
                  break;
            } 
      }
    }
  
  }
}

PlayerInputSystem::PlayerInputSystem(Window &window) : window(window) {}

int counter=15;
int counter2=15;
void PlayerInputSystem::update(entityx::EntityManager &es,
                               entityx::EventManager &events,
                               entityx::TimeDelta dt) {
  entityx::ComponentHandle<Player> player;
  entityx::ComponentHandle<Physics> physics;
  entityx::ComponentHandle<KnightAttack> attack;
  engine->setSoundVolume(0.1);
  for (entityx::Entity entity :
       es.entities_with_components(player, physics, attack)) {
    glm::vec2 v;
    if (window.isKeyPressed(GLFW_KEY_W)) {    
      v.y += SPEED;
      if(counter == 15){
        engine->play2D("assets/media/fx/step2.wav");
        counter = 0;
      }
      counter++;
    }
    if (window.isKeyPressed(GLFW_KEY_S)) {
      v.y += -SPEED;
      if(counter == 15){
        engine->play2D("assets/media/fx/step2.wav");
        counter = 0;
      }
      counter++;
    }
    if (window.isKeyPressed(GLFW_KEY_A)) {
      v.x += -SPEED;
      if(counter == 15){
        engine->play2D("assets/media/fx/step2.wav");
        counter = 0;
      }
      counter++;
    }
    if (window.isKeyPressed(GLFW_KEY_D)) {
      v.x += SPEED;
      if(counter == 15){
        engine->play2D("assets/media/fx/step2.wav");
        counter = 0;
      }
      counter++;
    }
    physics->velocity = decompose(v);

    if (window.isKeyPressed(GLFW_KEY_UP)) {
      attack->orientation = KnightAttack::Orientation::UP;
      attack->isAttacking = true;
      if(counter2 == 15){
        engine->play2D("assets/media/fx/attack.wav");
        counter2=0;
      }
      counter2++;      
    }
    if (window.isKeyPressed(GLFW_KEY_DOWN)) {
      attack->orientation = KnightAttack::Orientation::DOWN;
      attack->isAttacking = true;
      if(counter2 == 15){
        engine->play2D("assets/media/fx/attack.wav");
        counter2=0;
      } 
      counter2++;
    }
    if (window.isKeyPressed(GLFW_KEY_LEFT)) {
      attack->orientation = KnightAttack::Orientation::LEFT;
      attack->isAttacking = true;
      if(counter2 == 15){
        engine->play2D("assets/media/fx/attack.wav");
        counter2=0;
      }
      counter2++;
    }
    if (window.isKeyPressed(GLFW_KEY_RIGHT)) {
      attack->orientation = KnightAttack::Orientation::RIGHT;
      attack->isAttacking = true;
      if(counter2 == 15){
        engine->play2D("assets/media/fx/attack.wav");
        counter2=0;
      }
      counter2++;
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

void AttackListener::configure(entityx::EventManager &event_manager) {
  event_manager.subscribe<AttackMessage>(*this);
}

void AttackListener::update(entityx::EntityManager &entities,
                           entityx::EventManager &events,
                           entityx::TimeDelta dt) {}

void AttackListener::receive(const AttackMessage &attackMessage) {
  
}