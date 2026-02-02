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
#include "Miscellaneous.hpp"
#include "Events.hpp"
#include "Crit.hpp"

class Game
{
public:
  Game(int _w, int _h, int _a, int _b, sf::RenderWindow &_window);

private:
  sf::RenderWindow &window;
  int w, h, a, b;
  int isFighting = 0;
  int hasGeneratedEnemy = 0, madeATrader = 0, renderRequest = 0, renderConclusion = 0;

  int timerPlayer = 0, timerEnemy = 0, timerPickupText = 0, timerCheat = 0, timerShiftingRooms = 0;
  int RenderTextMissed = 0, EnemyRenderTextMissed = 0, RenderPickupText = 0, RenderOffers = 0, RenderCheat = 0;

  int shots = 6;         // this is used when we fight a mminotaur
  int selectedOffer = 0; // this is used for the trader interactions

  void ResetVar(Labyrinth& map, Player& player, std::unique_ptr<Enemy>& enemyPtr, std::unique_ptr<Event>& eventPtr);
};