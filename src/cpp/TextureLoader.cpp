#include "../TextureLoader.hpp"

using namespace txl;

TextureLoader::~TextureLoader() {
  oneExits.clear();
  twoExits.clear();
  threeExits.clear();
  fourExits.clear();
}
int TextureLoader::Load(std::string& fisier) {
  
}