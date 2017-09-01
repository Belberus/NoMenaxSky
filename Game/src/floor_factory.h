#ifndef FLOOR_FACTORY_H_
#define FLOOR_FACTORY_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <engine/components/two_d/aabb_collider.h>
#include <engine/components/two_d/tilemap.h>
#include <tmxlite/Map.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <tmxlite/TileLayer.hpp>

#include "floor_2d.h"

class FloorFactory {
 public:
  static std::unique_ptr<Floor2D> MakeFloor1(const std::string &file_name);

 private:
  static void ParseTilemap(const tmx::Map &map, Floor2D &floor);
  static engine::components::two_d::Tilemap::Layer ParseTilemapLayer(
      const tmx::TileLayer &tiled_tile_layer, const tmx::Map &tiled_map);
  static void ParseTileObjects(const tmx::Map &map, Floor2D &floor);
  static void ParseStaticColliders(const tmx::Map &map,
                                   const std::string &layer_name, Floor2D &floor);
  static std::unordered_map<std::string, std::unique_ptr<Floor2D::Room>>
  ParseRooms(const tmx::Map &map, const std::string &layer_name);
  static void ParseRoomContents(const tmx::Map &map,
                                const tmx::ObjectGroup &object_layer,
                                Floor2D::Room &room);
};

#endif  // FLOOR_FACTORY_H_
