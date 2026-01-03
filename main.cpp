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
  // window.setKeyRepeatEnabled(false);
///////////////////////////////////////////////////////////////////////////
// Building the labyrinth and player:
  Labyrinth map(w, h, a, b);
  Player player(100);

///////////////////////////////////////////////////////////////////////////
// Gameplay:

  int isFighting = 0;
  int hasGeneratedEnemy = 0, madeATrader = 0;

  int timerPlayer = 0, timerEnemy = 0, timerPickupText = 0, timerCheat = 0, timerShiftingRooms = 0;
  int RenderTextMissed = 0, EnemyRenderTextMissed = 0, RenderPickupText = 0, RenderOffers = 0, RenderCheat = 0;
  static std::unique_ptr<Enemy> _enemy = NULL;
  int shots = 6; // this is used when we fight a mminotaur
  int selectedOffer = 0; // this is used for the trader interactions
  FightContext context(player, map, RenderTextMissed, EnemyRenderTextMissed, shots, selectedOffer);



  while (window.isOpen()) {
    bool shouldExit = false;
    if(isFighting && !hasGeneratedEnemy) {
      _enemy = EncounterManager::Instance().GenerateAnEnemy(window, player, madeATrader);
      if(_enemy == NULL) isFighting = 0;
      else {
        hasGeneratedEnemy = 1;
        _enemy->PositionSprite();
        _enemy->PlayAudio(1);

        if(madeATrader) { // since the interaction with the trader differs, we cehck for it
          madeATrader = 1;
          _enemy->attack(player);
          RenderOffers = 1;
        }
      }
    }

    while (const std::optional event = window.pollEvent()) {
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
        if(isFighting && hasGeneratedEnemy) {
          if(buttonPressed->button == sf::Mouse::Button::Left) {
            selectedOffer = 3;
            int status = EncounterManager::Instance().Fight(context, _enemy.get());
            if(status == 1) { // the player won
              shots = 6;
              selectedOffer = 0;
              isFighting = 0;
              hasGeneratedEnemy = 0;
            }
            if(status == -1) { // the player lost
              shots = 6;
              selectedOffer = 0;
              isFighting = 0;
              hasGeneratedEnemy = 0;
            }
            // else the fight continues
          }
        }
      }
      else if (event->is<sf::Event::KeyPressed>()) {
        const auto *keyPressed = event->getIf<sf::Event::KeyPressed>();

        //////////////////////////////////////////////////////////////////////////////////
        // Input for handling the trader interaction:
        if(isFighting && hasGeneratedEnemy && madeATrader) {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Q) {
              Miscellaneous::Instance().Trade(context, _enemy.get(), RenderOffers, isFighting, hasGeneratedEnemy, madeATrader);
              continue;
            }
          if (keyPressed->scancode == sf::Keyboard::Scancode::E) {
              Miscellaneous::Instance().Trade(context, _enemy.get(), RenderOffers, isFighting, hasGeneratedEnemy, madeATrader);
              continue;
            }
        }

        //////////////////////////////////////////////////////////////////////////////////
        // handling the input for moving
        if(!isFighting) {

          if (keyPressed->scancode == sf::Keyboard::Scancode::Up)
            if(Miscellaneous::Instance().EffectsOfMoving(player, "up", map, shouldExit, isFighting) == 1) continue;


          if (keyPressed->scancode == sf::Keyboard::Scancode::Down) 
            if(Miscellaneous::Instance().EffectsOfMoving(player, "down", map, shouldExit, isFighting) == 1) continue;


          if (keyPressed->scancode == sf::Keyboard::Scancode::Left) 
            if(Miscellaneous::Instance().EffectsOfMoving(player, "left", map, shouldExit, isFighting) == 1) continue;


          if (keyPressed->scancode == sf::Keyboard::Scancode::Right) 
            if(Miscellaneous::Instance().EffectsOfMoving(player, "right", map, shouldExit, isFighting) == 1) continue;
        }

        //////////////////////////////////////////////////////////////////////////////////
        // Misc. input:
        if (keyPressed->scancode == sf::Keyboard::Scancode::Num0) {
          shouldExit = true;
          break;
        }


        if (keyPressed->scancode == sf::Keyboard::Scancode::Num1 && !isFighting) {
          int c = map.CheckForItems();

          timerPickupText = 0;
          if(c == -1) RenderPickupText = 1; // we alr checked
          if(c == 0) RenderPickupText = 2;// nothing
          if (c == 1) { 
            RenderPickupText = 3;// resources
            player.RefillWater();
            player.RefillFood();
          }
          if(c == 2) {
            RenderPickupText = 4;// bullets
            player.SetBullets(8);
          }
          continue;
        }


        if (keyPressed->scancode == sf::Keyboard::Scancode::Equal && !isFighting) {
          map.GetCloserToExit(10);
          RenderCheat = 1;
          continue;
        }


        // if (keyPressed->scancode == sf::Keyboard::Scancode::Num2 && !isFighting) { // we display the inventory
        //   player.BackPack();
        //   continue;
        // }
      }
    }
    if (shouldExit) {
      window.close();
      break;
    }

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(10ms);

    //////////////////////////////////////////////////////////////////////////////////
    // Rendering / timers: 
    window.clear();
    Miscellaneous::Instance().Renders_WithOut_timers(window, map, isFighting, player);

    if(isFighting && hasGeneratedEnemy) {
      _enemy->PositionSprite();
      window.draw(_enemy->GetSprite());
    }
    if(map.GetShouldDisplayDeadEndText()) window.draw(UI::Instance().GetText(0));
    if(RenderTextMissed) {
      window.draw(UI::Instance().GetText(1));
      timerPlayer++;
      if(timerPlayer >= 120) {
        RenderTextMissed = 0;
        timerPlayer = 0;
      }
    }
    if(EnemyRenderTextMissed) {
      window.draw(UI::Instance().GetText(2));
      timerEnemy++;
      if(timerEnemy >= 120) {
        EnemyRenderTextMissed = 0;
        timerEnemy = 0;
      }
    }
    if(RenderPickupText) {
      timerPickupText++;

      if(RenderPickupText == 1) window.draw(UI::Instance().GetText(4));
      if(RenderPickupText == 2) window.draw(UI::Instance().GetText(7));
      if(RenderPickupText == 3) window.draw(UI::Instance().GetText(5));
      if(RenderPickupText == 4) window.draw(UI::Instance().GetText(6));
      
      if(timerPickupText >= 120) {
        RenderPickupText = 0;
        timerPickupText = 0;
      }
    }
    if(RenderCheat) {
      timerCheat++;
      window.draw(UI::Instance().GetText(11));
      
      if(timerCheat >= 120) {
        RenderCheat = 0;
        timerCheat = 0;
      }
    }
    if(RenderOffers) {
      const Trader* m = dynamic_cast<Trader*>(_enemy.get());
      if( m != nullptr) {
        window.draw(m->GetOffer1());
        window.draw(m->GetOffer2());
      }
    }
    if(UI::Instance().GetShiftingRooms()) {
      timerShiftingRooms++;
      window.draw(UI::Instance().GetText(12));
      
      if(timerShiftingRooms >= 120) {
        UI::Instance().SetShiftingRooms(0);
        timerShiftingRooms = 0;
      }
    }
    window.display();
  }


  window.close();
  intrare.close();
  return 0;
}