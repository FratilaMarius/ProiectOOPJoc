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
#include <SFML/Graphics.hpp>
#include "TextureLoader.hpp"
#include "Exceptions.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Labyrinth class is the main one, handling the movement and map
//
class Labyrinth {

public:
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Construcotrs/ destrucotrs:
//
  explicit Labyrinth(int _width, int _height, int plX, int plY);
  ~Labyrinth();

  Labyrinth &operator=(const Labyrinth &other);
  Labyrinth(const Labyrinth &other);

  friend std::ostream &operator<<(std::ostream &os, const Labyrinth &labyrinth);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Gameplay:
//
  void ResetLayout(int posX, int posY); // resets the entire map, minus the room[posX][posY]

  void GenerateRoom(int x, int y, int originX, int originY); // generates a new room (gives it a texture and exits)
  void Spawn(int x, int y); // the equivalent of GenerateRoom but only called on start

  int Move(const std::string &where); // moves the player from one room to another, f(string) : up/down/left/right
  void GetCloserToExit(int n) { moves += n/10; chanceForExit -= n; }

  int CheckForItems() { return layout[playerCords[0]][playerCords[1]].FindPickup(RNG() % NR_UNIQUE_PICKUPS); }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GFX:
//
  const sf::Sprite& GetCurrentRoomSprite() const {  return layout[playerCords[0]][playerCords[1]].GetSprite();}
  void SetCurrentRoomSprScale(const sf::RenderWindow& window) {layout[playerCords[0]][playerCords[1]].FitSpriteToFrmae(window);}

  const int* FigureWhatUItoRender() const { return layout[playerCords[0]][playerCords[1]].GetExitsArray(); }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setters/Getters:
//
  int CheckIfFinished() const { return finish; }
  int GetShouldDisplayDeadEndText() const { return shouldDisplayDeadEndText; }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
  int playerCords[2] = {0, 0}; // used to store the current player coords
  int shouldDisplayDeadEndText = 0;

  int width, height;
  std::vector<std::vector<Room>> layout;
  
  int moves = 0; // how many moves we have done
  int chanceForExit = 100; // is used as rand() % cFE == 0?, so cFE = 100 --> 1% chance to exit, cFE = 1 --> 100% chance
                           // it is decremented by 10 each time we make a move to a new room
  int finish = 0; // if this is 1 we end (win)
};

std::ostream &operator<<(std::ostream &cout, const Labyrinth &labyrinth);
