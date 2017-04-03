#include "Messages.h"

DeathMessage::DeathMessage(entityx::Entity e) : entity(e) {}
GoToRoomMessage::GoToRoomMessage(int room) : room(room) {
}