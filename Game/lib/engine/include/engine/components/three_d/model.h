#ifndef ENGINE_COMPONENTS_THREE_D_MODEL_H_
#define ENGINE_COMPONENTS_THREE_D_MODEL_H_

#include <memory>
#include <string>

namespace engine {
namespace components {
namespace three_d {
/// This class represents a 3D model.
class Model {
 public:
  /// Import a model from the COLLADA file who has the specified name.
  /// @param filename the name of the file containing the file. The must have he
  /// COLLADA format.
  Model(const std::string &filename);

 private:
  struct Impl;
  std::unique_ptr<Impl> pimpl_;
};
}  // namespace three_d
}  // namespace components
}  // namespace engine
#endif  // ENGINE_COMPONENTS_THREE_D_MODEL_H_