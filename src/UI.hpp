#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <array>
#include <vector>
#include <chrono>
#include <thread>
#include <unordered_map>

#include <SFML/Graphics.hpp>
#include "TextureLoader.hpp"
#include "Exceptions.hpp"
#include "Utility.hpp"
#include "Player.hpp"

// we use this one as a singleton
class UI {
  public:  

  static UI &Instance();
  void UpdateTheHp(Player &player);
  void UpdateTheAcc(Player &player);
  void UpdateTheBullets(Player &player);

  // positions all UI elements. to be used either on resize or on every frame
  void PositionUI(sf::RenderWindow &window);

  ////////////////////////////////////////////////////////////////////////////
  // Getters for the UI elements:
  const sf::Text &GetText(int which);

  const sf::Sprite &GetUI_Sprites(int which) {
    return *DirectionActive[which];
  }
  const sf::Sprite &GetEmptyUI_Sprites(int which) {
    return *DirectionDeActive[which];
  }
  const sf::Sprite &GetMiscUIsprite(std::string which) {
    return *(miscUIsprites[which]);
  }
  /////////////////////////////////////////////////////////////////////////////

  
  private:

  UI();
  ~UI() = default;


  std::optional<sf::Sprite> DirectionActive[4];
  std::optional<sf::Sprite> DirectionDeActive[4];

  std::unordered_map<std::string,std::optional<sf::Sprite>> miscUIsprites;
  
  std::unordered_map<std::string, std::optional<sf::Text>> Texts;
  

};