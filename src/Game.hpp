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
  Game(int W, int h, int a, int b, sf::RenderWindow &window);

private:
};