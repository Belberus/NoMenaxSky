#ifndef ENGINE_CORE_RESOURCE_MANAGER_H_
#define ENGINE_CORE_RESOURCE_MANAGER_H_
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>

#include "engine/components/two_d/texture.h"

namespace engine {
namespace core {
class ResourceManager {
 public:
  ResourceManager() {
    resources_.emplace(
        std::type_index(typeid(engine::components::two_d::Texture)),
        std::unordered_map<std::string, std::unique_ptr<IResource>>());
  }
  template <typename T>
  std::shared_ptr<T> Load(const std::string &resource_name) {
    auto resource_type = resources_.find(std::type_index(typeid(T)));
    if (resource_type != resources_.end()) {
      auto t_type_resource = (*resource_type).second.find(resource_name);
      if (t_type_resource != (*resource_type).second.end()) {
        return (static_cast<Resource<T> *>((*t_type_resource).second.get()))
            ->managed_resource;
      } else {
        auto new_resource = std::make_shared<T>(resource_name);
        (*resource_type)
            .second.emplace(resource_name, std::unique_ptr<IResource>(
                                               new Resource<T>(new_resource)));
        return new_resource;
      }
    } else {
      return std::shared_ptr<T>(nullptr);
    }
  }

 private:
  struct IResource {};
  template <typename T>
  struct Resource : public IResource {
    Resource(std::shared_ptr<T> &resource) : managed_resource(resource) {}
    std::shared_ptr<T> managed_resource;
  };
  std::unordered_map<
      std::type_index,
      std::unordered_map<std::string, std::unique_ptr<IResource>>>
      resources_;
};
}  // namespace core
}  // namespace engine
#endif  // ENGINE_CORE_RESOURCE_MANAGER_H_