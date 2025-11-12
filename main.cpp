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

int main()
{

  //////////////////////////////////////////////////////////////////////////
  std::srand(std::time(nullptr));

  std::ifstream intrare("dateIntrare.txt");
  if (!intrare.is_open())
  {
    std::cout << "Eroare la deschiderea fisierului de intrare!\n";
    return -1;
  }

  int w = 11, h = 11;
  intrare >> w >> h;
  if ((w < 3 || h < 3) || (w > 500 || h > 500))
  {
    std::cout << "Dimensiuni invalide pentru labirint! (minim 3x3, maxim 500x500)\n";
    intrare.close();
    return -1;
  }
  std::cout << "\nDimm lab: " << w << " " << h << "\n";

  int a = 2, b = 5;
  intrare >> a >> b;
  std::cout << "\nCoord Spawn: " << a << " " << b << "\n";

  if (a >= w - 1 || b >= h - 1 || a <= 1 || b <= 1)
  {
    std::cout << "Coordonate invalide pentru spawn!\n";
    intrare.close();
    return -1;
  }

  Labyrinth map(w, h, a, b);
  Player jucator(100);
  //////////////////////////////////////////////////////////////////////////

  sf::RenderWindow window;
  ///////////////////////////////////////////////////////////////////////////
  /// NOTE: sync with env variable APP_WINDOW from .github/workflows/cmake.yml:31
  window.create(sf::VideoMode({800, 700}), "Expedition", sf::Style::Default);
  ///////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////3)////////////
  /// NOTE: mandatory use one of vsync or FPS limit (not both)            ///
  /// This is needed so we do not burn the GPU                            ///
  // window.setVerticalSyncEnabled(true);                                 ///
  window.setFramerateLimit(20); ///
  ///////////////////////////////////////////////////////////////////////////
  int displayConsoleStuff = 1;
  while (window.isOpen())
  {

    bool shouldExit = false;

    while (const std::optional event = window.pollEvent())
    {
      if (displayConsoleStuff)
      {
        std::cout << "Continue? (keys)  Check for Items? (1)  Check your backpack? (2)  End? (0)   \n";
        std::cout << map;
        displayConsoleStuff = 0;
      }
      if (map.HasFinished())
      {
        shouldExit = true;
        break;
      }
      if (event->is<sf::Event::Closed>())
      {
        window.close();
        std::cout << "Fereastra a fost închisă\n";
      }
      else if (event->is<sf::Event::Resized>())
      {
      }
      else if (event->is<sf::Event::KeyPressed>())
      {
        const auto *keyPressed = event->getIf<sf::Event::KeyPressed>();

        if (keyPressed->scancode == sf::Keyboard::Scancode::Up)
        {
          displayConsoleStuff = 1;
          if (map.Move("up"))
          {
            int alive = jucator.PlayerStatus();
            if (alive < 0)
            {
              std::cout << "\nYou died\n\n";
              shouldExit = true;
            }
          }
          continue;
        }
        if (keyPressed->scancode == sf::Keyboard::Scancode::Down)
        {
          displayConsoleStuff = 1;
          if (map.Move("down"))
          {
            int alive = jucator.PlayerStatus();
            if (alive < 0)
            {
              std::cout << "\nYou died\n\n";
              shouldExit = true;
            }
          }
          continue;
        }
        if (keyPressed->scancode == sf::Keyboard::Scancode::Left)
        {
          displayConsoleStuff = 1;
          if (map.Move("left"))
          {
            int alive = jucator.PlayerStatus();
            if (alive < 0)
            {
              std::cout << "\nYou died\n\n";
              shouldExit = true;
            }
          }
          continue;
        }
        if (keyPressed->scancode == sf::Keyboard::Scancode::Right)
        {
          displayConsoleStuff = 1;
          if (map.Move("right"))
          {
            int alive = jucator.PlayerStatus();
            if (alive < 0)
            {
              std::cout << "\nYou died\n\n";
              shouldExit = true;
            }
          }
          continue;
        }
        if (keyPressed->scancode == sf::Keyboard::Scancode::Num0)
        {
          shouldExit = true;
          break;
        }
        if (keyPressed->scancode == sf::Keyboard::Scancode::Num1)
        {
          displayConsoleStuff = 1;
          int c = map.CheckForItems();
          if (c == 1)
            jucator.RefillWater();
          if (c == 2)
            jucator.RefillFood();
          continue;
        }
        if (keyPressed->scancode == sf::Keyboard::Scancode::Num2)
        {
          displayConsoleStuff = 1;
          jucator.BackPack();
          continue;
        }
        std::cout << "\nUnrecognised input\n";
      }
    }
    if (shouldExit)
    {
      window.close();
      std::cout << "Fereastra a fost închisă (shouldExit == true)\n";
      break;
    }
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(300ms);


    window.clear();

    map.RenderCurrentRoom(&window);
    window.display();
  }



  window.close();
  intrare.close();
  return 0;
}