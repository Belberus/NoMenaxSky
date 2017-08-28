#ifndef ENGINE_COMPONENTS_THREE_D_MODEL_H_
#define ENGINE_COMPONENTS_THREE_D_MODEL_H_

#include <memory>
#include <string>

#include <glm/glm.hpp>

namespace engine {
namespace systems {
namespace three_d {
class ModelRenderer;
}  // namespace three_d
}  // namespace systems
}  // namespace engine

namespace engine {
namespace components {
namespace three_d {
namespace impl {
class ModelImpl;
}  // namespace impl
}  // namespace three_d
}  // namespace components
}  // namespace engine

namespace engine {
namespace components {
namespace three_d {
/// This class represents a 3D model.
class Model {
 public:
  /// Import a model from the COLLADA file who has the specified name.
  /// @param filename the name of the file containing the 3D model. This must
  /// be in COLLADA format.
  Model(const std::string &filename);
  Model(const Model &model);
  Model &operator=(const Model &model);
  Model(Model &&model);
  Model &operator=(Model &&model);
  ~Model();

 private:
  std::unique_ptr<engine::components::three_d::impl::ModelImpl> pimpl_;

  // The system responsible of consuming this class data an rendering the
  // 3D model correctly.
  friend class engine::systems::three_d::ModelRenderer;
};
}  // namespace three_d
}  // namespace components
}  // namespace engine
#endif  // ENGINE_COMPONENTS_THREE_D_MODEL_H_