#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <chrono>
#include <thread>

namespace data{

  #define NR_UNIC_PICKUPS 5
  #define TEXTURE_NAME_SIZE 15
//////////////////////////////////////////
    int RNG () {
      return rand();
    }

    // FUNCTIE NECESAARA DOAR PENTRU DEMO, REDO:
    std::string _Input() { // 0 (up) | 1 (down) | 2 left | 3 right
      std::string a; 
      std::cout<<"\nDirection: ";
      std::cin>>a;
      return a;
    }
//////////////////////////////////////////

  class Enemy{
    public:
      Enemy(int _type, int _hp) : type(_type), hp(_hp) {
        if(type == 0) std::cout<<"\nInvalid enemy type generated\n";
        if(type == 1)

;      }

    private: 
      int type;  //  id, deocamdata 2 tipuri
      int hp;
      int damage;
      std::string texture = "";

  };




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

      void RefillWater() {
        water = 15;
        if(food > 0 && water > 0) movesUntilDemise = 5;
      }      
      void RefillFood() {
        food = 12;
        if(food > 0 && water > 0) movesUntilDemise = 5;
      }
      
      int PlayerStatus() {
        water--;
        food--;
        if(water < 0) movesUntilDemise--;
        if(food < 0) movesUntilDemise--;

        if(food < 0) std::cout<<OutOfFood;
        if(water < 0) std::cout<<OutOfWater;
        if(food<0||water<0) std::cout<<outOfSupplies<<movesUntilDemise<<"\n";

        if(movesUntilDemise < 0) return -1;
        return 1;
      }


      friend std::ostream& operator<<(std::ostream& os, const Player& player);
////////////////////////

    void BackPack() const {
      std::cout<<"\n"<<"Food: "<<food<<", Water: "<<water<<"\n";
    }

    int Hp() const { 
      return hp;
    }

    private: 
      int hp;
      int accuracy = 1;
      int bullets = 6;
      int lights = 8;
      int tent = 0;
      int meds = 25;
      int water = 15;
      int food = 12;

      int movesUntilDemise = 5;

      std::string OutOfFood = "\nYou have run out of food!\n";
      std::string OutOfWater = "\nYou have run out of water!\n";
      std::string outOfSupplies = "\nYou are out of supplies! Days until the elements overtake you: ";
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
      Room() : id(0), texture(""), hasEnemy(0), hasPlayer(0) {}
      explicit Room(int _id) { // constructor daca am nevoie de id predefinit
        this->id = _id;
        texture = "";
        hasEnemy= 0;
        hasPlayer = 0;
      }
      Room(const Room& other) {
        this->id = other.id;
        this->hasEnemy = other.hasEnemy;
        this->hasPlayer = other.hasPlayer;
        this->texture = other.texture;
        std::copy(other.exits, other.exits + 4, this->exits);
      }
      Room& operator=(const Room other) {
        if (this == &other) return *this;
        this->id = other.id;
        this->hasEnemy = other.hasEnemy;
        this->hasPlayer = other.hasPlayer;
        std::copy(other.exits, other.exits + 4, this->exits);
        std::copy(other.exits, other.exits + 4, this->exits);
        return *this;
      }      
      ~Room() = default;
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
      int Exits(std::string where) {
        if(where == "up") return exits[0];
        if(where == "down") return exits[1];
        if(where == "left") return exits[2];
        if(where == "right") return exits[3];
        return -1;
      }
      int Exits(int where) {
        if(where == 0) return exits[0];
        if(where == 1) return exits[1];
        if(where == 2) return exits[2];
        if(where == 3) return exits[3];
        return -1;
      }
      void Exits(std::string where, int val) {
        if(where == "up") exits[0] = val;
        if(where == "down") exits[1] = val;
        if(where == "left") exits[2] = val;
        if(where == "right") exits[3] = val;
      }
      void Exits(int where, int val) {
        if(where == 0) exits[0] = val;
        if(where == 1) exits[1] = val;
        if(where == 2) exits[2] = val;
        if(where == 3) exits[3] = val;
      }
      int NrRoutes() {
        return exits[0] + exits[1] + exits[2] + exits[3];
      }
      int TimesVisited() {
        return timesVisited;
      }
      void TimesVisited(int x) {
        timesVisited += x;
      }


      void ResetRoom() {
        id = 0;
        texture = "";
        exits[0] = 1;
        exits[1] = 1;
        exits[2] = 1;
        exits[3] = 1;        
        hasEnemy = 0;
        hasPlayer = 0;
        timesVisited = 0;
      }
    //   void ReadProp(int _id) {
    //     std::ifstream propCamere("Rooms.txt"); // deschide fișierul pentru citire
    //     if(!propCamere) {
    //       std::cout<<"Nu s a gasit fisierul\n";
    //       propCamere.close();
    //       return;
    //     }
    //     std::string line = "";
    //     std::string nr;
    //     int nr_int = -1;
    //     int spatiu;
    //     while(nr_int != _id) {
    //       if(propCamere.eof()) break;
    //       getline(propCamere, line);  
    //       spatiu = line.find(" ");
    //       nr = line.substr(0, spatiu);
    //       nr_int = std::stoi(nr);
    //       // nr int e acum numarul liniei
    //       if(nr_int == id) break;
    //     }
    //     if(nr_int == id) {
    //       std::string inFIleExits = line.substr(spatiu + 1, spatiu + 7); // citim exiturile din fisier
    //       texture = line.substr(spatiu + 9);
    //       exits[0] = inFIleExits[0]-48; 
    //       exits[1] = inFIleExits[2]-48;
    //       exits[2] = inFIleExits[4]-48;
    //       exits[3] = inFIleExits[6]-48;
    //     }
    //     propCamere.close();
    // }
    
    int FindPickup(int what) {
      if(checkedForPickups) {
        std::cout << "\nYou've already checked here\n";
        return 0;
      }
      checkedForPickups = 1;
      switch(what) {
        case 0:
        case 1:
        case 2: 
          std::cout<<"\nFound nothing\n";
          return 0;
        break;

        case 3:
          std::cout<<"\nFound some water\n";
          return 1;
        break;

        case 4:
          std::cout<<"\nFound some food\n";
          return 2;
        break;

        default: 
        break;
      }
      return 0;
    }

    private:
      int id;
      std::string texture = "";
      int exits[4] = {1,1,1,1}; // up, down, left, right
      int hasEnemy;      // hasEnemy si hasPlayer sunt 0 default, 1 la nevoie
      int hasPlayer;     // 
      int timesVisited = 0;

      int checkedForPickups = 0;
  };
    std::ostream& operator<<(std::ostream& os,  const Room& room) {
      os << "Room("
      << "ID=" << room.id
      << " exits=" << room.exits[0] << "(up) " << room.exits[1] << "(down) " << room.exits[2] << "(left) " << room.exits[3]
      << "(rigth), texture=" << room.texture
      << ", hasPlayer=" << room.hasPlayer
      << ", hasEnemy=" << room.hasEnemy
      << ")";
      return os;
    }








  ////////////////////////////////////////////
  // clasa Labyrinth e clasa "principala", in ea imi retin harta "labirintului" si, pentru demo, ma ocup si de movement
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
      int HasFinished() {
        return finish;
      }
      void ResetLayout(int posX, int posY) {
        for(int i = 0; i < width; i++) 
          for(int j = 0; j < height; j++) {
            if(i == posX && j == posY) continue;
            layout[i][j].ResetRoom();
          }
      }

      void GenerateRoom(int x, int y, int originX, int originY) {         // cand intram intr o camera noua daca e goala o generam
        if(layout[x][y].Id() == 0) {
          moves++;
          layout[x][y].Exits(0, 0);
          layout[x][y].Exits(1, 0);
          layout[x][y].Exits(2, 0);
          layout[x][y].Exits(3, 0);
          layout[x][y].Id(1);

          int retur;
          if(originX - x < 0) {
            layout[x][y].Exits("up", 1);
            retur = 0;
          } // daca a venit de sus setam iesire in sus
          if(x - originX < 0) {
            layout[x][y].Exits("down", 1);
            retur = 1;
          } // daca a venit de jos setam iesire in jos
          if(y - originY < 0) {
            layout[x][y].Exits("right", 1);
            retur = 3;
          } // daca a venit din dreapta setam iesire in jos
          if(originY - y < 0) {
            layout[x][y].Exits("left", 1);
            retur = 2;
          } // daca a venit din stanga setam iesire in jos

          int chance = RNG() % 100;
          if( chance < 100) { // facem doar cu 2 iesiri
            int z = RNG() % 4;
            if(z == retur) {
              z += 2;
              z %= 4;
            }
            std::cout<<"\n"<<z<<"\n";
            layout[x][y].Exits(z, 1);
          }
          if( (chance) <= 60) { // facem doar cu 3 iesiri
            int z = RNG() % 4;
            if(z == retur) {
              z += 2;
              z %= 4;
            }
            layout[x][y].Exits(z, 1);

            if(RNG() % 2) z += 1;
            else z += 3;
            z %= 4;
            layout[x][y].Exits(z, 1);
          }
          if( (chance) < 25) { // facem cu 4 iesiri
            layout[x][y].Exits(0, 1);
            layout[x][y].Exits(2, 1);
            layout[x][y].Exits(1, 1);
            layout[x][y].Exits(3, 1);
          }
        if(RNG() % chanceForExit == 0) {
          finish = 1;
        }
        }  // o camera noua are sigur cale de intoarcere + o alta cale 
      }

      void Spawn(int x, int y) {               // functia de mai sus dar apelata la inceput
          layout[x][y].Id(1);
            
            layout[x][y].Exits("up", RNG() % 2);
            layout[x][y].Exits("down", RNG() % 2);
            layout[x][y].Exits("left", RNG() % 2);
            layout[x][y].Exits("right", RNG() % 2);
          if(layout[x][y].NrRoutes() < 1) {
            int z = RNG() % 4;
            layout[x][y].Exits(z, 1);
          } 

        layout[x][y].HasPlayer(1);
        layout[x][y].TimesVisited(1);
        playerCords[0] = x;
        playerCords[1] = y;
      }    ///// Spawn e o functie apelata de constructor. genereaza o camera si plaseaza playerul in ea. camera are minim o iesire

      int Move() {
        std::cout<<"\n"<<layout[playerCords[0]] [playerCords[1]]<<"\n"<<"moves="<<moves<<" cFE="<<chanceForExit<<"\n";
        std::string where = _Input();
        int newX = -1, newY = -1;
        switch (where[0])
        {
        case 'u':
          if (playerCords[0] - 1 == -1) // daca vrea sa mearga in (up) dar iese din lab
          {
            std::cout<<"\nWall\n";
            break;
          }

            if(!layout[playerCords[0]] [playerCords[1]].Exits("up")) 
            {
              std::cout<<"\nDead end\n";
              break;
            } /// daca casuta curenta nu il lasa sa mearga in sus
            if(layout[playerCords[0]-1] [playerCords[1]].Id() != 0 && !layout[playerCords[0]-1] [playerCords[1]].Exits("down")) {  
              std::cout<<"\nDead end\n";
              break;
            } /// daca casuta care trebuie sa il primeasca nu are deschidere catre casuta curenta


          newX = playerCords[0] - 1;
          newY = playerCords[1];
        break;


        case 'd':
          if (playerCords[0] + 1 == width) // daca vrea sa mearga in (down) dar iese din lab
          {
            std::cout<<"\nWall\n";
            break;
          }

            if(!layout[playerCords[0]] [playerCords[1]].Exits("down")) 
            {
              std::cout<<"\nDead end\n";
              break;
            }
            if(layout[playerCords[0]+1] [playerCords[1]].Id() != 0 && !layout[playerCords[0]+1] [playerCords[1]].Exits("up")) {  
              std::cout<<"\nDead end\n";
              break;
            } /// daca casuta care trebuie sa il primeasca nu are deschidere catre casuta curenta

          newX = playerCords[0] + 1;
          newY = playerCords[1];
        break;
        
        case 'l':
          if (playerCords[1] - 1 == -1) // daca vrea sa mearga in stanga dar iese din lab
          {
            std::cout<<"\nWall\n";
            break;
          }

            if(!layout[playerCords[0]] [playerCords[1]].Exits("left")) 
            {
              std::cout<<"\nDead end\n";
              break;
            }
            if(layout[playerCords[0]] [playerCords[1]-1].Id() != 0 && !layout[playerCords[0]] [playerCords[1]-1].Exits("right")) {  
              std::cout<<"\nDead end\n";
              break;
            } /// daca casuta care trebuie sa il primeasca nu are deschidere catre casuta curenta

          newX = playerCords[0];
          newY = playerCords[1] - 1;
        break;
        
        case 'r':
          if (playerCords[1] + 1 == height) // daca vrea sa mearga in dreapta dar iese din lab
          {
            std::cout<<"\nWall\n";
            break;
          }

            if(!layout[playerCords[0]] [playerCords[1]].Exits("right")) 
            {
              std::cout<<"\nDead end\n";
              break;
            }
            if(layout[playerCords[0]] [playerCords[1]+1].Id() != 0 && !layout[playerCords[0]] [playerCords[1]+1].Exits("left")) {  
              std::cout<<"\nDead end\n";
              break;
            } /// daca casuta care trebuie sa il primeasca nu are deschidere catre casuta curenta

          newX = playerCords[0];
          newY = playerCords[1] + 1;
        break;
        
        default:
          break;
        }
        
        if(newX == -1 && newY == -1) return 0;
          layout[playerCords[0]][playerCords[1]].HasPlayer(0);
          layout[playerCords[0]][playerCords[1]].Id(1);

          GenerateRoom(newX, newY, playerCords[0], playerCords[1]);
          layout[newX][newY].HasPlayer(1);
          layout[newX][newY].TimesVisited(1);

          playerCords[0] = newX;
          playerCords[1] = newY;
          layout[playerCords[0]][playerCords[1]].Id(2);


          if(layout[newX][newY].TimesVisited() > 2) {
            ResetLayout(playerCords[0], playerCords[1]);
          }

          if(moves > 10 && chanceForExit > 10) {
            chanceForExit -= 10; //dupa 10 mutari sansele pentru a castiga devin mai mari cu 10% la fiecare noua mutare
          }
          
          if(finish) std::cout<<"Congrats! You have escaped!";
          return 1;
      }

      int CheckForItems() {
        return layout[playerCords[0]] [playerCords[1]].FindPickup(RNG() % NR_UNIC_PICKUPS);
      }

    private:
      int playerCords[2];
      int width, height;
      Room **layout;
      int moves = 0;
      int chanceForExit = 101; // se va folosi ca rand() % cFE == 0?, astfel cFE = 100 --> 1% sansa pentru exit, cFE = 1 --> 100% sansa
      int finish = 0;
      //////////////////////////
      // in layout imi retin asezarea curenta a lucrurilor pe harta
      //
      //


  };
    std::ostream& operator<<(std::ostream& cout, const Labyrinth &labyrinth) {
      for(int i = 0; i < labyrinth.width; i++) {
        for(int j = 0; j < labyrinth.height; j++)
          cout << labyrinth.layout[i][j].Id()<< " ";
        cout<<'\n';
        }
      cout<<'\n';
      return cout;
    }

};

//TODO:
// sistem de citire de caracterisitici de camere si texturi din fisier                            X?
// sistem de spawnare de inamici
// sistem de lumina
// sistem de in functie de tip de camera nu are voie sa se duca decat in anumit loc               X
// sistem de combat cu monstri 
// de copiat fisierul cu date despre camere din ./data/Rooms in install dir         
// failsafe pentru labirint in cerc, dca trece de 2/3 ori prin aceeasi camera se reseteaza        X
// mecanica de iesire                                                                             X
// mecanica de portal
// mecanica de pickupuri in camere
// general balance: player dies too quick, rooms need to generate with more exits on average
// redo the README !!!!!!!                                                                        X