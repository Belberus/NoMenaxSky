#ifndef ENGINE_EVENTS_KEY_RELEASED_H
#define ENGINE_EVENTS_KEY_RELEASED_H
namespace engine {
namespace events {
struct KeyReleased {
  KeyReleased(int key) : key(key) {}
  int key;
};
}  // namespace events
}  // namespace engine
#endif  // ENGINE_EVENTS_KEY_RELEASED_H
