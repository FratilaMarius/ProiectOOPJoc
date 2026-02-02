#pragma once

#include <iostream>
#include <fstream>
#include <array>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>

#include "Player.hpp"
#include "Enemy.hpp"
#include "Utility.hpp"
#include "Room.hpp"
#include "Labyrinth.hpp"
#include "TextureLoader.hpp"
#include "Exceptions.hpp"
#include "UI.hpp"
#include "Miscellaneous.hpp"

class Cnev // = Context Event, for short
{
private:
  Cnev() = default;
  ~Cnev() = default;

int GeneratedEvent = -1;
int affectTheNextEnemy = 0;

public:
  static Cnev &Instance()
  {
    static Cnev instance;
    return instance;
  }
  int &GetGeneratedEvent() { return GeneratedEvent; }
  void SetGeneratedEvent(int n) { GeneratedEvent = n; }
  int &GetAffectTheNextEnemy() { return affectTheNextEnemy; }
  void SetAffectTheNextEnemy(int n) { affectTheNextEnemy = n; }

};

class Event
{
public:
  Event(Player &_player, sf::Texture &texture, sf::RenderWindow &_window, Labyrinth &_map);
  virtual ~Event() = default;

  virtual void ApplyEvent() = 0;
  virtual void PlayAnimation() = 0; // game runs at 60 fps i need to remember that for making the animations

  Player &GetPlayer() { return player; }
  Labyrinth &GetLab() { return map; }
  sf::RenderWindow &GetWindow() { return window; };
  sf::Sprite &GetSpriteEvent() { return spriteEvent.value(); }
  sf::Sprite &GetSpriteBackground() { return spriteBackground; }

  sf::Text &GetRequest() { return request.value(); }
  sf::Text &GetConclusion() { return conclusion.value(); }

private:
  Player &player;
  sf::RenderWindow &window;
  std::optional<sf::Sprite> spriteEvent;
  sf::Sprite &spriteBackground;
  Labyrinth &map;

  std::optional<sf::Text> request;
  std::optional<sf::Text> conclusion;
};

class EventTent final : public Event
{
public:
  EventTent(Player &_player, sf::RenderWindow &_window, Labyrinth &_map);
  ~EventTent() override = default;

  void ApplyEvent() override;

  void PlayAnimation() override;

private:
  int hpBonus = 100;
  int accBonus = 50;

  int animationTimerFade = 0;
  int fadeSpeed = 3;
};

class EventHole final : public Event
{
public:
  EventHole(Player &_player, sf::RenderWindow &_window, Labyrinth &_map);
  ~EventHole() override = default;

  void ApplyEvent() override;

  void PlayAnimation() override;

private:
  int hpLoss = 10;
  int accLoss = 2;
  int fadeSpeed = 5;

  std::vector<std::string> windowNames = {"Window1.png", "Window2.png", "Window3.png", "Window4.png", "Window5.png", "Window6.png", "Window0.png"};
};

class EventPage final : public Event
{
public:
  EventPage(Player &_player, sf::RenderWindow &_window, Labyrinth &_map);
  ~EventPage() override = default;

  void ApplyEvent() override;
  void PlayAnimation() override;

private:
  int accBonus = 10;
};

class EventGenerator
{
public:
  static EventGenerator &Instance();
  std::unique_ptr<Event> GenerateEvent(Player &_player, sf::RenderWindow &_window, Labyrinth &_map);

private:
  EventGenerator() = default;
  ~EventGenerator() = default;
};