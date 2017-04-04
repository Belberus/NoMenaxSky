#include "Messages.h"

DeathMessage::DeathMessage(entityx::Entity e) : entity(e) {}
GoToRoomMessage::GoToRoomMessage(int room, int fromWhere) : room(room) , fromWhere(fromWhere){
}