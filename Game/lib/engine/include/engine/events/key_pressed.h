#ifndef ENGINE_EVENTS_KEY_PRESSED_H_
#define ENGINE_EVENTS_KEY_PRESSED_H_
namespace engine {
namespace events {
struct KeyPressed {
  KeyPressed(int key) : key(key) {}
  int key;
};
}  // namespace events
}  // namespace engine
#endif  // ENGINE_EVENTS_KEY_PRESSED_H
