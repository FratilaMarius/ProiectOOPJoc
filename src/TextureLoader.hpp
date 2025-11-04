#include <iostream>
#include <fstream>
#include <filesystem>
#include <array>
#include <vector>
#include <chrono>
#include <thread>
#include <../SFML/Graphics.hpp>

namespace fsys = std::filesystem;
/// texture loader se foloseste la inceput de main, la init deschide toate texturile
namespace txl {
  class TextureLoader {
      public:
      TextureLoader() = default;
      ~TextureLoader();

      
      int Load(std::string& fisier, int vectorToLoadInto);
      
      private:
      std::vector<sf::Texture> textures[4];

      std::string path = "assets/textures";
  };
};