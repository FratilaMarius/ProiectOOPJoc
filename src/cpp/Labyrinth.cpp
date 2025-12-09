#include "../Labyrinth.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Construcotrs/ destrucotrs:
//
Labyrinth::Labyrinth(int _width, int _height, int plX, int plY) {
  this->width = _width;
  this->height = _height;

  layout.assign(width, std::vector<Room>(height));

  Spawn(plX, plY);
}

Labyrinth::~Labyrinth() {
  for (int i = 0; i < width; ++i)
    layout[i].clear();
  layout.clear();
}

Labyrinth::Labyrinth(const Labyrinth &other) : playerCords{other.playerCords[0], other.playerCords[1]},
                                               width(other.width),
                                               height(other.height),

                                               layout(other.layout),

                                               moves(other.moves),
                                               chanceForExit(other.chanceForExit)
{}

Labyrinth &Labyrinth::operator=(const Labyrinth &other) {
  for (int i = 0; i < width; ++i)
    layout[i].clear();
  layout.clear();

  this->width = other.width;
  this->height = other.height;
  layout.assign(width, std::vector<Room>(height));

  this->layout = other.layout;
  this->playerCords[0] = other.playerCords[0];
  this->playerCords[1] = other.playerCords[1];

  this->moves = other.moves;
  this->chanceForExit = other.chanceForExit;

  return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Gameplay/ dealing with the rooms:
//
void Labyrinth::ResetLayout(int posX, int posY) {
  for (int i = 0; i < width; i++)
    for (int j = 0; j < height; j++)
    {
      if (i == posX && j == posY)
        continue;
      layout[i][j].ResetRoom();
    }
}

void Labyrinth::GenerateRoom(int x, int y, int originX, int originY) { // when entering a new room we give it exits and a texture
  if (layout[x][y].GetTimesVisited() == 0) {

// we delete all the exits it has by default:
    layout[x][y].Exits(0, 0);
    layout[x][y].Exits(1, 0);
    layout[x][y].Exits(2, 0);
    layout[x][y].Exits(3, 0);

///////////////////////////////////////////
// making sure we can go back the way we came:
    int retur = -1;
    if (originX - x < 0) {
      layout[x][y].Exits("up", 1);
      retur = 0;
    }
    if (x - originX < 0) {
      layout[x][y].Exits("down", 1);
      retur = 1;
    }
    if (y - originY < 0) {
      layout[x][y].Exits("right", 1);
      retur = 3;
    }
    if (originY - y < 0) {
      layout[x][y].Exits("left", 1);
      retur = 2;
    }
///////////////////////////////////////////
// we give it some exits depending on this chance:
    int chance = RNG() % 100;
    int chanceFor2 = 97;  // the higher these chances the more likely that's how many exits we generate
    int chanceFor3 = 60;
    int chanceFor4 = 25;
    
    if (chance < chanceFor2) {
      int z = RNG() % 4;
      if (z == retur)
      {
        z += 2;
        z %= 4;
      }
      layout[x][y].Exits(z, 1);
    }
    if ((chance) <= chanceFor3) {
      int z = RNG() % 4;
      if (z == retur)
      {
        z += 2;
        z %= 4;
      }
      layout[x][y].Exits(z, 1);
      if (RNG() % 2)
        z += 1;
      else
        z += 3;
      z %= 4;
      layout[x][y].Exits(z, 1);
    }
    if ((chance) < chanceFor4) {
      layout[x][y].Exits(0, 1);
      layout[x][y].Exits(2, 1);
      layout[x][y].Exits(1, 1);
      layout[x][y].Exits(3, 1);
    }
///////////////////////////////////////////
    layout[x][y].SetSprite();
  }
}



void Labyrinth::Spawn(int x, int y) { // we only call this one in the constructor

/////////////////////////////////////////////
// we give the starting room at least 1 exit
  layout[x][y].Exits("up", RNG() % 2);
  layout[x][y].Exits("down", RNG() % 2);
  layout[x][y].Exits("left", RNG() % 2);
  layout[x][y].Exits("right", RNG() % 2);

  if (layout[x][y].GetNrRoutes() < 1) {
    int z = RNG() % 4;
    layout[x][y].Exits(z, 1);
  }
/////////////////////////////////////////////
  layout[x][y].SetHasPlayer(1);
  layout[x][y].IncrTimesVisited(1);
  playerCords[0] = x;
  playerCords[1] = y;

  layout[playerCords[0]][playerCords[1]].SetSprite();

  std::cout << "\n"
            << layout[playerCords[0]][playerCords[1]];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Gameplay/ movement:
//
int Labyrinth::Move(const std::string& where) {

  shouldDisplayDeadEndText = 0;

/////////////////////////////////////////////////////////////////////////////////////////
// interpreting the param string to see which way we go:
  int newX = -1, newY = -1;
  switch (where[0]) {
  
  case 'u':
    if (playerCords[0] - 1 == -1) { // check for out of bounds
      std::cout << "\nWall\n";
      shouldDisplayDeadEndText = 1;
      break;
    }
    if (!layout[playerCords[0]][playerCords[1]].SeeIfExitHere("up")) {  // if we cant go that way 
      std::cout << "\nDead end\n";
      shouldDisplayDeadEndText = 1;
      break;
    }
    // if the way we want to go has no door facing us:
    if (layout[playerCords[0] - 1][playerCords[1]].GetTimesVisited() != 0 && !layout[playerCords[0] - 1][playerCords[1]].SeeIfExitHere("down")) {
      std::cout << "\nDead end\n";
      shouldDisplayDeadEndText = 1;
      break;
    }

    // we passed the checks, we are moving:
    newX = playerCords[0] - 1;
    newY = playerCords[1];
    break;

  case 'd':
    if (playerCords[0] + 1 == width) { // check for out of bounds
      std::cout << "\nWall\n";
      shouldDisplayDeadEndText = 1;
      break;
    }
    if (!layout[playerCords[0]][playerCords[1]].SeeIfExitHere("down")) { // if we cant go that way 
      std::cout << "\nDead end\n";
      shouldDisplayDeadEndText = 1;
      break;
    }
    // if the way we want to go has no door facing us:
    if (layout[playerCords[0] + 1][playerCords[1]].GetTimesVisited() != 0 && !layout[playerCords[0] + 1][playerCords[1]].SeeIfExitHere("up")) {
      std::cout << "\nDead end\n";
      shouldDisplayDeadEndText = 1;
      break;
    }

    // we passed the checks, we are moving:
    newX = playerCords[0] + 1;
    newY = playerCords[1];
    break;

  case 'l':
    if (playerCords[1] - 1 == -1) { // check for out of bounds
      std::cout << "\nWall\n";
      shouldDisplayDeadEndText = 1;
      break;
    }
    if (!layout[playerCords[0]][playerCords[1]].SeeIfExitHere("left")) { // if we cant go that way 
      std::cout << "\nDead end\n";
      shouldDisplayDeadEndText = 1;
      break;
    }
    // if the way we want to go has no door facing us:
    if (layout[playerCords[0]][playerCords[1] - 1].GetTimesVisited() != 0 && !layout[playerCords[0]][playerCords[1] - 1].SeeIfExitHere("right")) {
      std::cout << "\nDead end\n";
      shouldDisplayDeadEndText = 1;
      break;
    }

    // we passed the checks, we are moving:
    newX = playerCords[0];
    newY = playerCords[1] - 1;
    break;

  case 'r':
    if (playerCords[1] + 1 == height) { // check for out of bounds
      std::cout << "\nWall\n";
      shouldDisplayDeadEndText = 1;
      break;
    }
    if (!layout[playerCords[0]][playerCords[1]].SeeIfExitHere("right")) { // if we cant go that way 
      std::cout << "\nDead end\n";
      shouldDisplayDeadEndText = 1;
      break;
    }
    // if the way we want to go has no door facing us:
    if (layout[playerCords[0]][playerCords[1] + 1].GetTimesVisited() != 0 && !layout[playerCords[0]][playerCords[1] + 1].SeeIfExitHere("left")) {
      std::cout << "\nDead end\n";
      shouldDisplayDeadEndText = 1;
      break;
    }

    // we passed the checks, we are moving:
    newX = playerCords[0];
    newY = playerCords[1] + 1;
    break;

  default:
    break;
  }
  if (newX == -1 && newY == -1) throw(LabExceptionCouldntMove(playerCords[0], playerCords[1]));
  // at the end of the switch() we have newX/newY as the coordonates for out room we need to move into
  // and playercoords[0] and [1] still stores the coords of where we came from

  moves++;

  layout[playerCords[0]][playerCords[1]].SetHasPlayer(0);
  
  if(layout[newX][newY].GetTimesVisited() == 0) {
    GenerateRoom(newX, newY, playerCords[0], playerCords[1]);
    
    // generating an enemy in the new rooom:
    if (RNG() % 100 < 25)
      layout[newX][newY].SetHasEnemy(1);
  }

  layout[newX][newY].SetHasPlayer(1);
  layout[newX][newY].IncrTimesVisited(1);
  
  // if we've already been to this room >3 times we reset the layout, minus the room we're moving to
  if (layout[newX][newY].GetTimesVisited() > 3) { 
    ResetLayout(newY, newX);
  }

  playerCords[0] = newX;
  playerCords[1] = newY;



  if (moves > 20 && chanceForExit > 10) { // after 10 moves we start to get close to the exit
    chanceForExit -= 2;
  }
  if(chanceForExit <= 10 && RNG() % chanceForExit == 0) finish = 1;
  if (finish) {
    std::cout << "Congrats! You have escaped!";
    return 2;
  }
  if(layout [playerCords[0]][playerCords[1]].GetHasEnemy() == 1) return 2;

  std::cout << "\n"
            << layout[playerCords[0]][playerCords[1]];
  return 1;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Printing the labyrinth by timesVisited of each room:
//
std::ostream &operator<<(std::ostream &cout, const Labyrinth &labyrinth)
{
  int iMAX = labyrinth.width;
  int jMAX = labyrinth.height;
  cout << '\n';

  for (int i = 0; i < iMAX; i++)
  {
    for (int j = 0; j < jMAX; j++)
    {
      cout << labyrinth.layout[i][j].GetTimesVisited() << " ";
    }
    cout << '\n';
  }
  cout << '\n';
  return cout;
}