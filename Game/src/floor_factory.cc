#include "floor_factory.h"

#include <algorithm>
#include <regex>

#include <engine/components/common/camera.h>
#include <engine/components/common/transform.h>
#include <engine/components/three_d/model.h>
#include <engine/components/two_d/sprite.h>
#include <engine/components/two_d/tilemap.h>
#include <engine/core/engine.h>
#include <engine/core/resource_manager.h>
#include <engine/systems/two_d/collider_renderer.h>
#include <engine/systems/two_d/physics.h>
#include <engine/systems/two_d/sprite_animator.h>
#include <engine/systems/two_d/sprite_renderer.h>
#include <engine/systems/two_d/tilemap_renderer.h>
#include <entityx/Entity.h>
#include <tmxlite/TileLayer.hpp>
#include "components.h"
#include "entity_factory_2d.h"
#include "entity_factory_3d.h"
#include "floor_3d.h"
#include "systems.h"

glm::ivec2 GetTilePosition(const tmx::Map &map, int id) {
  auto horizontal_tiles = map.getTilesets()[0].getColumnCount();
  auto vertical_tiles = map.getTilesets()[0].getTileCount() / horizontal_tiles;
  int x = (id - 1) % horizontal_tiles;
  int y = (vertical_tiles - 1) - ((id - 1) / horizontal_tiles);
  return glm::ivec2(x, y);
}

engine::utils::Rectangle GetTextureCoord(const tmx::Map &map, int id) {
  auto tile_pos = GetTilePosition(map, id);
  return engine::utils::Rectangle(
      glm::vec2(tile_pos.x * map.getTileSize().x,
                tile_pos.y * map.getTileSize().y),
      glm::vec2(map.getTileSize().x, map.getTileSize().y));
}

engine::components::two_d::Tilemap::Layer FloorFactory::ParseTilemapLayer(
    const tmx::TileLayer &tiled_tile_layer, const tmx::Map &tiled_map) {
  std::vector<engine::components::two_d::Tilemap::Layer::Tile> tilemap_tiles;
  auto tiled_tiles = tiled_tile_layer.getTiles();
  tilemap_tiles.resize(tiled_tiles.size(),
                       engine::components::two_d::Tilemap::Layer::Tile(0));
  for (const auto &tiled_tile : tiled_tiles) {
    if (tiled_tile.ID == 0) {
      tilemap_tiles.emplace_back(0);
      continue;
    }
    auto tiled_num_tiles_horizontal =
        tiled_map.getTilesets()[0].getColumnCount();
    auto tile_pos = GetTilePosition(tiled_map, tiled_tile.ID);
    int tilemap_tile_id =
        tiled_num_tiles_horizontal * tile_pos.y + tile_pos.x + 1;
    tilemap_tiles.emplace_back(tilemap_tile_id);
  }
  std::reverse(tilemap_tiles.begin(), tilemap_tiles.end());
  auto num_tiles_horizontal = tiled_map.getTileCount().x;
  for (unsigned int i = 0; i < num_tiles_horizontal; ++i) {
    std::reverse(tilemap_tiles.begin() + num_tiles_horizontal * i,
                 tilemap_tiles.begin() + num_tiles_horizontal * (i + 1));
  }
  return engine::components::two_d::Tilemap::Layer(tilemap_tiles);
}

void FloorFactory::ParseTilemap(const tmx::Map &map, Floor &floor) {
  auto texture = engine::core::Engine::GetInstance()
                     .Get<engine::core::ResourceManager>()
                     .Load<engine::components::two_d::Texture>(
                         map.getTilesets()[0].getImagePath());
  std::vector<engine::components::two_d::Tilemap::Layer> tilemap_layers;
  for (auto &layer : map.getLayers()) {
    if (layer->getType() == tmx::Layer::Type::Tile) {
      auto tile_layer = dynamic_cast<tmx::TileLayer *>(layer.get());
      tilemap_layers.push_back(ParseTilemapLayer(*tile_layer, map));
    }
  }
  auto new_entity = floor.entities.create();
  new_entity.assign<engine::components::common::Transform>(glm::vec3(0, 0, 0));
  new_entity.assign<engine::components::two_d::Tilemap>(
      map.getTileCount().x, map.getTileCount().y, map.getTileSize().x,
      tilemap_layers, texture);
}

void FloorFactory::ParseTileObjects(const tmx::Map &map, Floor &floor) {
  auto texture = engine::core::Engine::GetInstance()
                     .Get<engine::core::ResourceManager>()
                     .Load<engine::components::two_d::Texture>(
                         map.getTilesets()[0].getImagePath());
  for (const auto &layer : map.getLayers()) {
    if (layer->getType() == tmx::Layer::Type::Object) {
      auto object_layer = dynamic_cast<tmx::ObjectGroup *>(layer.get());
      for (const auto &object : object_layer->getObjects()) {
        if (object.getTileID() != 0) {
          auto object_pos = object.getPosition();
          auto object_aabb = object.getAABB();
          auto new_entity = floor.entities.create();
          glm::vec3 position(
              object_pos.x + object_aabb.width / 2.0f,
              map.getBounds().height - object_pos.y + object_aabb.height / 2.0f,
              0.0f);
          new_entity.assign<engine::components::common::Transform>(position);
          new_entity.assign<engine::components::two_d::Sprite>(
              texture, GetTextureCoord(map, object.getTileID()));
        }
      }
    }
  }
}

void FloorFactory::ParseStaticColliders(const tmx::Map &map,
                                        const std::string &layer_name,
                                        Floor &floor) {
  auto center = ParseCenter(map);
  for (auto &layer : map.getLayers()) {
    if (layer->getType() == tmx::Layer::Type::Object &&
        layer->getName() == layer_name) {
      auto object_layer = dynamic_cast<tmx::ObjectGroup *>(layer.get());
      for (const auto &object : object_layer->getObjects()) {
        if (object.getTileID() == 0 &&
            object.getShape() == tmx::Object::Shape::Rectangle) {
          auto new_entity = floor.entities.create();
          auto object_aabb = object.getAABB();
          auto object_pos = object.getPosition();
          glm::vec3 position(
              object_pos.x + object_aabb.width / 2.0f,
              map.getBounds().height - object_pos.y - object_aabb.height / 2.0f,
              0.0f);
          position -= center;
          new_entity.assign<engine::components::common::Transform>(position);
          new_entity.assign<engine::components::two_d::AABBCollider>(
              glm::vec2(0.0f, 0.0f),
              glm::vec2(object_aabb.width / 2.0f, object_aabb.height / 2.0f));
          if (object.getType() == "low") {
            new_entity.assign<LowCollision>();
          }
        }
      }
    }
  }
}

std::unordered_map<std::string, std::unique_ptr<Floor::Room>>
FloorFactory::ParseRooms(const tmx::Map &map, const std::string &layer_name,
                         const std::shared_ptr<EntityFactory> &factory, const std::string &level) {
  std::unordered_map<std::string, std::unique_ptr<Floor::Room>> rooms;
  for (auto &layer : map.getLayers()) {
    if (layer->getType() == tmx::Layer::Type::Object &&
        std::regex_match(layer->getName(), std::regex(layer_name))) {
      auto object_layer = dynamic_cast<tmx::ObjectGroup *>(layer.get());
      auto floor = std::make_unique<Floor::Room>();
      ParseRoomContents(map, *object_layer, factory, *floor, level);
      rooms.emplace(layer->getName(), std::move(floor));
    }
  }
  return rooms;
}

void FloorFactory::ParseRoomContents(
    const tmx::Map &map, const tmx::ObjectGroup &object_layer,
    const std::shared_ptr<EntityFactory> &factory, Floor::Room &room, const std::string &level) {

  srand(time(NULL));
  float frecuencias[10] = {250.0f,  500.0f,  750.0f,  1000.0f, 1250.0f,
                           1500.0f, 1750.0f, 2000.0f, 2250.0f, 2500.0f};
  auto center = ParseCenter(map);
  for (const auto &object : object_layer.getObjects()) {
    auto object_aabb = object.getAABB();
    auto object_pos = object.getPosition();
    glm::vec3 position(
        object_pos.x + object_aabb.width / 2.0f,
        map.getBounds().height - object_pos.y - object_aabb.height / 2.0f,
        0.0f);
    position -= center;
    if (object.getType() == "door") {
      engine::components::two_d::AABBCollider collider(
          glm::vec2(0, 0),
          glm::vec2(object_aabb.width / 2.0f, object_aabb.height / 2.0f), true);
      auto properties = object.getProperties();
      Door door(properties[0].getStringValue(), properties[1].getStringValue());
      auto fn_door =
          [=](entityx::EntityManager &em) -> std::vector<entityx::Entity> {
        auto id = em.create();
        id.assign<engine::components::common::Transform>(position);
        id.assign<engine::components::two_d::AABBCollider>(collider);
        id.assign<Door>(door);
        return std::vector<entityx::Entity>({id});
      };
      room.entity_creators_.push_back(fn_door);
    } else if (object.getType() == "bossDoor") {
      engine::components::two_d::AABBCollider collider(
          glm::vec2(0, 0),
          glm::vec2(object_aabb.width / 2.0f, object_aabb.height / 2.0f), true);
      auto properties = object.getProperties();
      BossDoor bossDoor(properties[0].getStringValue(),
                        properties[1].getStringValue());
      std::cerr << level << std::endl;
      if (level == "1") {
        bossDoor.level = "1";
      } else if (level == "2") {
        bossDoor.level = "2";
      } else bossDoor.level = "3";

      auto fn_bossDoor =
          [=](entityx::EntityManager &em) -> std::vector<entityx::Entity> {
        auto id = em.create();
        id.assign<engine::components::common::Transform>(position);
        id.assign<engine::components::two_d::AABBCollider>(collider);
        id.assign<BossDoor>(bossDoor);
        return std::vector<entityx::Entity>({id});
      };
      room.entity_creators_.push_back(fn_bossDoor);
    } else if (object.getType() == "fantasma") {
      auto fn_ghost =
          [=](entityx::EntityManager &em) -> std::vector<entityx::Entity> {
        return factory->MakeGhost(em, position);
      };
      room.entity_creators_.push_back(fn_ghost);
    } else if (object.getType() == "torreta") {
      float frecuencia = frecuencias[(rand() % 3)];
      auto fn_turret =
          [=](entityx::EntityManager &em) -> std::vector<entityx::Entity> {
        return factory->MakeTurret(em, position, frecuencia);
      };
      room.entity_creators_.push_back(fn_turret);
    } else if (object.getType() == "lancero") {
      auto fn_lancer =
          [=](entityx::EntityManager &em) -> std::vector<entityx::Entity> {
        return factory->MakeLancer(em, position);
      };
      room.entity_creators_.push_back(fn_lancer);
    } else if (object.getType() == "trampa") {
      auto properties = object.getProperties();
      std::string direccion = properties[0].getStringValue();
      float frecuencia = frecuencias[(rand() % 10)];
      auto fn_trap =
          [=](entityx::EntityManager &em) -> std::vector<entityx::Entity> {
        return factory->MakeTrap(em, position, direccion, frecuencia);
      };
      room.entity_creators_.push_back(fn_trap);
    } else if (object.getType() == "manueleth") {
      auto fn_manueleth =
          [=](entityx::EntityManager &em) -> std::vector<entityx::Entity> {
        return factory->MakeManueleth(em, position);
      };
      room.entity_creators_.push_back(fn_manueleth);
    } else if (object.getType() == "cofre1") {
      engine::components::two_d::AABBCollider collider(
          glm::vec2(0, 0),
          glm::vec2(object_aabb.width / 2.0f, object_aabb.height / 2.0f), true);
      auto fn_cofre =
          [=](entityx::EntityManager &em) -> std::vector<entityx::Entity> {
        auto id = em.create();
        id.assign<engine::components::common::Transform>(position);
        id.assign<engine::components::two_d::AABBCollider>(collider);
        id.assign<Chest>(false);
        return std::vector<entityx::Entity>({id});
      };
      room.entity_creators_.push_back(fn_cofre);
    } else if (object.getType() == "cofre2") {
      engine::components::two_d::AABBCollider collider(
          glm::vec2(0, 0),
          glm::vec2(object_aabb.width / 2.0f, object_aabb.height / 2.0f), true);
      auto fn_cofre =
          [=](entityx::EntityManager &em) -> std::vector<entityx::Entity> {
        auto id = em.create();
        id.assign<engine::components::common::Transform>(position);
        id.assign<engine::components::two_d::AABBCollider>(collider);
        id.assign<Chest>(true);
        return std::vector<entityx::Entity>({id});
      };
      room.entity_creators_.push_back(fn_cofre);
    } else if (object.getType() == "palanca1") {
      engine::components::two_d::AABBCollider collider(
          glm::vec2(0, 0),
          glm::vec2(object_aabb.width / 2.0f, object_aabb.height / 2.0f), true);
      auto fn_palanca =
          [=](entityx::EntityManager &em) -> std::vector<entityx::Entity> {
        auto id = em.create();
        id.assign<engine::components::common::Transform>(position);
        id.assign<engine::components::two_d::AABBCollider>(collider);
        id.assign<Lever>("1");
        return std::vector<entityx::Entity>({id});
      };
      room.entity_creators_.push_back(fn_palanca);
    } else if (object.getType() == "palanca2") {
      engine::components::two_d::AABBCollider collider(
          glm::vec2(0, 0),
          glm::vec2(object_aabb.width / 2.0f, object_aabb.height / 2.0f), true);
      auto fn_palanca =
          [=](entityx::EntityManager &em) -> std::vector<entityx::Entity> {
        auto id = em.create();
        id.assign<engine::components::common::Transform>(position);
        id.assign<engine::components::two_d::AABBCollider>(collider);
        id.assign<Lever>("2");
        return std::vector<entityx::Entity>({id});
      };
      room.entity_creators_.push_back(fn_palanca);
    } else if (object.getType() == "masiatrix") {
      auto properties = object.getProperties();
      std::string id = properties[0].getStringValue();
      std::cerr << "Las creo" << std::endl;
      auto fn_masiatrix =
          [=](entityx::EntityManager &em) -> std::vector<entityx::Entity> {
        return factory->MakeMasiatrix(em, position, id);
      };
      room.entity_creators_.push_back(fn_masiatrix);
    } 
  }
}

std::unique_ptr<Floor> FloorFactory::MakeFloorOne2D(
    const std::string &file_name, Game *parent_scene,  const std::string &role) {
  auto floor = std::make_unique<Floor2D>(parent_scene);
  std::shared_ptr<EntityFactory> factory(std::make_shared<EntityFactory2D>());
  tmx::Map tiled_map;
  tiled_map.load(file_name);
  ParseTilemap(tiled_map, *floor);
  ParseTileObjects(tiled_map, *floor);
  ParseStaticColliders(tiled_map, "Colisiones", *floor);
  floor->rooms_ = ParseRooms(tiled_map, "(1\\.\\d*)", factory, "1");
  floor->current_room_ = "1.0";
  floor->rooms_[floor->current_room_]->Load(*floor);

  auto camera = floor->entities.create();
  camera.assign<engine::components::common::Transform>(
      glm::vec3(1376.0f, 640.0f, 1.0f));
  camera.assign<engine::components::common::Camera>(512.0f, 288.0f, 0.1f,
                                                    1000.0f);
  if (role == "knight") {
    factory->MakeKnight(floor->entities, glm::vec3(1376.0f, 640.0f, 0));
  } else {
    factory->MakeWizard(floor->entities, glm::vec3(1376.0f, 640.0f, 0));
  }
  return floor;
}

std::unique_ptr<Floor> FloorFactory::MakeFloorTwo2D(
    const std::string &file_name, Game *parent_scene,  const std::string &role) {
  auto floor = std::make_unique<Floor2D>(parent_scene);
  std::shared_ptr<EntityFactory> factory(std::make_shared<EntityFactory2D>());
  tmx::Map tiled_map;
  tiled_map.load(file_name);
  ParseTilemap(tiled_map, *floor);
  ParseTileObjects(tiled_map, *floor);
  ParseStaticColliders(tiled_map, "Colisiones", *floor);
  floor->rooms_ = ParseRooms(tiled_map, "(2\\.\\d*)", factory, "2");
  floor->current_room_ = "2.0";
  floor->rooms_[floor->current_room_]->Load(*floor);
  auto camera = floor->entities.create();
  camera.assign<engine::components::common::Transform>(
      glm::vec3(1006.0f, 2863.0f, 1.0f));
  camera.assign<engine::components::common::Camera>(512.0f, 288.0f, 0.1f,
                                                    1000.0f);
  if (role == "knight") {
    factory->MakeKnight(floor->entities, glm::vec3(1006.0f, 2863.0f, 0));
  } else {
    factory->MakeWizard(floor->entities, glm::vec3(1006.0f, 2863.0f, 0));
  } 
  return floor;
}

std::unique_ptr<Floor> FloorFactory::MakeFloorThree2D(
    const std::string &file_name, Game *parent_scene,  const std::string &role) {
  auto floor = std::make_unique<Floor2D>(parent_scene);
  std::shared_ptr<EntityFactory> factory(std::make_shared<EntityFactory2D>());
  tmx::Map tiled_map;
  tiled_map.load(file_name);
  ParseTilemap(tiled_map, *floor);
  ParseTileObjects(tiled_map, *floor);
  ParseStaticColliders(tiled_map, "Colisiones", *floor);
  floor->rooms_ = ParseRooms(tiled_map, "(3\\.\\d*)", factory, "3");
  floor->current_room_ = "3.0";
  floor->rooms_[floor->current_room_]->Load(*floor);

  auto camera = floor->entities.create();
  camera.assign<engine::components::common::Transform>(
      glm::vec3(288.0f, 3272.0f, 1.0f));
  camera.assign<engine::components::common::Camera>(512.0f, 288.0f, 0.1f,
                                                    1000.0f);

  if (role == "knight") {
    factory->MakeKnight(floor->entities, glm::vec3(288.0f, 3272.0f, 0));
  } else {
    factory->MakeWizard(floor->entities, glm::vec3(288.0f, 3272.0f, 0));
  }
  return floor;
}

std::unique_ptr<Floor> FloorFactory::MakeFloorOne3D(
    const std::string &file_name, Game *parent_scene, const std::string &role) {
  // TODO: move this line somewhere else
  engine::core::Engine::GetInstance().EnableDepthTest(
      engine::core::Engine::DepthTest::kLess);
  std::unique_ptr<Floor> floor(std::make_unique<Floor3D>(parent_scene));
  std::shared_ptr<EntityFactory> factory(std::make_shared<EntityFactory3D>());
  // create the floor model
  auto floor_model = floor->entities.create();
  floor_model.assign<engine::components::three_d::Model>(
      "assets/3d/castillo/planta1/planta1.dae");
  engine::components::common::Transform floor_transform(
      glm::vec3(0.0f, 0.0f, 0.0f));
  glm::quat floor_model_rotation;
  floor_model_rotation = glm::rotate(floor_model_rotation, glm::radians(180.0f),
                                     glm::vec3(0.0f, 0.0f, 1.0f));
  floor_transform.SetLocalOrientation(floor_model_rotation);
  floor_model.assign<engine::components::common::Transform>(floor_transform);
  // create the camera
  auto camera = floor->entities.create();
  camera.assign<engine::components::common::Camera>(glm::radians(75.0f), 160.0f,
                                                    60.0f, 0.1f, 1000.0f);
  engine::components::common::Transform camera_transform(
      glm::vec3(0.0f, 0.0f, 30.0f));
  camera.assign<engine::components::common::Transform>(camera_transform);
  // create the player

  if(role == "knight"){
    factory->MakeKnight(floor->entities, glm::vec3(0.0f, 0.0f, 7.0f));
    //factory->MakeGhost(floor->entities, glm::vec3(0.0f,0.0f,7.0f));    
  }
  else{
    //factory->MakeKnight(floor->entities, glm::vec3(0.0f, 0.0f, 7.0f));
    factory->MakeWizard(floor->entities, glm::vec3(0.0f, 0.0f, 7.0f));
  }
  
  // create the colliders
  tmx::Map tiled_map;
  tiled_map.load("assets/test/nivel1.tmx");
  ParseStaticColliders(tiled_map, "StaticColliders", *floor);
  floor->rooms_ = ParseRooms(tiled_map, "(1\\.\\d*)", factory, "1");
  floor->current_room_ = "1.0";
  floor->rooms_[floor->current_room_]->Load(*floor);
  return floor;
}

glm::vec3 FloorFactory::ParseCenter(const tmx::Map &map) {
  glm::vec3 center;
  for (auto &layer : map.getLayers()) {
    if (layer->getType() == tmx::Layer::Type::Object &&
        layer->getName() == "Center") {
      auto object_layer = dynamic_cast<tmx::ObjectGroup *>(layer.get());
      for (const auto &object : object_layer->getObjects()) {
        if (object.getTileID() == 0 &&
            object.getShape() == tmx::Object::Shape::Rectangle) {
          auto object_aabb = object.getAABB();
          auto object_pos = object.getPosition();
          center = glm::vec3(
              object_pos.x + object_aabb.width / 2.0f,
              map.getBounds().height - object_pos.y - object_aabb.height / 2.0f,
              0.0f);
        }
      }
    }
  }
  return center;
}