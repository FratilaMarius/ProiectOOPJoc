#include <iostream>
#include <array>
#include <chrono>
#include <thread>

namespace data{

  #define NR_UNIC_CAMERE 10
  #define NR_UNIC_PICKUPS 6
  #define TEXTURE_NAME_SIZE 15
//////////////////////////////////////////
    int RNG () {
      srand(time(nullptr));
      return rand();
    }

    // FUNCTIE NECESAARA DOAR PENTRU DEMO, REDO:
    int Input() { // -1 back(up) | 1 fwrd(down) | 2 left | 3 right
      int a; 
      std::cout<<"\nDiretion: ";
      std::cin>>a;
      return a;
    }
//////////////////////////////////////////



//////////////////////////////////////////
// clasa player retine toate informatiile despre player, un fel de inventar.
// nu este un lucru "fizic", nu contine date despre pozitia playerului spre exemplu
  class Player{
    public:

////////////////////////
      explicit Player(int _hp) {
        this->hp = _hp;
      }
      ~Player() = default;
      Player(const Player& other) {
        this->hp = other.hp;
        this->accuracy = other.accuracy;
        this->bullets = other.bullets;
        this->lights = other.lights;
        this->tent = other.tent;
        this->meds = other.meds;
        this->water = other.water;
        this->food = other.food;
      }
      Player& operator=(const Player other) {
        if (this == &other) return *this;
        this->hp = other.hp;
        this->accuracy = other.accuracy;
        this->bullets = other.bullets;
        this->lights = other.lights;
        this->tent = other.tent;
        this->meds = other.meds;
        this->water = other.water;
        this->food = other.food;
        return *this;
      }      
      friend std::ostream& operator<<(std::ostream& os, const Player& player);
////////////////////////

    int Hp() const { 
      return hp;
    }

    private: 
      int hp;
      int accuracy = 1;
      int bullets = 6;
      int lights = 3;
      int tent = 0;
      int meds = 25;
      int water = 100;
      int food = 100;

  };
    std::ostream& operator<<(std::ostream& os,  const Player& player) {
      os << "Player("
      << "hp=" << player.hp
      << ", accuracy=" << player.accuracy
      << ", bullets=" << player.bullets
      << ", water=" << player.water
      << ")";
      return os;
    }
//////////////////////////////////////////



//////////////////////////////////////////
// Clasa Room retine informatii despre o anumita camera din labirint
// 
  class Room {
    public:

////////////////////////
      Room() : id(0), texture(NULL), exits{1,0,0,0}, hasEnemy(0), hasPlayer(0) {}
      explicit Room(int _id) { // constructor daca am nevoie de id predefinit
        this->id = _id;
        texture = NULL;
        hasEnemy= 0;
        hasPlayer = 0;
      }
      Room(const Room& other) {
        this->id = other.id;
        this->hasEnemy = other.hasEnemy;
        this->hasPlayer = other.hasPlayer;
        std::copy(other.texture, other.texture + TEXTURE_NAME_SIZE, this->texture);
        std::copy(other.exits, other.exits + 4, this->exits);
      }
      Room& operator=(const Room other) {
        if (this == &other) return *this;
        this->id = other.id;
        this->hasEnemy = other.hasEnemy;
        this->hasPlayer = other.hasPlayer;
        std::copy(other.texture, other.texture + TEXTURE_NAME_SIZE, this->texture);
        std::copy(other.exits, other.exits + 4, this->exits);
        return *this;
      }      
      ~Room() {
        delete texture;
      }
      friend std::ostream& operator<<(std::ostream& os, const Room& room);
////////////////////////

      int Id() const{
        return id;
      }
      void Id(int _id) {
        id =_id;
      }
      int HasEnemy() const{
        return hasEnemy;
      }      
      int HasPlayer() const{
        return hasPlayer;
      }
      void HasPlayer(int yesOrNo) {
        hasPlayer = yesOrNo;
      }
    private:
      int id;
      char *texture;
      int exits[4] = {1,0,0,0}; // back, left, right, forward
      int hasEnemy;      // hasEnemy si hasPlayer sunt 0 default, 1 la nevoie
      int hasPlayer;     // 
  };
    std::ostream& operator<<(std::ostream& os,  const Room& room) {
      os << "Player("
      << "HP=" << room.id
      << ", accuracy=" << room.texture
      << ", bullets=" << room.hasPlayer
      << ", water=" << room.hasEnemy
      << ")";
      return os;
    }








  ////////////////////////////////////////////
  // clasa Labyrinth e clasa "principala" rn, in ea imi retin harta "labirintului" si, pentru demo, ma ocup si de movement
  class Labyrinth{
    public:
////////////////////////
      explicit Labyrinth(int _width, int _height, int plX, int plY) {
        this->width = _width;
        this->height = _height;
        layout = new Room*[width];

        for (int i = 0; i < width; ++i) {
          layout[i] = new Room[height];
        }
        for (int i = 0; i < width; ++i)
          for (int j = 0; j < height; ++j)
            layout[i][j].Id(0);
        
        Spawn(plX, plY);
      }
      ~Labyrinth() {
          for (int i = 0; i < width; ++i) 
            delete[] layout[i];
          delete[] layout;
      }
      Labyrinth& operator=(const Labyrinth other) {
        if (this == &other) return *this;

        for (int i = 0; i < width; ++i) 
          delete[] layout[i];
        delete[] layout;

        this->width = other.width;
        this->height = other.height;

        layout = new Room*[width];
        for (int i = 0; i < width; ++i) {
            layout[i] = new Room[height];
        }
        if (layout) {
            std::copy(other.layout, other.layout + (width * height), layout);
        }
        return *this;
      }
      Labyrinth(const Labyrinth &other) {

        for (int i = 0; i < width; ++i) 
          delete[] layout[i];
        delete[] layout;

        this->width = other.width;
        this->height = other.height;

        layout = new Room*[width];
        for (int i = 0; i < width; ++i) {
            layout[i] = new Room[height];
        }
        if (layout) {
            std::copy(other.layout, other.layout + (width * height), layout);
        }
      }
      friend std::ostream& operator<<(std::ostream& os, const Labyrinth &labyrinth);
////////////////////////

      void GenerateRoom(int x, int y) {         // cand intram intr o camera noua daca e goala o generam
        if(layout[x][y].Id() == 0) {
          layout[x][y].Id(RNG() % NR_UNIC_CAMERE);
          if(layout[x][y].Id() == 0) layout[x][y].Id( layout[x][y].Id() + 1);
        }
      }

      void Spawn(int x, int y) {               // functia de mai sus dar apelata la inceput
        layout[x][y].Id(RNG() % NR_UNIC_CAMERE);
        if(layout[x][y].Id() == 0) layout[x][y].Id( layout[x][y].Id() + 1);
        layout[x][y].HasPlayer(1);
        playerCords[0] = x;
        playerCords[1] = y;
      }
      
      void Move() {
        int where = Input();
        int newX = -1, newY = -1;
        switch (where)
        {
        case 1:
          if (playerCords[0] + 1 == width) // daca vrea sa mearga in fata(down) dar iese din lab
          {
            std::cout<<"\nWall\n";
            break;
          }
          newX = playerCords[0] + 1;
          newY = playerCords[1];
        break;
      
        case -1:
          if (playerCords[0] - 1 == -1) // daca vrea sa mearga in spate(up) dar iese din lab
          {
            std::cout<<"\nWall\n";
            break;
          }
          newX = playerCords[0] - 1;
          newY = playerCords[1];
        break;
        
        case 2:
          if (playerCords[1] - 1 == -1) // daca vrea sa mearga in stanga dar iese din lab
          {
            std::cout<<"\nWall\n";
            break;
          }
          newX = playerCords[0];
          newY = playerCords[1] - 1;
        break;
        
        case 3:
          if (playerCords[1] + 1 == height) // daca vrea sa mearga in dreapta dar iese din lab
          {
            std::cout<<"\nWall\n";
            break;
          }
          newX = playerCords[0];
          newY = playerCords[1] + 1;
        break;
        
        default:
          break;
        }
        if(newX == -1 && newY == -1) return;
        layout[playerCords[0]][playerCords[1]].HasPlayer(0);
        layout[newX][newY].HasPlayer(1);
        playerCords[0] = newX;
        playerCords[1] = newY;
        GenerateRoom(newX, newY);
      }

////////////////// functie de debug/demo
      // void PrintMap() {
      //   for(int i = 0; i < width; i++) {
      //     for(int j = 0; j < height; j++)
      //       std::cout << this->layout[i][j].Id()<< " ";
      //     std::cout<<'\n';
      //   }
      //   std::cout<<'\n';
      // }

    private:
      int playerCords[2];
      int width, height;
      Room **layout;
      //////////////////////////
      // in layout imi retin asezarea curenta a lucrurilor pe harta
      // 0 = perete, 1-NR_UNIC_CAMERE sunt camere de toate diferitele tipuri
      //
      //


  };
    std::ostream& operator<<(std::ostream& cout,  const Labyrinth &labyrinth) {
      for(int i = 0; i < labyrinth.width; i++) {
        for(int j = 0; j < labyrinth.height; j++)
          cout << labyrinth.layout[i][j].Id()<< " ";
        cout<<'\n';
        }
      cout<<'\n';
      return cout;
    }

};
