#pragma once

#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <chrono>
#include <thread>

///////////////////////////////////////////////////////////////////////////////////
// Class player stores the current status of the player (i.e. inventory not stuff about position)
class Player {
public:
  explicit Player(int _hp);
  ~Player();
  Player(const Player &other);            
  Player &operator=(const Player &other); 
  friend std::ostream &operator<<(std::ostream &os, const Player &player);
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // Gameplay:  
  void DealDamage(int dmg) { hp -= dmg;}
  void HurtAccuracy(int howMuch) { accuracy-= howMuch; }
  int GetAccuracy() const { return accuracy; }
  int GetHp() const { return hp; }
  int GetBullets() const { return bullets; }
  void SetBullets(int n) { bullets = n; }

  void RefillWater();
  void RefillFood();
  int PlayerStatus();
  void BackPack() const;
  // void TakeDmg(int dmg);
  ///////////////////////////////////////////////////////////////////////////////////////////////////

private:
  int hp;
  int accuracy = 100;
  int bullets = 6;   // unused
  int lights = 8;    // unused
  int tent = 0;      // unuesd
  int meds = 25;     // unused
  int water = 15;
  int food = 12;

  int movesUntilDemise = 5;

  const std::string OutOfFood = "\nYou have run out of food!";
  const std::string OutOfWater = "\nYou have run out of water!";
  const std::string outOfSupplies = "\nYou are out of supplies! Days until the elements overtake you: ";
};

std::ostream &operator<<(std::ostream &os, const Player &player);
