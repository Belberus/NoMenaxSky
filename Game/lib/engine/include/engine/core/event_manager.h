#ifndef ENGINE_CORE_EVENT_MANAGER_H_
#define ENGINE_CORE_EVENT_MANAGER_H_
#include <entityx/entityx.h>
namespace engine {
namespace core {
  class EventManager {
  public:
    EventManager() : event_manager_() {};

    template <typename E, typename R>
    void Subscribe(R& receiver) {
      event_manager_.subscribe<E, R>(receiver);
    }

    template <typename E, typename... Args>
    void Emit(Args &&... args) {
      event_manager_.emit<E>(std::forward<Args>(args)...);
    }

  private:
    entityx::EventManager event_manager_;
  };
} // namespace core
} // namespace engine


#endif  // ENGINE_CORE_EVENT_MANAGER_H_