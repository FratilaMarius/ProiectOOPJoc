#include "../Room.hpp"

Enemy Room::GenerateEnemy(const int _type, const int _hp)
{
  hasEnemy = 1;
  Enemy inamic(_type, _hp);
  return inamic;
}

int Room::Exits(const std::string &where)
{ // pentru setat prin string | up, down, left, right
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
int Room::Exits(int where)
{ // pentru setat prin index | 0-up,1-down,2-left,3-right
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
void Room::Exits(const std::string &where, int val)
{ // ca mai sus dar pentru setat, index string
  if (where == "up")
    exits[0] = val;
  if (where == "down")
    exits[1] = val;
  if (where == "left")
    exits[2] = val;
  if (where == "right")
    exits[3] = val;
}
void Room::Exits(int where, int val)
{ // ca mai sus dar pentru setat, index int
  if (where == 0)
    exits[0] = val;
  if (where == 1)
    exits[1] = val;
  if (where == 2)
    exits[2] = val;
  if (where == 3)
    exits[3] = val;
}
void Room::ResetRoom()
{ // reseteaza complet o camera, fara iesiri, etc
  id = 0;
  exits[0] = 1;
  exits[1] = 1;
  exits[2] = 1;
  exits[3] = 1;
  hasEnemy = 0;
  hasPlayer = 0;
  timesVisited = 0;
}
int Room::FindPickup(int what)
{ // cauta un pickup in camera. 0-2 nimic, 3-apa,4-mancare
  if (checkedForPickups)
  {
    std::cout << "\nYou've already checked here\n";
    return 0;
  }
  checkedForPickups = 1;
  switch (what)
  {
  case 0:
  case 1:
  case 2:
    std::cout << "\nFound nothing\n";
    return 0;
    break;

  case 3:
    std::cout << "\nFound some water\n";
    return 1;
    break;

  case 4:
    std::cout << "\nFound some food\n";
    return 2;
    break;

  default:
    break;
  }
  return 0;
}

void Room::SetSprite()
{
  std::cout<<"\n\nSetSprite() in room: NrROutes e "<< NrRoutes();
  sprite.setTexture(txl::TextureLoader::Instance().GetTexture(NrRoutes()));
}
void Room::FitSpriteToFrmae(const sf::RenderWindow &window) {
  float WinScaleX = window.getSize().x;
  float WinScaleY = window.getSize().y;
  float TextureScaleX = sprite.getTexture().getSize().x;
  float TextureScaleY = sprite.getTexture().getSize().y;

  float scale = std::min(WinScaleX/TextureScaleX, WinScaleY/TextureScaleY);
  sprite.setScale({scale, scale});

  sprite.setOrigin({sprite.getLocalBounds().getCenter().x, sprite.getLocalBounds().getCenter().y}); // centru
  sprite.setPosition({WinScaleX/2, WinScaleY/2});
}  
std::ostream &operator<<(std::ostream &os, const Room &room)
{
  os << "Room("
     << "ID=" << room.id
     << " exits=" << room.exits[0] << "(up) " << room.exits[1] << "(down) "
     << room.exits[2] << "(left) " << room.exits[3]
     << "(right) " << ", hasPlayer=" << room.hasPlayer
     << ", hasEnemy=" << room.hasEnemy
     << ")";
  return os;
}