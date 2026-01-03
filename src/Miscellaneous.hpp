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

class Miscellaneous
{

public:
  static Miscellaneous &Instance();

  // return 1 = continue
  int EffectsOfMoving(Player &player, const std::string &where, Labyrinth &map, bool &shouldExit, int& isFighting);

  void Trade(FightContext &context,  Enemy *enemy, int &RenderOffers, int &isFighting, int &hasGeneratedEnemy, int &MadeATrader);


  void Renders_WithOut_timers(sf::RenderWindow &window, Labyrinth &map, int &isFighting, const Player &player);


private:
  Miscellaneous() = default;
  ~Miscellaneous() = default;
  

};
