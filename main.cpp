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
  std::cout << "\nDimm lab: " << w << " x " << h;

  int a = 2, b = 5;
  intrare >> a >> b;
  if (a >= w - 1 || b >= h - 1 || a <= 1 || b <= 1)
  {
    std::cout << "Coordonate invalide pentru spawn!\n";
    intrare.close();
    return -1;
  }
  std::cout << "\nCoord Spawn: " << a << ", " << b;
//////////////////////////////////////////////////////////////////////////
// Graphics:
  sf::RenderWindow window;
  /// NOTE: sync with env variable APP_WINDOW from .github/workflows/cmake.yml:31
  window.create(sf::VideoMode({1920/2, 1080/2}), "Expedition", sf::Style::Default);

  /// NOTE: mandatory use one of vsync or FPS limit (not both)            
  /// This is needed so we do not burn the GPU                            
  // window.setVerticalSyncEnabled(true);                                 
  window.setFramerateLimit(60); 
///////////////////////////////////////////////////////////////////////////
// Building the labyrinth and player:
  Labyrinth map(w, h, a, b);
  Player player(100);

///////////////////////////////////////////////////////////////////////////
// Gameplay:
  int displayConsoleStuff = 1;
  int isFighting = 0;
  int hasGeneratedEnemy = 0;

  int timer = 0;
  int RenderTextMissed = 0;
  static std::unique_ptr<Enemy> _enemy = NULL;

  while (window.isOpen()) {
    bool shouldExit = false;
    if(isFighting && !hasGeneratedEnemy) {
      _enemy = EncounterManager::Instance().GenerateAnEnemy(window, player);
      _enemy->PositionSprite();
      hasGeneratedEnemy = 1;
    }

    while (const std::optional event = window.pollEvent()) {
      if (displayConsoleStuff) {
        std::cout << "\nContinue? (keys)  Check for Items? (1)  Check your backpack? (2)  End? (0)";
        std::cout << map;
        displayConsoleStuff = 0;
      }
      if (map.CheckIfFinished()) {
        shouldExit = true;
        break;
      }
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
      else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
        sf::FloatRect visibleArea({0.f, 0.f}, sf::Vector2f(resized->size));
        window.setView(sf::View(visibleArea));
      }
      else if (event->is<sf::Event::MouseButtonPressed>()) {
      //////////////////////////////////////////////////////////////////////////////////
      // handling the input for fighting
        const auto *buttonPressed = event->getIf<sf::Event::MouseButtonPressed>();

        if(buttonPressed->button == sf::Mouse::Button::Left && isFighting && hasGeneratedEnemy) {
          int status = EncounterManager::Instance().Fight(*_enemy.get() , player, RenderTextMissed);
          if(status == 1) { // the player won
            isFighting = 0;
            hasGeneratedEnemy = 0;
          }
          if(status == -1) { // the player died
            isFighting = 0;
            hasGeneratedEnemy = 0;
          }
          // else the fight continues
        }
      }
      else if (event->is<sf::Event::KeyPressed>()) {
        const auto *keyPressed = event->getIf<sf::Event::KeyPressed>();
    
        //////////////////////////////////////////////////////////////////////////////////
        // handling the input for moving
        if (keyPressed->scancode == sf::Keyboard::Scancode::Up && !isFighting) {
          displayConsoleStuff = 1;
          try {
            if(map.Move("up") == 2) isFighting = 1;

            int alive = player.PlayerStatus();
            if (alive < 0) {
              std::cout << "\nYou died\n\n";
              shouldExit = true;
            }
          } catch(LabExceptionCouldntMove &exp) {
              std::cout<< exp.what();
            }
          continue;
        }

        if (keyPressed->scancode == sf::Keyboard::Scancode::Down && !isFighting) {
          displayConsoleStuff = 1;
          try {
            if(map.Move("down") == 2) isFighting = 1;

            int alive = player.PlayerStatus();
            if (alive < 0) {
              std::cout << "\nYou died\n\n";
              shouldExit = true;
            }
          } catch(LabExceptionCouldntMove &exp) {
              std::cout<< exp.what(); 
            }
          continue;
        }

        if (keyPressed->scancode == sf::Keyboard::Scancode::Left && !isFighting) {
          displayConsoleStuff = 1;
          try{
           if(map.Move("left") == 2) isFighting = 1;;
          
            int alive = player.PlayerStatus();
            if (alive < 0) {
              std::cout << "\nYou died\n\n";
              shouldExit = true;
            }
          } catch(LabExceptionCouldntMove &exp) {
              std::cout<< exp.what(); 
            }
          continue;
        }

        if (keyPressed->scancode == sf::Keyboard::Scancode::Right && !isFighting)
        {
          displayConsoleStuff = 1;
          try{
            if(map.Move("right") == 2) isFighting = 1;;

            int alive = player.PlayerStatus();
            if (alive < 0) {
              std::cout << "\nYou died\n\n";
              shouldExit = true;
            }
          } catch (LabExceptionCouldntMove &exp) {
              std::cout<< exp.what(); 
            }
          continue;
        }
//////////////////////////////////////////////////////////////////////////////////
        if (keyPressed->scancode == sf::Keyboard::Scancode::Num0) {
          shouldExit = true;
          break;
        }
        if (keyPressed->scancode == sf::Keyboard::Scancode::Num1) {
          displayConsoleStuff = 1;
          int c = map.CheckForItems();
          if (c == 1)
            player.RefillWater();
          if (c == 2)
            player.RefillFood();
          continue;
        }
        if (keyPressed->scancode == sf::Keyboard::Scancode::Num2) {
          displayConsoleStuff = 1;
          player.BackPack();
          continue;
        }
        std::cout << "\nUnrecognised input\n";
      }
    }
    if (shouldExit) {
      window.close();
      break;
    }

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(10ms);

    window.clear();

    map.SetCurrentRoomSprScale(window);
    window.draw(map.GetCurrentRoomSprite());

    UI::Instance().PositionUI(window);

    window.draw(UI::Instance().GetMiscUIsprite("hpBar"));
    window.draw(UI::Instance().GetMiscUIsprite("hpBar_empty"));
    window.draw(UI::Instance().GetMiscUIsprite("accBar"));
    window.draw(UI::Instance().GetMiscUIsprite("accBar_empty"));

    if(!isFighting)  
      for(int i = 0; i < 4; i++) {
        const int *tempExitArray = map.FigureWhatUItoRender();
        if(tempExitArray[0] == 1) window.draw(UI::Instance().GetUI_Sprites(0));
          else window.draw(UI::Instance().GetEmptyUI_Sprites(0));
        if(tempExitArray[1] == 1) window.draw(UI::Instance().GetUI_Sprites(1));
          else window.draw(UI::Instance().GetEmptyUI_Sprites(1));
        if(tempExitArray[2] == 1) window.draw(UI::Instance().GetUI_Sprites(2));
          else window.draw(UI::Instance().GetEmptyUI_Sprites(2));
        if(tempExitArray[3] == 1) window.draw(UI::Instance().GetUI_Sprites(3));
          else window.draw(UI::Instance().GetEmptyUI_Sprites(3));
      }
    if(isFighting && hasGeneratedEnemy) {
      _enemy->PositionSprite();
      window.draw(_enemy->GetSprite());
    }

    UI::Instance().UpdateTheBullets(player);
    window.draw(UI::Instance().GetText(2));

    if(map.GetShouldDisplayDeadEndText()) window.draw(UI::Instance().GetText(0));
    if(RenderTextMissed) {
      window.draw(UI::Instance().GetText(1));
      timer++;
      if(timer == 120) {
        RenderTextMissed = 0;
        timer = 0;
      }
    }
    
    window.display();
  }


  window.close();
  intrare.close();
  return 0;
}