#include "../TextureLoader.hpp"

namespace fsys = std::filesystem;

namespace txl
{

  TextureLoader::TextureLoader() {
    try {
      if(!defaultTexture.loadFromFile("Textures/0/txtr01.png")) throw(FileException("default texture"));
      else std::cout << "\nLoaded: Textures/0/txtr01.png as default";
    }
    catch(FileException &exp) {
      std::cout<< exp.what() <<'\n';
      exit(-1);
    }

    try{
      LoadTextures(path0, 0);
      LoadTextures(path1, 1);
      LoadTextures(path2, 2);
      LoadTextures(path3, 3);

      LoadUI();
      if(!font.openFromFile("Textures/CourierPrime-Regular.ttf")) throw(FileException("Textures/CourierPrime-Regular.ttf"));
        else std::cout << "\nLoaded: CourierPrime-Regular.ttf as font";
    } 
    catch(TextureFileExceptionOutOfBounds &exp) {
      std::cout<< exp.what() <<'\n';
      exit(-1);
    }    
    catch(TextureFileExceptionCorrupted &exp) {
      std::cout<< exp.what() <<'\n';
      exit(-1);      
    }    
    catch(TextureFileExceptionExtension &exp) {
      std::cout<< exp.what() <<'\n';
      exit(-1);      
    }    
    catch(FileException &exp) {
      std::cout<< exp.what() <<'\n';
      exit(-1);     
    }
  }
  TextureLoader &TextureLoader::Instance() {
    static TextureLoader instance;
    return instance;
  }

  // Loads all the room textures from the numbered folders 0,1,2,3
  void TextureLoader::LoadTextures(const std::string &path, int nrOfExits) {
    if (nrOfExits > 3 || nrOfExits < 0) {
      throw(TextureFileExceptionOutOfBounds(nrOfExits));
    }

    for (fsys::directory_iterator file(path); file != fsys::directory_iterator(); file++) {
      sf::Texture temp;
      const auto &fis = *file;
      if (!fis.is_regular_file()) {
        throw(TextureFileExceptionCorrupted(fis.path().filename().string()));
      }
      if (!(fis.path().extension().string() == ".png")) {
        throw(TextureFileExceptionExtension(fis.path().filename().string()));
      }
      if (!temp.loadFromFile(fis.path().string())) {
        throw(FileException(fis.path().filename().string()));
      }

      map_textures[nrOfExits][fis.path().filename().string()] = temp;
      std::cout << "\nLoaded: " << fis.path().filename();
    }
  }

  // Loads all UI related textures
  void TextureLoader::LoadUI() {
    for (fsys::directory_iterator file(pathUI); file != fsys::directory_iterator(); file++) {
      sf::Texture temp;
      const auto &fis = *file;
      if (!fis.is_regular_file()) {
        throw(TextureFileExceptionCorrupted(fis.path().filename().string()));
      }
      if (!(fis.path().extension().string() == ".png")) {
        throw(TextureFileExceptionExtension(fis.path().filename().string()));
      }
      if (!temp.loadFromFile(fis.path().string())) {
        throw(FileException(fis.path().filename().string()));
      }

      map_UI[fis.path().filename().string()] = temp;
      std::cout << "\nLoaded: " << fis.path().filename().string();
    }
  }

  // ret a random texture depending on nrOfExits
  // possible Exceptions: OutOfBounds, EmptyArray, NoSuchFile
  sf::Texture &TextureLoader::GetTexture(int nrOfExits) {
    nrOfExits--;
    if (nrOfExits < 0 || nrOfExits > 3) {
      throw(TextureFileExceptionOutOfBounds(nrOfExits));
    }

    std::string name = "txtr";
    int size;
    size = map_textures[nrOfExits].size();
    if (size <= 0) {
      throw(TextureFileExceptionEmptyArray(nrOfExits));
    }
    
    // building the name of the file//////////////
    name += static_cast<char>('0' + nrOfExits);
    int temp = RNG() % size;
    if (temp == 0)
      temp++;
    name += std::to_string(temp); 
    name += ".png";               
    ///////////////////////////////////////////////

    if (map_textures[nrOfExits].find(name) == map_textures[nrOfExits].end()) {
      throw(TextureFileExceptionNoSuchFile(nrOfExits, name));
    }
    return map_textures[nrOfExits][name];
  }

  // returns a UI related texture, by name
  sf::Texture &TextureLoader::GetUITexture(std::string which) {
    if (map_UI.find(which) == map_UI.end()) {
      throw(TextureFileExceptionNoSuchFile(5, which));
    }
    return map_UI[which];
  }
};
  
