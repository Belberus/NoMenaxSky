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

#include "entity_factory.h"
#include "floor_2d.h"

class FloorFactory {
 public:
  static std::unique_ptr<Floor> MakeFloorOne2D(const std::string &file_name,
                                               Game *parent_scene, const std::string &role);
  static std::unique_ptr<Floor> MakeFloorTwo2D(const std::string &file_name,
                                               Game *parent_scene, const std::string &role);
  static std::unique_ptr<Floor> MakeFloorThree2D(const std::string &file_name,
                                               Game *parent_scene, const std::string &role);
  static std::unique_ptr<Floor> MakeFloorOne3D(const std::string &file_name,
                                               Game *parent_scene);

 private:
  static void ParseTilemap(const tmx::Map &map, Floor &floor);

  static engine::components::two_d::Tilemap::Layer ParseTilemapLayer(
      const tmx::TileLayer &tiled_tile_layer, const tmx::Map &tiled_map);

  static void ParseTileObjects(const tmx::Map &map, Floor &floor);

  static void ParseStaticColliders(const tmx::Map &map,
                                   const std::string &layer_name, Floor &floor);

  static std::unordered_map<std::string, std::unique_ptr<Floor::Room>>
  ParseRooms(const tmx::Map &map, const std::string &layer_name,
             const std::shared_ptr<EntityFactory> &factory, const std::string &level);

  static void ParseRoomContents(const tmx::Map &map,
                                const tmx::ObjectGroup &object_layer,
                                const std::shared_ptr<EntityFactory> &factory,
                                Floor::Room &room, const std::string &level);

  // NOTE: placing 3d objects in the level
  // For generating the 3D levels we keep using Tiled; we take an image of our
  // 3D levels running in engine with the orthographic camera (width and height
  // should be the same as the viewport), we paste that in Tiled and we start
  // positioning the different objects.

  /// Searches a layer named "Center" that should have a single rectangle
  /// collider, the position of this collider is considererd to be the center of
  /// the map, which means that all of the objects will be placed relative to
  /// it.
  /// @param map tiled map.
  /// @return center of the map; (0,0,0) if the layer doesnt exist or if it
  /// exists but doesnt have a rectangle collider.
  static glm::vec3 ParseCenter(const tmx::Map &map);
};

#endif  // FLOOR_FACTORY_H_
