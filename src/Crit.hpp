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
#include "Events.hpp"

template <typename Subject>
class CritHit {

public:
    CritHit(Subject& _subject, int penalty, int damage, sf::RenderWindow &_window) : subject(_subject), penaltyApplied(penalty), window(_window) {
      subject.SetAccuracy(subject.GetAccuracy() - penaltyApplied);
      subject.SetHp(subject.GetHp() - damage * (critDmg - 1));

      critOnPlayer.emplace(txl::TextureLoader.Instance().GetUITexture("CritHitOnPlayer.png"));
      critOnEnemy.emplace(txl::TextureLoader.Instance().GetUITexture("CritHitOnEnemy.png"));
    }
    ~CritHit() = default;
    

private:
    void PlayAnimationOnPlayer() {
      while(1) {
        timerFadeIn++;
      }
    }
    int timerFadeIn = 0;

    Subject& subject;
    int penaltyApplied;
    float critDmg = 1.5;

    std::optional<sf::Sprite> critOnEnemy;
    std::optional<sf::Sprite> critOnPlayer;

    sf::RenderWindow &window;

//screenshake
};