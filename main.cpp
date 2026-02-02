#include <iostream>
#include <fstream>
#include <array>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>

#include "src/Player.hpp"
#include "src/Enemy.hpp"
#include "src/Utility.hpp"
#include "src/Room.hpp"
#include "src/Labyrinth.hpp"
#include "src/TextureLoader.hpp"
#include "src/Exceptions.hpp"
#include "src/UI.hpp"
#include "src/Miscellaneous.hpp"
#include "src/Game.hpp"


int main()
{
  std::srand(std::time(nullptr));

//////////////////////////////////////////////////////////////////////////
// reading the info needded for building the labyrinth:
// spawn coordonates and labyrinth dimmensions
  std::ifstream intrare("dateIntrare.txt");
  if (!intrare.is_open())
  {
    std::cout << "\nEroare la deschiderea fisierului de intrare!";
    return -1;
  }

  int w = 11, h = 11;
  intrare >> w >> h;
  if ((w < 3 || h < 3) || (w > 500 || h > 500))
  {
    std::cout << "\nDimensiuni invalide pentru labirint! (minim 3x3, maxim 500x500)";
    intrare.close();
    return -1;
  }
  // std::cout << "\nDimm lab: " << w << " x " << h;

  int a = 2, b = 5;
  intrare >> a >> b;
  if (a >= w - 1 || b >= h - 1 || a <= 1 || b <= 1)
  {
    std::cout << "Coordonate invalide pentru spawn!\n";
    intrare.close();
    return -1;
  }
  // std::cout << "\nCoord Spawn: " << a << ", " << b;

  sf::RenderWindow window;
  try {
    Game(w, h, a, b, window);
  }
  catch(AppException &exp) {
    std::cout<< exp.what() <<'\n';
    return -1;
  }
  window.close();
  intrare.close();

  return 0;
}