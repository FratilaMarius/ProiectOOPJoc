#pragma once

#include <memory>
#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <chrono>
#include <thread>

#include "Enemy.hpp"
#include <SFML/Graphics.hpp>
#include "Utility.hpp"
#include "TextureLoader.hpp"

//////////////////////////////////////////////////////////////////////
//  Class Room stores info about a specific room from the labyrinth
//
class Room {
public:
  Room() : hasEnemy(0), hasPlayer(0), timesVisited(0), checkedForPickups(0) {}
  Room(const Room &other) = default;
  Room &operator=(const Room &other) = default;
  ~Room() = default;
  friend std::ostream &operator<<(std::ostream &os, const Room &room);
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // printing and setting:
  
  // int GetHasEnemy() const { return hasEnemy; }
  // int GetHasPlayer() const { return hasPlayer; }

  void SetHasPlayer(int yesOrNo) { hasPlayer = yesOrNo; }
  int GetNrRoutes() const { return exits[0] + exits[1] + exits[2] + exits[3]; }
  int GetTimesVisited() const { return timesVisited; }

  int SeeIfExitHere(const std::string &where);    // for setting to 1 via string | up, down, left, right
  int Exits(int where);                           // for setting to 1 via index  | 0-up,1-down,2-left,3-right
  void Exits(const std::string &where, int val);  // for setting to specific value via string
  void Exits(int where, int val);                 // for setting to specific value via index

  void IncrTimesVisited(int x) { timesVisited += x; } // increments the number of visits
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // Gameplay:
  void ResetRoom();                       // resets a room
  // Enemy GenerateEnemy(int _type, int _hp);
  int FindPickup(int what);               // looks for a pickup-able item
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // GFX:
  void SetSprite();                                      // chooses a random txtr f(number of exits)
  void FitSpriteToFrmae(const sf::RenderWindow &window); // centers and scales the sprite
  const sf::Sprite& GetSprite() const {return sprite;}
  ///////////////////////////////////////////////////////////////////////////////////////////////////

private:
  int exits[4] = {1, 1, 1, 1}; // up, down, left, right
  int hasEnemy = 0;            // hasEnemy and hasPlayer are 0 by default
  int hasPlayer = 0;           //
  int timesVisited = 0;        // how many times have we been here?
  int checkedForPickups = 0;   // have we looked for items here?

  sf::Sprite sprite = sf::Sprite(txl::TextureLoader::Instance().GetDefaultTexture());
};

std::ostream &operator<<(std::ostream &os, const Room &room);
