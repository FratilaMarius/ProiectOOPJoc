#include "../UI.hpp"


UI &UI::Instance() {
  static UI instance;
  return instance;
}

void UI::UpdateTheHp(Player &player) {
  int hp = player.GetHp();
  if(hp < 0) hp = 0;
  miscUIsprites["hpBar"]->setScale({2 * (static_cast<float>(hp)/100), 2});
}
void UI::UpdateTheAcc(Player &player) {
  int acc = player.GetAccuracy();
  if(acc < 0) acc = 0;
  miscUIsprites["accBar"]->setScale({2 * (static_cast<float>(acc)/100), 2});
}
void UI::UpdateTheBullets(Player &player) {
  int b = player.GetBullets();
  Texts["Bullets: "]->setString("Bullets: " + std::to_string(b));
}

void UI::PositionUI(sf::RenderWindow &window) {
  float winX = window.getSize().x, winY = window.getSize().y;
  float posX_ctrls = winX *3 / 4;
  float posY_ctrls = winY *3 / 4;

  Texts["deadEnd"]->setPosition({20, 110});
  Texts["Missed"]->setPosition({200, 200});
  Texts["Bullets: "]->setPosition({10, 80});

  miscUIsprites["hpBar"]->setPosition({10, 10});
  miscUIsprites["hpBar_empty"]->setPosition({10, 10});
  miscUIsprites["accBar"]->setPosition({10, 45});
  miscUIsprites["accBar_empty"]->setPosition({10, 45});

  DirectionActive[0]->setPosition({posX_ctrls, posY_ctrls - 30});
  DirectionActive[1]->setPosition({posX_ctrls, posY_ctrls + 30});
  DirectionActive[2]->setPosition({posX_ctrls - 30, posY_ctrls});
  DirectionActive[3]->setPosition({posX_ctrls + 30, posY_ctrls});

  DirectionDeActive[0]->setPosition({posX_ctrls, posY_ctrls - 30});
  DirectionDeActive[1]->setPosition({posX_ctrls, posY_ctrls + 30});
  DirectionDeActive[2]->setPosition({posX_ctrls - 30, posY_ctrls});
  DirectionDeActive[3]->setPosition({posX_ctrls + 30, posY_ctrls});
}

const sf::Text &UI::GetText(int which) {
  switch(which) {
    case 0:
      return *(Texts["deadEnd"]);
      break;
    case 1:
      return *(Texts["Missed"]);
      break;
    case 2:
      return *(Texts["Bullets: "]);
      break;
    
    default:
      break;
  }
}

UI::UI() {
    for (int i = 0; i < 4; ++i) {
      DirectionActive[i].emplace(txl::TextureLoader::Instance().GetDefaultTexture());
      DirectionDeActive[i].emplace(txl::TextureLoader::Instance().GetDefaultTexture());
    }

    Texts["deadEnd"].emplace(txl::TextureLoader::Instance().GetFont(), ">Dead end", 15);
    Texts["Missed"].emplace(txl::TextureLoader::Instance().GetFont(), "Missed", 15);
    Texts["Bullets: "].emplace(txl::TextureLoader::Instance().GetFont(), "Ammo:", 15);
  
    try {

      miscUIsprites["hpBar"].emplace(txl::TextureLoader::Instance().GetUITexture("HP.png"));
      miscUIsprites["hpBar_empty"].emplace(txl::TextureLoader::Instance().GetUITexture("HP_Outline.png"));
      miscUIsprites["hpBar"]->setScale({2, 2});
      miscUIsprites["hpBar_empty"]->setScale({2, 2});

      miscUIsprites["accBar"].emplace(txl::TextureLoader::Instance().GetUITexture("HP.png"));
      miscUIsprites["accBar_empty"].emplace(txl::TextureLoader::Instance().GetUITexture("HP_Outline.png"));
      miscUIsprites["accBar"]->setScale({2, 2});
      miscUIsprites["accBar_empty"]->setScale({2, 2});      
      miscUIsprites["accBar"]->setColor({113, 113, 255, 255});

      DirectionActive[0]->setTexture(txl::TextureLoader::Instance().GetUITexture("Up.png"));
      DirectionActive[1]->setTexture(txl::TextureLoader::Instance().GetUITexture("Bottom.png"));
      DirectionActive[2]->setTexture(txl::TextureLoader::Instance().GetUITexture("Left.png"));
      DirectionActive[3]->setTexture(txl::TextureLoader::Instance().GetUITexture("Right.png"));
            
      DirectionDeActive[0]->setTexture(txl::TextureLoader::Instance().GetUITexture("Up-Empty.png"));
      DirectionDeActive[1]->setTexture(txl::TextureLoader::Instance().GetUITexture("Bottom-Empty.png"));
      DirectionDeActive[2]->setTexture(txl::TextureLoader::Instance().GetUITexture("Left-Empty.png"));
      DirectionDeActive[3]->setTexture(txl::TextureLoader::Instance().GetUITexture("Right-Empty.png"));

      DirectionActive[0]->setScale({0.55f, 0.55f});
      DirectionActive[1]->setScale({0.55f, 0.55f});
      DirectionActive[2]->setScale({0.55f, 0.55f});
      DirectionActive[3]->setScale({0.55f, 0.55f});

      DirectionDeActive[0]->setScale({0.55f, 0.55f}); 
      DirectionDeActive[1]->setScale({0.55f, 0.55f});
      DirectionDeActive[2]->setScale({0.55f, 0.55f});
      DirectionDeActive[3]->setScale({0.55f, 0.55f});
    }
    catch(TextureFileExceptionNoSuchFile &exp) {
      std::cout<<exp.what()<<"\n";
      exit(-1);
    }
  }