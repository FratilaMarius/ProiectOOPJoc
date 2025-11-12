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
#include "Utility.hpp"

namespace fsys = std::filesystem;

namespace txl
{
  class TextureLoader
  {
  public:
    TextureLoader(const TextureLoader &other) = delete;
    TextureLoader &operator=(const TextureLoader &other) = delete;

    static TextureLoader &Instance();

    // friend   std::ostream &operator<<(std::ostream &out, const TextureLoader&);

    sf::Texture &GetTexture(int nrOfExits);

  private:
    TextureLoader();
    ~TextureLoader() = default;

    std::unordered_map<std::string, sf::Texture> map_textures[4];
    sf::Texture defaultTexture;

    std::string path0 = "Textures/0";
    std::string path1 = "Textures/1";
    std::string path2 = "Textures/2";
    std::string path3 = "Textures/3";

    void LoadTextures(const std::string &path, int nrOfExits);
  };
  // std::ostream &operator<<(std::ostream &out, const TextureLoader&);
}
