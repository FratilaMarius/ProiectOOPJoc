#include "../TextureLoader.hpp"

using namespace txl;

TextureLoader::TextureLoader() {}
TextureLoader::~TextureLoader()
{
  textures[0].clear();
  textures[2].clear();
  textures[1].clear();
  textures[3].clear();
}
TextureLoader &TextureLoader::operator=(const TextureLoader &other)
{
  if (this == &other)
    return *this;
  for (int v = 0; v < 4; ++v)
  {
    txtNames[v] = other.txtNames[v];
    textures[v].clear();
    textures[v].reserve(txtNames[v].size());
    for (const auto &name : txtNames[v])
    {
      sf::Texture tex;
      if (!tex.loadFromFile(name))
      {
        std::cout << "TextureLoader assign: failed to reload " << name << '\n';
        sf::Texture empty;
        textures[v].push_back(std::move(empty));
      }
      else
      {
        textures[v].push_back(std::move(tex));
      }
    }
  }
  return *this;
}
TextureLoader::TextureLoader(const TextureLoader &other)
{
  for (int v = 0; v < 4; v++)
  {
    txtNames[v] = other.txtNames[v];
    textures[v].clear();
    textures[v].reserve(txtNames[v].size());
    for (const auto &name : txtNames[v])
    {
      sf::Texture tex;
      if (!tex.loadFromFile(name))
      {
        std::cout << "TextureLoader copy: failed to reload " << name << '\n';
        // push an empty texture so indices remain valid
        sf::Texture empty;
        textures[v].push_back(std::move(empty));
      }
      else
      {
        textures[v].push_back(std::move(tex));
      }
    }
  }
}

int TextureLoader::Load(int vectorToLoadInto, const std::string &_path)
{
  int cnt = 0;
  for (fsys::directory_iterator fisiere(_path); fisiere != fsys::directory_iterator(); fisiere++)
    cnt++; //  o rezervare de spatiu, reduce ~4 secunde din 20 la deschideri
  textures[vectorToLoadInto].reserve(cnt);
  txtNames[vectorToLoadInto].reserve(cnt);
  for (fsys::directory_iterator fisiere(_path); fisiere != fsys::directory_iterator(); fisiere++)
  {
    const auto &fis = *fisiere;
    if (fis.is_regular_file())
    {
      if (fis.path().extension().string() == ".png")
      {
        sf::Texture temp;
        if (!temp.loadFromFile(fis.path().string()))
          std::cout << "\nfailed to open " << fis.path();

        textures[vectorToLoadInto].push_back(std::move(temp));
        txtNames[vectorToLoadInto].push_back(fis.path().string());
      }
    }
    else
    { // daca nu e regular file a crapat cv probabil
      std::cout << "Eroare la deschiderea unei texturi ";
      return -1;
    }
  }
  return 0;
}

const sf::Texture &TextureLoader::GetTexture(int whichVector, int whichTexture)
{
  if (whichVector > 3)
  {
    std::cout << "\nError 1 while getting texture png \n";
    return textures[0][0];
  }
  if (whichTexture > static_cast<int>(textures[whichVector].size()))
  {
    std::cout << "\nError 2 while getting texture png \n";
    return textures[0][0];
  }
  return textures[whichVector][whichTexture];
}
const std::string &TextureLoader::GetTextureName(int whichVector, int whichTexture)
{
  if (whichVector > 3)
  {
    std::cout << "\nError 1 while getting texture png \n";
    return txtNames[0][0];
  }
  if (whichTexture > static_cast<int>(txtNames[whichVector].size()))
  {
    std::cout << "\nError 2 while getting texture png \n";
    return txtNames[0][0];
  }
  return txtNames[whichVector][whichTexture];
}

namespace txl
{
  std::ostream &operator<<(std::ostream &out, const TextureLoader &txtrld)
  {
    out << "\n\n";
    int temp;

    temp = static_cast<int>(txtrld.txtNames[0].size());
    for (int i = 0; i < temp; i++)
      out << txtrld.txtNames[0][i] << " | ";
    out << "\n";

    temp = static_cast<int>(txtrld.txtNames[1].size());
    for (int i = 0; i < temp; i++)
      out << txtrld.txtNames[1][i] << " | ";
    out << "\n";

    temp = static_cast<int>(txtrld.txtNames[2].size());
    for (int i = 0; i < temp; i++)
      out << txtrld.txtNames[2][i] << " | ";
    out << "\n";

    temp = static_cast<int>(txtrld.txtNames[3].size());
    for (int i = 0; i < temp; i++)
      out << txtrld.txtNames[3][i] << " | ";
    out << "\n\n";

    return out;
  }
}