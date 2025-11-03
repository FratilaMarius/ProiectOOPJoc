#pragma once

#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <chrono>
#include <thread>

///////////////////////////////////////////////////////////////////////////////////
// clasa player retine toate informatiile despre player, un fel de inventar.
// nu este un lucru "fizic", nu contine date despre pozitia playerului spre exemplu
  class Player{
    public:
///////////////////////////////////////////////////////////////////////////////////
      explicit Player(int _hp);
      ~Player();
      Player(const Player& other);              //  copiatori
      Player& operator=(const Player& other);  //
///////////////////////////////////////////////////////////////////////////////////
      void RefillWater();  //  umple inapoi var de apa la numarul max
      void RefillFood();   //  lfl pentru mancare
        
      int PlayerStatus();  //verifica daca playerul mai traieste/mai are resurse si cat timp mai are fara, return -1 la moarte

      void BackPack() const; //printeaza continutul din backpack

      int Hp()const;       //
      int Bullets()const;  // printari de variabile private
      int Water()const;    //
      int Food()const;     //

      // void TakeDmg(int dmg);  // scade hp-ul
///////////////////////////////////////////////////////////////////////////////////
    private: 
      int hp;
      int accuracy = 1;
      int bullets = 6;
      int lights = 8;
      int tent = 0;
      int meds = 25;
      int water = 15;
      int food = 12;

      int movesUntilDemise = 5;

      const std::string OutOfFood = "\nYou have run out of food!\n";
      const std::string OutOfWater = "\nYou have run out of water!\n";
      const std::string outOfSupplies = "\nYou are out of supplies! Days until the elements overtake you: ";
  };
///////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& os,  const Player& player);
