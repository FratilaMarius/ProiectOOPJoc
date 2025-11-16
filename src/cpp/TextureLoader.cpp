#include "../TextureLoader.hpp"

namespace fsys = std::filesystem;

/// texture loader se foloseste la inceput de main, la init deschide toate texturile
namespace txl
{

  TextureLoader::TextureLoader()
  {
    if (defaultTexture.loadFromFile("Textures/0/txtr01.png"))
    {
      std::cout << "\nLoaded: Textures/0/txtr01.png as default" << "\n";
    }

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

      map_textures[nrOfExits][fis.path().filename().string()] = temp;
      std::cout << "\nLoaded: " << fis.path().filename() <<" pe vectorul "<<nrOfExits<< "\n";
    }
  }

  sf::Texture &TextureLoader::GetTexture(int nrOfExits)
  {
    nrOfExits--;
    std::cout<<"    GetTexture() in TXL: nrOfExits e "<< nrOfExits;
    if (nrOfExits < 0 || nrOfExits > 3)
    {
      /////////throw ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
      std::cout << "\nrOfExits prost\n";
    }
    // if number of exits > 3 throw ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

    std::string name = "txtr";
    int size;
    size = map_textures[nrOfExits].size();
    if (size <= 0)
    {
      /////////throw ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
      std::cout << "\nsize prea mic\n";
    }
    name += static_cast<char>('0' + nrOfExits); // adaugam sufixul de nume pentru folder
    int temp = RNG() % size;
    if (temp == 0)
      temp++;
    name += std::to_string(temp); // adaugam sufixul de nume de ID din folder
    name += ".png";               // adaugam .png

    if (map_textures[nrOfExits].find(name) == map_textures[nrOfExits].end())
    {
      std::cout << "\n||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| 5\n";

      // daca nu il gasim aruncam exceptie ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    }
    std::cout << "\nGAVE TEXTURE " << name <<" de pe vectorul "<<nrOfExits<<"\n";
    return map_textures[nrOfExits][name];
  }
};
