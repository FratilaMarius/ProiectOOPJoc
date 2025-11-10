#pragma once

#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <string>
#include <chrono>
#include <thread>

#include "Player.hpp"
#include "Enemy.hpp"
#include "Utility.hpp"
#include "Room.hpp"
#include "TextureLoader.hpp"
#include <SFML/Graphics.hpp>

////////////////////////////////////////////
// clasa Labyrinth e clasa "principala", in ea imi retin harta "labirintului" si, pentru demo, ma ocup si de movement
class Labyrinth
{
public:
  ////////////////////////
  // pl = player coords
  explicit Labyrinth(int _width, int _height, int plX, int plY, txl::TextureLoader *_txtrLd);
  ~Labyrinth();
  Labyrinth &operator=(const Labyrinth &other);
  Labyrinth(const Labyrinth &other);
  friend std::ostream &operator<<(std::ostream &os, const Labyrinth &labyrinth);
  ////////////////////////
  void ResetLayout(int posX, int posY); // reseteaza toate camerele cu exceptia celei in care e playeru
                                        // o camera noua are sigur cale de intoarcere + o alta cale
                                        // cand intram intr o camera noua daca e goala o generam
  void GenerateRoom(int x, int y, int originX, int originY, txl::TextureLoader *_textureLoader);
  ///// Spawn e o functie apelata de constructor. genereaza o camera si plaseaza playerul in ea. camera are minim o iesire
  void Spawn(int x, int y, txl::TextureLoader *_textureLoader);
  int Move(std::string where); // muta playerul in functie de input
  int CheckForItems() { return layout[playerCords[0]][playerCords[1]].FindPickup(RNG() % NR_UNIC_PICKUPS); }

  int HasFinished() const { return finish; }
  
  void RenderCurrentRoom(sf::RenderWindow& window);

private:
  int playerCords[2] = {0, 0};
  int width, height;
  std::vector<std::vector<Room>> layout;
  int moves = 0;
  int chanceForExit = 101; // se va folosi ca rand() % cFE == 0?, astfel cFE = 100 --> 1% sansa pentru exit, cFE = 1 --> 100% sansa
  int finish = 0;
  txl::TextureLoader *_labyrinthRefToTheTxtrLd;
  //////////////////////////
  // in layout imi retin asezarea curenta a lucrurilor pe harta
  //
  //
};

std::ostream &operator<<(std::ostream &cout, const Labyrinth &labyrinth);
