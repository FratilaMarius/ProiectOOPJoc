#include "../Room.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////
// printing and setting the exits:

// where = up, down, left, right
int Room::SeeIfExitHere(const std::string &where) {
  if (where == "up")
    return exits[0];
  if (where == "down")
    return exits[1];
  if (where == "left")
    return exits[2];
  if (where == "right")
    return exits[3];
  return -1;
}
// where = 0-up,1-down,2-left,3-right
int Room::Exits(int where) {
  if (where == 0)
    return exits[0];
  if (where == 1)
    return exits[1];
  if (where == 2)
    return exits[2];
  if (where == 3)
    return exits[3];
  return -1;
}
// setting via string
void Room::Exits(const std::string &where, int val) {
  if (where == "up")
    exits[0] = val;
  if (where == "down")
    exits[1] = val;
  if (where == "left")
    exits[2] = val;
  if (where == "right")
    exits[3] = val;
}
// setting via index
void Room::Exits(int where, int val) {
  if (where == 0)
    exits[0] = val;
  if (where == 1)
    exits[1] = val;
  if (where == 2)
    exits[2] = val;
  if (where == 3)
    exits[3] = val;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// Gameplay:

// resets a room back to the default | SPRITE IS LEFT UNCHANGED UNTIL SetSprite() IS CALLED
void Room::ResetRoom() {
  exits[0] = 1;
  exits[1] = 1;
  exits[2] = 1;
  exits[3] = 1;

  hasEnemy = 0;
  hasPlayer = 0;
  timesVisited = 0;
}
// work in progress:
Enemy Room::GenerateEnemy(const int _type, const int _hp) {
  hasEnemy = 1;
  Enemy enemy(_type, _hp);
  return enemy;
}
// looks for a pickup, takes the type of item as param: 0/1/2 = nothing, 3 = water, 4 = food
int Room::FindPickup(int what) {
  if (checkedForPickups) {
    std::cout << "\nYou've already checked here";
    return 0;
  }
  checkedForPickups = 1;
  switch (what) {
  case 0:
  case 1:
  case 2:
    std::cout << "\nFound nothing";
    return 0;
    break;

  case 3:
    std::cout << "\nFound some water";
    return 1;
    break;

  case 4:
    std::cout << "\nFound some food";
    return 2;
    break;

  default:
    break;
  }
  return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// GFX:

// Gets a random texture for the sprite depending on GetNrRoutes()
void Room::SetSprite() {
  sprite.setTexture(txl::TextureLoader::Instance().GetTexture(GetNrRoutes()));
}
// Centers/aligns/scales the sprite
void Room::FitSpriteToFrmae(const sf::RenderWindow &window) {
  float WinScaleX = window.getSize().x;
  float WinScaleY = window.getSize().y;
  float TextureScaleX = sprite.getTexture().getSize().x;
  float TextureScaleY = sprite.getTexture().getSize().y;

  float scale = std::min(WinScaleX/TextureScaleX, WinScaleY/TextureScaleY);
  sprite.setScale({scale, scale});

  sprite.setOrigin({sprite.getLocalBounds().getCenter().x, sprite.getLocalBounds().getCenter().y}); // center
  sprite.setPosition({WinScaleX/2, WinScaleY/2});
}  


std::ostream &operator<<(std::ostream &os, const Room &room) {
  os << "\nRoom("
     << " exits=" 
      << room.exits[0] << "(up) " 
      << room.exits[1] << "(down) "
      << room.exits[2] << "(left) "
      << room.exits[3]<< "(right) "
     << ", hasEnemy=" << room.hasEnemy
     << ")";
  return os;
}