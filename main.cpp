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

int main() {
    // sf::RenderWindow window;
    // ///////////////////////////////////////////////////////////////////////////
    // /// NOTE: sync with env variable APP_WINDOW from .github/workflows/cmake.yml:31
    // window.create(sf::VideoMode({800, 700}), "My Window", sf::Style::Default);
    // ///////////////////////////////////////////////////////////////////////////
    // std::cout << "Fereastra a fost creată\n";
    // /////////////////////////////////////////////////////////////3)//////////////
    // /// NOTE: mandatory use one of vsync or FPS limit (not both)            ///
    // /// This is needed so we do not burn the GPU                            ///
    // // window.setVerticalSyncEnabled(true);                                 ///
    // window.setFramerateLimit(20);                                            ///
    // ///////////////////////////////////////////////////////////////////////////

    // while(window.isOpen()) {
    //     bool shouldExit = false;

    //     while(const std::optional event = window.pollEvent()) {
    //         if (event->is<sf::Event::Closed>()) {
    //             window.close();
    //             std::cout << "Fereastra a fost închisă\n";
    //         }
    //         else if (event->is<sf::Event::Resized>()) {
    //             std::cout << "New width: " << window.getSize().x << '\n'
    //                       << "New height: " << window.getSize().y << '\n';
    //         }
    //         else if (event->is<sf::Event::KeyPressed>()) {
    //             const auto* keyPressed = event->getIf<sf::Event::KeyPressed>();
    //             std::cout << "Received key " << (keyPressed->scancode == sf::Keyboard::Scancode::X ? "X" : "(other)") << "\n";
    //             if(keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
    //                 shouldExit = true;
    //             }
    //         }
    //     }
    //     if(shouldExit) {
    //         window.close();
    //         std::cout << "Fereastra a fost închisă (shouldExit == true)\n";
    //         break;
    //     }
    //     using namespace std::chrono_literals;
    //     std::this_thread::sleep_for(300ms);
        
    //     window.clear();
    //     window.display();
    // }



//////////////////////////////////////////////////////////////////////////
    std::srand(std::time(nullptr));
    std::ifstream intrare("dateIntrare.txt");
    if(!intrare.is_open()) {
      std::cout << "Eroare la deschiderea fisierului de intrare!\n";
      return -1;
    }


    int w = 11, h = 11;
    intrare>>w>>h;
    if((w < 3 || h < 3) || (w > 500 || h > 500)) {
      std::cout << "Dimensiuni invalide pentru labirint! (minim 3x3, maxim 500x500)\n";
      intrare.close();
      return -1;
    }
    std::cout<<"\nDimm lab: "<<w<<" "<<h<<"\n";

    int a = 2, b = 5;
    intrare>>a>>b;
    std::cout<<"\nCoord Spawn: "<<a<<" "<<b<<"\n";

    if( a >= w -1 || b >= h -1 || a <= 1 || b <= 1) {
      std::cout << "Coordonate invalide pentru spawn!\n";
      intrare.close();
      return -1;
    }
    
    Labyrinth map(w, h, a, b);
    Player jucator(100);

    while(1) {
      if(map.HasFinished()) break;
      std::cout<<map;
      std::cout<<"Continue? (1)  Check for Items? (2)  Check your backpack? (3)  End? (0)   ";
      std::cin>>a;
      if(a == 0) break;
      if(a == 2) {
        int c = map.CheckForItems();
        if(c == 1) jucator.RefillWater(); 
        if(c == 2) jucator.RefillFood();
        continue;
      }
      if(a == 3) {
        jucator.BackPack();
        continue;
      }
      if(a == 1) {
        if(map.Move()) {
          int alive = jucator.PlayerStatus();
          if(alive < 0) {
            std::cout << "\nYou died\n\n";
            return 0;
          }
        }
        continue;
      }
      std::cout<<"\nUnrecognised input\n";
    }
    std::cout << "Programul a terminat execuția\n";

    // window.close();
    intrare.close();
    return 0;
}

