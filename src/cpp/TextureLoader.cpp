#include "../TextureLoader.hpp"

using namespace txl;

TextureLoader::~TextureLoader() {
  textures[0].clear();
  textures[2].clear();
  textures[1].clear();
  textures[3].clear();
}
int TextureLoader::Load(const std::string& path, int vectorToLoadInto) {
  for (fsys::directory_iterator fisiere(path); fisiere != fsys::directory_iterator(); fisiere++) {
    const auto &fis = *fisiere;
    if (fis.is_regular_file()) {
      if(fis.path().extension().string() == ".png") {
        sf::Texture temp;
        if (!temp.loadFromFile(fis)) std::cout<< "\nfailed to open "<<fis.path();
        
        textures[vectorToLoadInto].push_back(temp);
      }
    }
    else {  // daca nu e regular file a crapat cv probabil

    }
  } 
  return 0;
}