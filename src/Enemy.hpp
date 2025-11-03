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
    Enemy(const Enemy& other) { *this = other;}
    Enemy& operator=(const Enemy& other) = default;
///////////////////////////////////////////////////////////////////////////////////

    // void Attack(Player &jucator, int chance); // the higher chance is the rare-er the hits
int Type() const { return type; }
int Hp() const { return hp; }
int Damage() const { return damage; }
const std::string& Texture() const { return texture; }
    ///////////////////////////////////////////////////////////////////////////////////
  private: 
    int type;  //  id, deocamdata 2 tipuri
    int hp = 1;
    int damage;
    std::string texture = "";

  };
///////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& os,  const Enemy& enemy);