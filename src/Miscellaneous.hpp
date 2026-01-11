#pragma once

#include <iostream>
#include <fstream>
#include <array>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>

#include "Player.hpp"
#include "Enemy.hpp"
#include "Utility.hpp"
#include "Room.hpp"
#include "Labyrinth.hpp"
#include "TextureLoader.hpp"
#include "Exceptions.hpp"
#include "UI.hpp"
#include "Events.hpp"

class Miscellaneous
{

public:
  static Miscellaneous &Instance();

  // return 1 = continue
  int EffectsOfMoving(Player &player, const std::string &where, Labyrinth &map, bool &shouldExit, int& isFighting, int& GeneratedEvent);

  void Trade(FightContext &context,  Enemy *enemy, int &RenderOffers, int &isFighting, int &hasGeneratedEnemy, int &MadeATrader);


  void Renders_WithOut_timers(sf::RenderWindow &window, Labyrinth &map, const int &isFighting, const Player &player, const int &GeneratedEvent);


private:
  Miscellaneous() = default;
  ~Miscellaneous() = default;
  

};
