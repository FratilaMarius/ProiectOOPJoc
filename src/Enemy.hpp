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
#include "UI.hpp"

class Labyrinth; 
///////////////////////////////////////////////////////////////////////////////////
//
//
class Enemy {
  public:
///////////////////////////////////////////////////////////////////////////////////
// Constr/Destr:
    Enemy(int _hp, sf::Texture &_texture) : hp(_hp) { sprite.emplace(_texture); };
    virtual ~Enemy() = default;
    Enemy(const Enemy& other) = default;
    Enemy& operator=(const Enemy& other) = default;
///////////////////////////////////////////////////////////////////////////////////
// Gameplay:
  virtual int attack(Player& player) = 0;
  virtual void applyOffer(Player& /*player*/, Labyrinth& /*lab*/, int /*selectedOffer*/) {} // we'll use this one in the trader interaction
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
    ~Shade() override = default;
    Shade(const Shade& other);
    Shade& operator=(const Shade &other) = delete; // nu pot avea op= din cauza referintei la &window

    void PositionSprite() override;

    std::unique_ptr<Enemy> Clone() const override { return std::make_unique<Shade>(*this); }

    // if the chance hits right, the shade deals dmg to the hp and accuracy of the player
    int attack(Player& player) override;
  private:
    sf::RenderWindow &window;
    int psichDmg;

    void PlaySounds(int which) override;
};

class Minotaur : public Enemy {
  public:
    Minotaur(sf::Texture& _texture, sf::RenderWindow &_window);
    ~Minotaur() override = default;
    Minotaur(const Minotaur& other) : Enemy(other), window(other.window) {};
    Minotaur& operator=(const Minotaur &other) = delete; 

    void PositionSprite() override;

    std::unique_ptr<Enemy> Clone() const override { return std::make_unique<Minotaur>(*this); }

    // if the chance hits right, the Minotaur deals dmg to the hp of the player
    int attack(Player& player) override;

  private:
    sf::RenderWindow &window;
    void PlaySounds(int which) override;
};

class Blob : public Enemy {  public:
    Blob(sf::Texture& _texture, sf::RenderWindow &_window);
    ~Blob() override = default;
    Blob(const Blob& other) : Enemy(other), window(other.window) {};
    Blob& operator=(const Minotaur &other) = delete; 

    void PositionSprite() override;

    std::unique_ptr<Enemy> Clone() const override { return std::make_unique<Blob>(*this); }

    // if the chance hits right, the blob will steal some of the players resources
    int attack(Player& player) override;

  private:
    sf::RenderWindow &window;
    void PlaySounds(int which) override;
};

class Trader : public Enemy {
  public:
    Trader(sf::Texture& _texture, sf::RenderWindow &_window);
    ~Trader() override = default;
    Trader(const Trader& other) : Enemy(other), window(other.window), a(other.a), b(other.b), c(other.c), d(other.d), e(other.e) {};
    Trader& operator=(const Trader &other) = delete; 

    void PositionSprite() override;

    std::unique_ptr<Enemy> Clone() const override { return std::make_unique<Trader>(*this); }

    int attack(Player& player) override;
    void applyOffer(Player& player, Labyrinth& lab, int selectedOffer) override;

    const sf::Text& GetOffer1() const { return offer1T.value(); }
    const sf::Text& GetOffer2() const { return offer2T.value(); }
    std::optional<sf::Text> offer1T, offer2T;


  private:
    sf::RenderWindow &window;
    void PlaySounds(int which) override;

    int a = -1, b = 0, c = 0, d = 0, e = 0; // used to decide offers, see definition of Trader::Attack
    std::string offer1;
    std::string offer2;
  };

class EncounterManager {
  public:
    static EncounterManager &Instance();

    // we call this when a bool in main is true and we press leftClck
    int Fight(Enemy* enemy, Player& player, Labyrinth& lab, int &RenderTextMissed, int &EnemyRenderTextMissed, int &Shots, int selectedOffer);

    std::unique_ptr<Enemy> GenerateAnEnemy(sf::RenderWindow &window, const Player &player);
  private:
    EncounterManager() = default;
    ~EncounterManager() = default;
};