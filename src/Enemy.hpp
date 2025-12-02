#pragma once

#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <chrono>
#include <thread>
#include <memory>

#include "Player.hpp"
#include "Utility.hpp"
#include "TextureLoader.hpp"


///////////////////////////////////////////////////////////////////////////////////
//
//
class Enemy {
  public:
///////////////////////////////////////////////////////////////////////////////////
// Constr/Destr:
    Enemy(int _hp, sf::Texture &_texture) : hp(_hp) { sprite.emplace(_texture); };
    ~Enemy() = default;
    Enemy(const Enemy& other) = default;
    Enemy& operator=(const Enemy& other) = default;
///////////////////////////////////////////////////////////////////////////////////
// Gameplay:
  virtual void attack(Player& player) = 0;

  virtual std::unique_ptr<Enemy> Clone() const = 0;

  void PlayAudio(int which) { PlaySounds(which); }

  void SetDamge(int amount) { damage = amount; }
  int GetDamage() { return damage; }

  sf::Sprite& GetSprite() { return sprite.value();}
  sf::Sprite* GetSpriteAddr() { return &sprite.value(); }
  virtual void PositionSprite() = 0;

  int GetHp() { return hp;}
  void HurtEnemy(int dmg) { hp -= dmg; }
///////////////////////////////////////////////////////////////////////////////////
  private: 
    int hp = 1;
    int damage = 1;
    std::optional<sf::Sprite> sprite;

    virtual void PlaySounds(int which) = 0;
};


////////////////////////////////////////////////////////////////////////////////////
class Shade : public Enemy {
  public:
    Shade(sf::Texture& _texture, sf::RenderWindow &_window);
    ~Shade() = default;
    Shade(const Shade& other) = default;
    Shade& operator=(const Shade &other) = default;

    void PositionSprite() override;

    std::unique_ptr<Enemy> Clone() const override { return std::make_unique<Shade>(*this); }

    // if the chance hits right, the shade deals dmg to the hp and accuracy of the player
    void attack(Player& player) override;

  private:
    int psichDmg;
    sf::RenderWindow &window;

    void PlaySounds(int which) override;
};

class Minotaur : public Enemy {
  public:
  private:
};

class Guy : public Enemy { 
  public:
  private:
};

class Blob : public Enemy {
  public:
  private:
};

class EncounterManager {
  public:
    static EncounterManager &Instance();

    // we call this when a bool in main is true and we press leftClck
    int Fight(Enemy& enemy, Player& player, int &RenderTextMissed);

    std::unique_ptr<Enemy> GenerateAnEnemy(sf::RenderWindow &window, const Player &player);
  private:
    EncounterManager() = default;
    ~EncounterManager() = default;
};