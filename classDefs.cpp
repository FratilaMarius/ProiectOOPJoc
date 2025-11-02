#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <chrono>
#include <thread>

#include "data/Player.hpp"
#include "data/Enemy.hpp"
#include "data/Utility.hpp"
#include "data/Room.hpp"
namespace data{
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
        if(layout[playerCords[0]] [playerCords[1]].HasEnemy()) {
          std::cout<< "Enemy encountered!";
        }

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


          if(RNG() % 4 == 0 && layout[playerCords[0]] [playerCords[1]].TimesVisited() == 0) {
            layout[playerCords[0]] [playerCords[1]].GenerateEnemy(1, 50);
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