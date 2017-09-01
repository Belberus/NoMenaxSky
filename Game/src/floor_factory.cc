#include "floor_factory.h"

#include <algorithm>
#include <regex>

#include <engine/components/common/camera.h>
#include <engine/components/common/transform.h>
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
#include "entity_factory.h"
#include "systems.h"

namespace cmp = engine::components;
namespace sys = engine::systems;
namespace utils = engine::utils;

glm::ivec2 GetTilePosition(const tmx::Map &map, int id) {
  auto horizontal_tiles = map.getTilesets()[0].getColumnCount();
  auto vertical_tiles = map.getTilesets()[0].getTileCount() / horizontal_tiles;
  int x = (id - 1) % horizontal_tiles;
  int y = (vertical_tiles - 1) - ((id - 1) / horizontal_tiles);
  return glm::ivec2(x, y);
}

utils::Rectangle GetTextureCoord(const tmx::Map &map, int id) {
  auto tile_pos = GetTilePosition(map, id);
  return utils::Rectangle(glm::vec2(tile_pos.x * map.getTileSize().x,
                                    tile_pos.y * map.getTileSize().y),
                          glm::vec2(map.getTileSize().x, map.getTileSize().y));
}

cmp::two_d::Tilemap::Layer FloorFactory::ParseTilemapLayer(
    const tmx::TileLayer &tiled_tile_layer, const tmx::Map &tiled_map) {
  std::vector<cmp::two_d::Tilemap::Layer::Tile> tilemap_tiles;
  auto tiled_tiles = tiled_tile_layer.getTiles();
  tilemap_tiles.resize(tiled_tiles.size(), cmp::two_d::Tilemap::Layer::Tile(0));
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
  return cmp::two_d::Tilemap::Layer(tilemap_tiles);
}

void FloorFactory::ParseTilemap(const tmx::Map &map, Floor &floor) {
  auto texture =
      engine::core::Engine::GetInstance()
          .Get<engine::core::ResourceManager>()
          .Load<cmp::two_d::Texture>(map.getTilesets()[0].getImagePath());
  std::vector<cmp::two_d::Tilemap::Layer> tilemap_layers;
  for (auto &layer : map.getLayers()) {
    if (layer->getType() == tmx::Layer::Type::Tile) {
      auto tile_layer = dynamic_cast<tmx::TileLayer *>(layer.get());
      tilemap_layers.push_back(ParseTilemapLayer(*tile_layer, map));
    }
  }
  auto new_entity = floor.entities.create();
  new_entity.assign<cmp::common::Transform>(glm::vec3(0, 0, 0));
  new_entity.assign<cmp::two_d::Tilemap>(
      map.getTileCount().x, map.getTileCount().y, map.getTileSize().x,
      tilemap_layers, texture);
}

void FloorFactory::ParseTileObjects(const tmx::Map &map, Floor &floor) {
  auto texture =
      engine::core::Engine::GetInstance()
          .Get<engine::core::ResourceManager>()
          .Load<cmp::two_d::Texture>(map.getTilesets()[0].getImagePath());
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
          new_entity.assign<cmp::common::Transform>(position);
          new_entity.assign<cmp::two_d::Sprite>(
              texture, GetTextureCoord(map, object.getTileID()));
        }
      }
    }
  }
}

void FloorFactory::ParseStaticColliders(const tmx::Map &map,
                                        const std::string &layer_name,
                                        Floor &floor) {
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
          new_entity.assign<cmp::common::Transform>(position);
          new_entity.assign<cmp::two_d::AABBCollider>(
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
FloorFactory::ParseRooms(const tmx::Map &map, const std::string &layer_name) {
  ;
  std::unordered_map<std::string, std::unique_ptr<Floor::Room>> rooms;
  for (auto &layer : map.getLayers()) {
    if (layer->getType() == tmx::Layer::Type::Object &&
        std::regex_match(layer->getName(), std::regex(layer_name))) {
      auto object_layer = dynamic_cast<tmx::ObjectGroup *>(layer.get());
      auto floor = std::make_unique<Floor::Room>();
      ParseRoomContents(map, *object_layer, *floor);
      rooms.emplace(layer->getName(), std::move(floor));
    }
  }
  return rooms;
}

void FloorFactory::ParseRoomContents(const tmx::Map &map,
                                     const tmx::ObjectGroup &object_layer,
                                     Floor::Room &room) {
  srand(time(NULL));
  float frecuencias [10] = {250.0f, 500.0f, 750.0f, 1000.0f, 1250.0f, 1500.0f, 1750.0f, 2000.0f, 2250.0f, 2500.0f};

  for (const auto &object : object_layer.getObjects()) {
    auto object_aabb = object.getAABB();
    auto object_pos = object.getPosition();
    glm::vec3 position(
        object_pos.x + object_aabb.width / 2.0f,
        map.getBounds().height - object_pos.y - object_aabb.height / 2.0f,
        0.0f);
    if (object.getType() == "door") {
      cmp::two_d::AABBCollider collider(
          glm::vec2(0, 0),
          glm::vec2(object_aabb.width / 2.0f, object_aabb.height / 2.0f), true);
      auto properties = object.getProperties();
      Door door(properties[0].getStringValue(), properties[1].getStringValue());
      auto fn_door =
          [=](entityx::EntityManager &em) -> std::vector<entityx::Entity> {
        auto id = em.create();
        id.assign<cmp::common::Transform>(position);
        id.assign<cmp::two_d::AABBCollider>(collider);
        id.assign<Door>(door);
        return std::vector<entityx::Entity>({id});
      };
      room.entity_creators_.push_back(fn_door);
    } else if (object.getType() == "bossDoor") {
      cmp::two_d::AABBCollider collider(
          glm::vec2(0, 0),
          glm::vec2(object_aabb.width / 2.0f, object_aabb.height / 2.0f), true);
      auto properties = object.getProperties();
      BossDoor bossDoor(properties[0].getStringValue(), properties[1].getStringValue());
      auto fn_bossDoor =
          [=](entityx::EntityManager &em) -> std::vector<entityx::Entity> {
        auto id = em.create();
        id.assign<cmp::common::Transform>(position);
        id.assign<cmp::two_d::AABBCollider>(collider);
        id.assign<BossDoor>(bossDoor);
        return std::vector<entityx::Entity>({id});
      };
      room.entity_creators_.push_back(fn_bossDoor);
    } else if (object.getType() == "fantasma") {
      auto fn_ghost =
          [=](entityx::EntityManager &em) -> std::vector<entityx::Entity> {
        return EntityFactory::MakeGhost(em, position);
      };
      room.entity_creators_.push_back(fn_ghost);
    } else if (object.getType() == "torreta") {
      float frecuencia = frecuencias[(rand() % 3)];
      auto fn_turret =
          [=](entityx::EntityManager &em) -> std::vector<entityx::Entity> {
        return EntityFactory::MakeTurret(em, position, frecuencia);
      };
      room.entity_creators_.push_back(fn_turret);
    } else if (object.getType() == "trampa") {
      auto properties = object.getProperties();
      std::string direccion = properties[0].getStringValue();
      float frecuencia = frecuencias[(rand() % 10)];
      auto fn_trap =
          [=](entityx::EntityManager &em) -> std::vector<entityx::Entity> {
        return EntityFactory::MakeTrap(em, position, direccion, frecuencia);
      };
      room.entity_creators_.push_back(fn_trap);
    } else if (object.getType() == "manueleth") {
      auto fn_manueleth =
          [=](entityx::EntityManager &em) -> std::vector<entityx::Entity> {
        return EntityFactory::MakeManueleth(em, position);
      };
      room.entity_creators_.push_back(fn_manueleth);
    } else if (object.getType() == "cofre1") {
      cmp::two_d::AABBCollider collider(
          glm::vec2(0, 0),
          glm::vec2(object_aabb.width / 2.0f, object_aabb.height / 2.0f), true);
      auto fn_cofre =
          [=](entityx::EntityManager &em) -> std::vector<entityx::Entity> {
        auto id = em.create();
        id.assign<cmp::common::Transform>(position);
        id.assign<cmp::two_d::AABBCollider>(collider);
        id.assign<Chest>(false);
        return std::vector<entityx::Entity>({id});
      };
      room.entity_creators_.push_back(fn_cofre);
    } else if (object.getType() == "cofre2") {

      cmp::two_d::AABBCollider collider(
          glm::vec2(0, 0),
          glm::vec2(object_aabb.width / 2.0f, object_aabb.height / 2.0f), true);
      auto fn_cofre =
          [=](entityx::EntityManager &em) -> std::vector<entityx::Entity> {
        auto id = em.create();
        id.assign<cmp::common::Transform>(position);
        id.assign<cmp::two_d::AABBCollider>(collider);
        id.assign<Chest>(true);
        return std::vector<entityx::Entity>({id});
      };
      room.entity_creators_.push_back(fn_cofre);
    }
  }
}

std::unique_ptr<Floor> FloorFactory::MakeFloor1(const std::string &file_name) {
  auto floor = std::make_unique<Floor>();
  tmx::Map tiled_map;
  tiled_map.load(file_name);
  ParseTilemap(tiled_map, *floor);
  ParseTileObjects(tiled_map, *floor);
  ParseStaticColliders(tiled_map, "Colisiones", *floor);
  floor->rooms_ = ParseRooms(tiled_map, "(1\\.\\d*)");
  floor->current_room_ = "1.0";
  floor->rooms_[floor->current_room_]->Load(*floor);

  auto camera = floor->entities.create();
  camera.assign<cmp::common::Transform>(glm::vec3(1376.0f, 640.0f, 1.0f));
  camera.assign<cmp::common::Camera>(512.0f, 288.0f, 0.1f, 1000.0f);
  EntityFactory::MakeKnight(floor->entities, glm::vec3(1376.0f, 640.0f, 0));
  return floor;
}
