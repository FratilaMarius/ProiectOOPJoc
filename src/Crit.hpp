#pragma once

#include <iostream>
#include <fstream>
#include <array>
#include <optional>
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
#include "Events.hpp"

template <typename Subject>
class CritHit
{

public:
  CritHit(Subject &_subject, int penaltyPerCent, int damage, sf::RenderWindow &_window) : subject(_subject), penaltyApplied(penaltyPerCent), window(_window)
  {
    CritEffects(damage);
  }
  ~CritHit() = default;

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //
  // onPlayer version
  void PlayAnimationOnPlayer(sf::Sprite &spriteEnemy, sf::Sprite &background)
  {

    std::optional<sf::Sprite> temp;

    temp.emplace(txl::TextureLoader::Instance().GetUITexture("CritHitOnPlayer.png"));
    float WinScaleX = window.getSize().x;
    float WinScaleY = window.getSize().y;
    float TextureScaleX = temp->getTexture().getSize().x;
    float TextureScaleY = temp->getTexture().getSize().y;

    float scale = std::max(WinScaleX / TextureScaleX, WinScaleY / TextureScaleY);
    temp->setScale({scale, scale});
    temp->setOrigin({temp->getLocalBounds().getCenter().x, temp->getLocalBounds().getCenter().y}); // center
    temp->setPosition({WinScaleX / 2, WinScaleY / 2});
    temp->setColor({temp->getColor().r, temp->getColor().g, temp->getColor().b, static_cast<uint8_t>(temp->getColor().a - 125)});

    while (1)
    {
      timerFadeIn++;
      sf::Color c = temp->getColor();

      if (c.a < fadespeed)
        c.a = 70;
      c.a -= fadespeed;

      temp->setColor(c);

      window.clear();

      window.draw(background);
      window.draw(spriteEnemy);
      window.draw(temp.value());

      window.display();
      if (timerFadeIn == 55)
        break;
    }
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //
  // onEnemy version
  void PlayAnimationOnEnemy(sf::Sprite &spriteEnemy, sf::Sprite &background)
  {
    std::optional<sf::Sprite> temp;

    temp.emplace(txl::TextureLoader::Instance().GetUITexture("CritHitOnEnemy.png"));
    temp->setOrigin({static_cast<float>(temp->getTexture().getSize().x / 2), static_cast<float>(temp->getTexture().getSize().y / 2)});
    temp->setColor({temp->getColor().r, temp->getColor().g, temp->getColor().b, static_cast<uint8_t>(temp->getColor().a - 100)});
    temp->setPosition({static_cast<float>(window.getSize().x / 2), static_cast<float>(window.getSize().y / 2)});
    temp->setScale({2.f, 2.f});
    while (1)
    {
      timerFadeIn++;

      window.clear();

      window.draw(background);
      window.draw(spriteEnemy);
      window.draw(temp.value());

      window.display();
      if (timerFadeIn == 35)
        break;
    }
  }

private:
  void CritEffects(int damage)
  {
    subject.SetAccuracy(subject.GetAccuracy() - penaltyApplied);
    subject.SetHp(subject.GetHp() - damage * (critDmg - 1));

    // critOnPlayer.emplace(txl::TextureLoader::Instance().GetUITexture("CritHitOnPlayer.png"));
    // critOnEnemy.emplace(txl::TextureLoader::Instance().GetUITexture("CritHitOnEnemy.png"));
  }

  int timerFadeIn = 0;
  int fadespeed = 5; // 255 / 5 =  51 steps
  Subject &subject;
  int penaltyApplied;
  float critDmg = 1.75;

  // std::optional<sf::Sprite> critOnEnemy;
  // std::optional<sf::Sprite> critOnPlayer;

  sf::RenderWindow &window;

  // screenshake
};