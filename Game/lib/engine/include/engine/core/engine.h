#ifndef ENGINE_CORE_ENGINE_MANAGER_H_
#define ENGINE_CORE_ENGINE_MANAGER_H_

#include <map>
#include <memory>
#include <typeindex>

struct GLFWwindow;
namespace engine {
namespace core {
class Engine {
 public:
  struct Properties {
    int width;
    int height;
    std::string title;
    bool fullscreen;
    bool resizable;
    bool vsync;
  };

  static void Init(const Properties& properties);
  static Engine& GetInstance();
  template <typename T>
  T& Get() {
    return *static_cast<Service<T>*>(
                services_[std::type_index(typeid(T))].get())
                ->provider;
  }
  ~Engine();

 private:
  struct IService {
    virtual ~IService(){};
  };

  template <typename T>
  struct Service : public IService {
    Service(std::unique_ptr<T>&& provider) : provider(std::move(provider)) {}
    std::unique_ptr<T> provider;
  };
  Engine();
  GLFWwindow* glfw_window_;
  std::map<std::type_index, std::unique_ptr<IService>> services_;
  template <typename T, typename... Args>
  void AddServiceProvider(Args&&... args) {
    services_.emplace(
        std::make_pair(std::type_index(typeid(T)),
                       std::make_unique<Service<T>>(
                           std::make_unique<T>(std::forward<Args>(args)...))));
  }
  template <typename T>
  void AddServiceProvider() {
    services_.emplace(
        std::make_pair(std::type_index(typeid(T)),
                       std::make_unique<Service<T>>(std::make_unique<T>())));
  }
  static Properties properties_;
  static std::unique_ptr<Engine> engine_;
};
}  // namespace core
}  // namespace engine
#endif  // ENGINE_CORE_ENGINE_MANAGER_H_
