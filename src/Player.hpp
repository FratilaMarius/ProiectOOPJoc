#pragma once

#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <chrono>
#include <thread>

///////////////////////////////////////////////////////////////////////////////////
// Class player stores the current status of the player (i.e. inventory not stuff about position)
class Player
{
public:
  explicit Player(int _hp);
  ~Player();
  Player(const Player &other);
  Player &operator=(const Player &other);
  friend std::ostream &operator<<(std::ostream &os, const Player &player);
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // Gameplay:
  void DealDamage(int dmg) { hp -= dmg; }
  void HurtAccuracy(int howMuch) { accuracy -= howMuch; }
  int GetAccuracy() const { return accuracy; }
  void SetAccuracy(int n) { accuracy = n; }
  int GetHp() const { return hp; }
  void SetHp(int n) { hp = n; }
  void AddHp(int amnt) { hp += amnt; }
  int GetBullets() const { return bullets; }
  int GetFood() const { return food; }
  int GetWater() const { return water; }
  void SetBullets(int n) { bullets = n; }
  void AddBullets(int n) { bullets += n; }
  void AddToAcc(int n) { accuracy += n; }
  int GetCritChance() { return critChance;}
  void SetDamage(int n) { damage = n;}
  int GetDamage() { return damage;}
  void SetCritChance(int n) { critChance = n;}
  void StealResources(int _water, int _food, int _bullets)
  {
    water -= _water;
    food -= _food;
    bullets -= _bullets;
  }

  void RefillWater();
  void AddWater(int n) { water += n; }
  void RefillFood();
  void AddFood(int n) { food += n; }
  int PlayerStatus();
  // void BackPack() const;
  // void TakeDmg(int dmg);
  ///////////////////////////////////////////////////////////////////////////////////////////////////

private:
  int hp;
  int accuracy = 99;
  int bullets = 8; 
  int lights = 8;  // unused
  int tent = 0;    // unuesd
  int meds = 25;   // unused
  int water = 105;
  int food = 102;
  int critChance = 10;
  int damage = 30;

  int movesUntilDemise = 5;

  const std::string OutOfFood = "\nYou have run out of food!";
  const std::string OutOfWater = "\nYou have run out of water!";
  const std::string outOfSupplies = "\nYou are out of supplies! Days until the elements overtake you: ";
};

std::ostream &operator<<(std::ostream &os, const Player &player);
