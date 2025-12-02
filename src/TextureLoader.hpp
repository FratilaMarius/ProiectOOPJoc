#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <array>
#include <vector>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <list>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Exceptions.hpp"
#include "Utility.hpp"

namespace fsys = std::filesystem;

//////////////////////////////////////////////////////////////////////
//  Class TextureLoader stores all the used textures/fonts
//
namespace txl {
  class TextureLoader {
  public:
    TextureLoader(const TextureLoader &other) = delete;
    TextureLoader &operator=(const TextureLoader &other) = delete;

    static TextureLoader &Instance();

    sf::Texture &GetTexture(int nrOfExits);
    sf::Texture &GetUITexture(const std::string& which);
    sf::Texture &GetEnemyTexture(const std::string& which);
    sf::Font &GetFont() { return font; }
    sf::Texture &GetDefaultTexture() { return defaultTexture;}

    void GetSound(const std::string &which);
    // sf::Music &GetMusic(const std::string &which);
  private:
    TextureLoader();
    ~TextureLoader() = default;

    std::unordered_map<std::string, sf::Texture> map_textures[4];
    std::unordered_map<std::string, sf::Texture> map_UI;
    std::unordered_map<std::string, sf::Texture> map_Enemy;
    sf::Font font;
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
    std::optional<sf::Sound> sound;
    std::unordered_map<std::string, sf::Music> ambientMusic;

    std::string path0 = "Textures/0/";
    std::string path1 = "Textures/1/";
    std::string path2 = "Textures/2/";
    std::string path3 = "Textures/3/";

    std::string pathUI = "Textures/UI";
    std::string pathEnemy = "Textures/Enemy";

    std::string pathSounds = "Audio/Sounds";
    std::string pathMusic = "Audio/Music";
    // std::string pathDefault = "Textures/";

    sf::Texture defaultTexture;
    void LoadTextures(const std::string &path, int nrOfExits);
    void LoadUI();

    void LoadEnemyTex();

    void LoadSounds();
    void LoadMusic();
  };
}