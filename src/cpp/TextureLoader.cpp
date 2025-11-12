#include "../TextureLoader.hpp"

namespace fsys = std::filesystem;

/// texture loader se foloseste la inceput de main, la init deschide toate texturile
namespace txl
{

  TextureLoader::TextureLoader()
  {
    LoadTextures(path0, 0);
    LoadTextures(path1, 1);
    LoadTextures(path2, 2);
    LoadTextures(path3, 3);
  }
  TextureLoader &TextureLoader::Instance()
  {
    static TextureLoader instance;
    return instance;
  }

  void TextureLoader::LoadTextures(const std::string &path, int nrOfExits)
  {
    nrOfExits--;

    if (nrOfExits > 3)
    {
      std::cout << "\n||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| 1\n";
    }
    for (fsys::directory_iterator fisiere(path); fisiere != fsys::directory_iterator(); fisiere++)
    {
      sf::Texture temp;
      const auto &fis = *fisiere;
      if (!fis.is_regular_file())
      {
        std::cout << "\n||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| 2\n";

        // throw is not regular file/ eroare la deschidere ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
      }
      if (!(fis.path().extension().string() == ".png"))
      {
        // throw exception for not png ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
        std::cout << "\n||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| 6\n";
      }
      if (!temp.loadFromFile(fis.path().string()))
      {
        // std::cout << "\nfailed to open " << fis.path();
        // throw failde to open texture from openned png ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
        std::cout << "\n||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| 3\n";
      }

      map_textures[nrOfExits][fis.path().filename()] = temp;
      std::cout << "\nLoaded: " << fis.path().filename() << "\n";
    }
  }

  sf::Texture &TextureLoader::GetTexture(int nrOfExits)
  {
    nrOfExits--;
    if (nrOfExits < 0 || nrOfExits > 3)
    {
      /////////throw ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
      std::cout << "\nrOfExits prost\n";
    }
    // if number of exits > 3 throw ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    std::cout << "\n||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| 4\n";

    std::string name = "txtr";
    int size = map_textures[nrOfExits].size();
    if (size <= 0)
    {
      /////////throw ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
      std::cout << "\nsize prea mic\n";
    }
    name += static_cast<char>('0' + nrOfExits); // adaugam sufixul de nume pentru folder
    name += std::to_string(RNG() % size);       // adaugam sufixul de nume de ID din folder
    name += ".png";                             // adaugam .png

    if (map_textures[nrOfExits].find(name) == map_textures[nrOfExits].end())
    {
      std::cout << "\n||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| 5\n";

      // daca nu il gasim aruncam exceptie ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    }
    return map_textures[nrOfExits][name];
  }
  // std::ostream &operator<<(std::ostream &out, const TextureLoader &)
  // {
  //   out << "\n";
  //   for (const auto &[key, value] : TextureLoader::Instance().map_textures[0])
  //   {
  //     out << key << '\n';
  //   }
  //   for (const auto &[key, value] : TextureLoader::Instance().map_textures[1])
  //   {
  //     out << key << '\n';
  //   }
  //   for (const auto &[key, value] : TextureLoader::Instance().map_textures[2])
  //   {
  //     out << key << '\n';
  //   }
  //   for (const auto &[key, value] : TextureLoader::Instance().map_textures[3])
  //   {
  //     out << key << '\n';
  //   }
  //   out << "\n";
  //   return out;
  // }
};
