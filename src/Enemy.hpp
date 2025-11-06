#pragma once

#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <chrono>
#include <thread>

#include "Player.hpp"
#include "Utility.hpp"

///////////////////////////////////////////////////////////////////////////////////
//
//
class Enemy {
  public:
///////////////////////////////////////////////////////////////////////////////////
    Enemy(int _type, int _hp);
    ~Enemy() = default;
    Enemy(const Enemy& other);
    Enemy& operator=(const Enemy& other) = default;
    friend std::ostream& operator<<(std::ostream& os,  const Enemy& enemy);
///////////////////////////////////////////////////////////////////////////////////

    // void Attack(Player &jucator, int chance); // the higher chance is the rarer the hits

    ///////////////////////////////////////////////////////////////////////////////////
  private: 
    int type;  //  id, deocamdata 2 tipuri
    int hp = 1;
    int damage;
    std::string texture = "";

  };
///////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& os,  const Enemy& enemy);