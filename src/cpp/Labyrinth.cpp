#include "../Labyrinth.hpp"

Labyrinth::Labyrinth(int _width, int _height, int plX, int plY)
{
  this->width = _width;
  this->height = _height;
  layout.assign(width, std::vector<Room>(height));
  Spawn(plX, plY);
}
Labyrinth::~Labyrinth()
{
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
{
}

Labyrinth &Labyrinth::operator=(const Labyrinth &other)
{
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
void Labyrinth::ResetLayout(int posX, int posY)
{
  for (int i = 0; i < width; i++)
    for (int j = 0; j < height; j++)
    {
      if (i == posX && j == posY)
        continue;
      layout[i][j].ResetRoom();
    }
}
void Labyrinth::GenerateRoom(int x, int y, int originX, int originY)
{ // cand intram intr o camera noua daca e goala o generam
  if (layout[x][y].Id() == 0)
  {
    moves++;
    layout[x][y].Exits(0, 0);
    layout[x][y].Exits(1, 0);
    layout[x][y].Exits(2, 0);
    layout[x][y].Exits(3, 0);
    layout[x][y].Id(1);
    int retur = -1;
    if (originX - x < 0)
    {
      layout[x][y].Exits("up", 1);
      retur = 0;
    } // daca a venit de sus setam iesire in sus
    if (x - originX < 0)
    {
      layout[x][y].Exits("down", 1);
      retur = 1;
    } // daca a venit de jos setam iesire in jos
    if (y - originY < 0)
    {
      layout[x][y].Exits("right", 1);
      retur = 3;
    } // daca a venit din dreapta setam iesire in jos
    if (originY - y < 0)
    {
      layout[x][y].Exits("left", 1);
      retur = 2;
    } // daca a venit din stanga setam iesire in jos
    int chance = RNG() % 100;
    if (chance < 97)
    { // facem doar cu 2 iesiri
      int z = RNG() % 4;
      if (z == retur)
      {
        z += 2;
        z %= 4;
      }
      layout[x][y].Exits(z, 1);
    }
    if ((chance) <= 60)
    { // facem doar cu 3 iesiri
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
    if ((chance) < 25)
    { // facem cu 4 iesiri
      layout[x][y].Exits(0, 1);
      layout[x][y].Exits(2, 1);
      layout[x][y].Exits(1, 1);
      layout[x][y].Exits(3, 1);
    }
    if (RNG() % chanceForExit == 0)
    {
      finish = 1;
    }
  } // o camera noua are sigur cale de intoarcere + o alta cale
  layout[x][y].SetSprite();
}
void Labyrinth::Spawn(int x, int y)
{ // functia de mai sus dar apelata la inceput
  layout[x][y].Id(1);

  layout[x][y].Exits("up", RNG() % 2);
  layout[x][y].Exits("down", RNG() % 2);
  layout[x][y].Exits("left", RNG() % 2);
  layout[x][y].Exits("right", RNG() % 2);
  if (layout[x][y].NrRoutes() < 1)
  {
    int z = RNG() % 4;
    layout[x][y].Exits(z, 1);
  }
  layout[x][y].HasPlayer(1);
  layout[x][y].TimesVisited(1);
  playerCords[0] = x;
  playerCords[1] = y;

  std::cout << "\n"
            << layout[playerCords[0]][playerCords[1]] << "\n"
            << "moves=" << moves << " cFE=" << chanceForExit << "\n";
  layout[playerCords[0]][playerCords[1]].SetSprite();
} ///// Spawn e o functie apelata de constructor. genereaza o camera si plaseaza playerul in ea. camera are minim o iesire
int Labyrinth::Move(std::string where)
{
  if (layout[playerCords[0]][playerCords[1]].HasEnemy())
  {
    std::cout << "Enemy encountered!";
  }

  int newX = -1, newY = -1;
  switch (where[0])
  {
  case 'u':
    if (playerCords[0] - 1 == -1) // daca vrea sa mearga in (up) dar iese din lab
    {
      std::cout << "\nWall\n";
      break;
    }
    if (!layout[playerCords[0]][playerCords[1]].Exits("up"))
    {
      std::cout << "\nDead end\n";
      break;
    } /// daca casuta curenta nu il lasa sa mearga in sus
    if (layout[playerCords[0] - 1][playerCords[1]].Id() != 0 && !layout[playerCords[0] - 1][playerCords[1]].Exits("down"))
    {
      std::cout << "\nDead end\n";
      break;
    } /// daca casuta care trebuie sa il primeasca nu are deschidere catre casuta curenta
    newX = playerCords[0] - 1;
    newY = playerCords[1];
    break;
  case 'd':
    if (playerCords[0] + 1 == width) // daca vrea sa mearga in (down) dar iese din lab
    {
      std::cout << "\nWall\n";
      break;
    }
    if (!layout[playerCords[0]][playerCords[1]].Exits("down"))
    {
      std::cout << "\nDead end\n";
      break;
    }
    if (layout[playerCords[0] + 1][playerCords[1]].Id() != 0 && !layout[playerCords[0] + 1][playerCords[1]].Exits("up"))
    {
      std::cout << "\nDead end\n";
      break;
    } /// daca casuta care trebuie sa il primeasca nu are deschidere catre casuta curenta
    newX = playerCords[0] + 1;
    newY = playerCords[1];
    break;

  case 'l':
    if (playerCords[1] - 1 == -1) // daca vrea sa mearga in stanga dar iese din lab
    {
      std::cout << "\nWall\n";
      break;
    }
    if (!layout[playerCords[0]][playerCords[1]].Exits("left"))
    {
      std::cout << "\nDead end\n";
      break;
    }
    if (layout[playerCords[0]][playerCords[1] - 1].Id() != 0 && !layout[playerCords[0]][playerCords[1] - 1].Exits("right"))
    {
      std::cout << "\nDead end\n";
      break;
    } /// daca casuta care trebuie sa il primeasca nu are deschidere catre casuta curenta
    newX = playerCords[0];
    newY = playerCords[1] - 1;
    break;

  case 'r':
    if (playerCords[1] + 1 == height) // daca vrea sa mearga in dreapta dar iese din lab
    {
      std::cout << "\nWall\n";
      break;
    }
    if (!layout[playerCords[0]][playerCords[1]].Exits("right"))
    {
      std::cout << "\nDead end\n";
      break;
    }
    if (layout[playerCords[0]][playerCords[1] + 1].Id() != 0 && !layout[playerCords[0]][playerCords[1] + 1].Exits("left"))
    {
      std::cout << "\nDead end\n";
      break;
    } /// daca casuta care trebuie sa il primeasca nu are deschidere catre casuta curenta
    newX = playerCords[0];
    newY = playerCords[1] + 1;
    break;

  default:
    break;
  }

  if (newX == -1 && newY == -1)
    return 0;
  layout[playerCords[0]][playerCords[1]].HasPlayer(0);
  layout[playerCords[0]][playerCords[1]].Id(1);
  if(layout[newX][newY].TimesVisited() == 0) {
    GenerateRoom(newX, newY, playerCords[0], playerCords[1]);
  }
  layout[newX][newY].HasPlayer(1);
  layout[newX][newY].TimesVisited(1);
  playerCords[0] = newX;
  playerCords[1] = newY;
  layout[playerCords[0]][playerCords[1]].Id(2);
  if (layout[newX][newY].TimesVisited() > 2)
  {
    ResetLayout(playerCords[0], playerCords[1]);
  }
  if (RNG() % 4 == 0 && layout[playerCords[0]][playerCords[1]].TimesVisited() == 0)
  {
    layout[playerCords[0]][playerCords[1]].GenerateEnemy(1, 50);
  }

  if (moves > 10 && chanceForExit > 10)
  {
    chanceForExit -= 10; // dupa 10 mutari sansele pentru a castiga devin mai mari cu 10% la fiecare noua mutare
  }
  if (finish)
    std::cout << "Congrats! You have escaped!";

  std::cout << "\n"
            << layout[playerCords[0]][playerCords[1]] << "\n"
            << "moves=" << moves << " cFE=" << chanceForExit << "\n";

  return 1;
}

std::ostream &operator<<(std::ostream &cout, const Labyrinth &labyrinth)
{
  int iMAX = labyrinth.width;
  int jMAX = labyrinth.height;

  for (int i = 0; i < iMAX; i++)
  {
    for (int j = 0; j < jMAX; j++)
    {
      cout << labyrinth.layout[i][j].Id() << " ";
    }
    cout << '\n';
  }
  cout << '\n';
  return cout;
}