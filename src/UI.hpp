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

class UI {
  public:  

  static UI &Instance() {
    static UI instance;
    return instance;
  }

  void PositionUI(sf::RenderWindow &window) {
    float winX = window.getSize().x, winY = window.getSize().y;
    float posX_ctrls = winX *3 / 4;
    float posY_ctrls = winY *3 / 4;


    deadEnd->setPosition({20, 20});
    checkBackpack->setPosition({40, winY - 50});
    checkRoom->setPosition({40, winY - 90});

    DirectionActive[0]->setPosition({posX_ctrls, posY_ctrls - 30});
    DirectionActive[1]->setPosition({posX_ctrls, posY_ctrls + 30});
    DirectionActive[2]->setPosition({posX_ctrls - 30, posY_ctrls});
    DirectionActive[3]->setPosition({posX_ctrls + 30, posY_ctrls});

    DirectionDeActive[0]->setPosition({posX_ctrls, posY_ctrls - 30});
    DirectionDeActive[1]->setPosition({posX_ctrls, posY_ctrls + 30});
    DirectionDeActive[2]->setPosition({posX_ctrls - 30, posY_ctrls});
    DirectionDeActive[3]->setPosition({posX_ctrls + 30, posY_ctrls});
  }

  const sf::Text &GetText(int which) {
    if(which == 3) {
      return *deadEnd;
    }
    if(which == 2) {
      return *checkBackpack;
    }
      return *checkRoom;
  }
  const sf::Sprite &GetUI_Sprites(int which) {
    return *DirectionActive[which];
  }
  const sf::Sprite &GetEmptyUI_Sprites(int which) {
    return *DirectionDeActive[which];
  }


  private:

  UI() {
    for (int i = 0; i < 4; ++i) {
      DirectionActive[i].emplace(txl::TextureLoader::Instance().GetDefaultTexture());
      DirectionDeActive[i].emplace(txl::TextureLoader::Instance().GetDefaultTexture());
    }
    checkBackpack.emplace(txl::TextureLoader::Instance().GetFont(), "Check your backpack (2)", 30);
    checkRoom.emplace(txl::TextureLoader::Instance().GetFont(), "Check for items (1)", 30);
    deadEnd.emplace(txl::TextureLoader::Instance().GetFont(), ">Dead end", 15);
  
    try {
      DirectionActive[0]->setTexture(txl::TextureLoader::Instance().GetUITexture("Up.png"));
      DirectionActive[1]->setTexture(txl::TextureLoader::Instance().GetUITexture("Bottom.png"));
      DirectionActive[2]->setTexture(txl::TextureLoader::Instance().GetUITexture("Left.png"));
      DirectionActive[3]->setTexture(txl::TextureLoader::Instance().GetUITexture("Right.png"));
            
      DirectionDeActive[0]->setTexture(txl::TextureLoader::Instance().GetUITexture("Up-Empty.png"));
      DirectionDeActive[1]->setTexture(txl::TextureLoader::Instance().GetUITexture("Bottom-Empty.png"));
      DirectionDeActive[2]->setTexture(txl::TextureLoader::Instance().GetUITexture("Left-Empty.png"));
      DirectionDeActive[3]->setTexture(txl::TextureLoader::Instance().GetUITexture("Right-Empty.png"));

      DirectionActive[0]->setScale({0.55f, 0.55f});
      DirectionActive[1]->setScale({0.55f, 0.55f});
      DirectionActive[2]->setScale({0.55f, 0.55f});
      DirectionActive[3]->setScale({0.55f, 0.55f});

      DirectionDeActive[0]->setScale({0.55f, 0.55f});
      DirectionDeActive[1]->setScale({0.55f, 0.55f});
      DirectionDeActive[2]->setScale({0.55f, 0.55f});
      DirectionDeActive[3]->setScale({0.55f, 0.55f});
    }
    catch(TextureFileExceptionNoSuchFile &exp) {
      std::cout<<exp.what()<<"\n";
      exit(-1);
    }
  }
  ~UI() = default;


  std::optional<sf::Sprite> DirectionActive[4];
  std::optional<sf::Sprite> DirectionDeActive[4];
  
  std::optional<sf::Text> checkBackpack;
  std::optional<sf::Text> checkRoom;
  std::optional<sf::Text> deadEnd;

  // std::optional<sf::Text> foundSomeFood;
  // std::optional<sf::Text> foundSomeWater;
  // std::optional<sf::Text> youAlreadyCheckHere;
  
  

};