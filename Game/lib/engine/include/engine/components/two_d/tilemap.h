#ifndef ENGINE_COMPONENTS_TWO_D_TILEMAP_H_
#define ENGINE_COMPONENTS_TWO_D_TILEMAP_H_
#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "engine/components/two_d/texture.h"
#include "engine/systems/two_d/tilemap_renderer.h"

namespace engine {
namespace components {
namespace two_d {
class Tilemap {
 public:
  class Layer {
   public:
    class Tile {
     public:
      Tile(int id) : id(id) {}

     private:
      friend class Layer;
      friend class engine::systems::two_d::TilemapRenderer;
      int id;
    };

    Layer(const std::vector<Tile> &tiles) : tiles_(tiles) {}

   private:
    friend class Tilemap;
    friend class engine::systems::two_d::TilemapRenderer;
    std::vector<Tile> tiles_;
  };

  Tilemap(int num_tiles_horizontal, int num_tiles_vertical, int tile_size,
          const std::vector<Layer> &layers,
          const std::shared_ptr<Texture> &texture_atlas)
      : num_tiles_horizontal_(num_tiles_horizontal),
        num_tiles_vertical_(num_tiles_vertical),
        tile_size_(tile_size),
        texture_atlas_(texture_atlas),
        layers_(layers){};

 private:
  Tilemap();
  int num_tiles_vertical_;
  int num_tiles_horizontal_;
  int tile_size_;
  std::vector<Layer> layers_;
  std::shared_ptr<Texture> texture_atlas_;
  friend class engine::systems::two_d::TilemapRenderer;
};
}  // namespace two_d
}  // namespace components
}  // namespace engine

#endif  // ENGINE_COMPONENTS_TWO_D_TILEMAP_H_
