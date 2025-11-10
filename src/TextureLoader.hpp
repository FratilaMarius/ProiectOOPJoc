#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <array>
#include <vector>
#include <chrono>
#include <thread>

#include <SFML/Graphics.hpp>

namespace fsys = std::filesystem;
/// texture loader se foloseste la inceput de main, la init deschide toate texturile
namespace txl
{
  class TextureLoader
  {
  public:
    TextureLoader();
    ~TextureLoader();
    TextureLoader& operator=(const TextureLoader& other);
    TextureLoader(const TextureLoader& other);
    friend std::ostream &operator<<(std::ostream &out, const TextureLoader &txtrld);

    int Load(int vectorToLoadInto, const std::string &_path);

    const std::string &Path0() const { return path0; }
    const std::string &Path1() const { return path1; }
    const std::string &Path2() const { return path2; }
    const std::string &Path3() const { return path3; }

    int GetNrTextures(int which) { return txtNames[which].size(); }
    const sf::Texture& GetTexture(int whichVector, int whichTexture);
    const std::string &GetTextureName(int whichVector, int whichTexture);

  private:
    std::vector<sf::Texture> textures[4];
    std::vector<std::string> txtNames[4];
    std::string path0 = "Textures/0";
    std::string path1 = "Textures/1";
    std::string path2 = "Textures/2";
    std::string path3 = "Textures/3";
  };
  std::ostream &operator<<(std::ostream &out, const TextureLoader &txtrld);
}
