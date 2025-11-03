#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <chrono>
#include <thread>
#include <../SFML/Graphics.hpp>

/// texture loader se foloseste la inceput de main, la init deschide toate texturile
namespace txl {
  class TextureLoader {
      public:
      TextureLoader() = default;
      ~TextureLoader();

      
      int Load(std::string& fisier);
      
      private:
      std::vector<sf::Texture> oneExits;
      std::vector<sf::Texture> twoExits;
      std::vector<sf::Texture> threeExits;
      std::vector<sf::Texture> fourExits;
  } 
};