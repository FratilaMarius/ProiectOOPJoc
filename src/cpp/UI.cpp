#include "../UI.hpp"


UI &UI::Instance() {
  static UI instance;
  return instance;
}

void UI::UpdateTheHp(const Player &player) {
  int hp = player.GetHp();
  if(hp < 0) hp = 0;
  float multiplier = static_cast<float>(hp)/100;
  if(multiplier > 2) multiplier = 2;
  miscUIsprites["hpBar"]->setScale({2 * multiplier, 2});
  Texts["hp"]->setString("Hp: " + std::to_string(hp));
}
void UI::UpdateTheAcc(const Player &player) {
  int acc = player.GetAccuracy();
  if(acc < 0) acc = 0;
  float multiplier = static_cast<float>(acc)/100;
  if(multiplier > 2) multiplier = 2;
  miscUIsprites["accBar"]->setScale({2 * multiplier, 2});
  Texts["acc"]->setString("Acc: " + std::to_string(acc));
}
void UI::UpdateTheBullets(const Player &player) {
  int b = player.GetBullets();
  Texts["Bullets: "]->setString("Bullets: " + std::to_string(b));
}
void UI::UpdateTheInv(const Player &player) {
  const std::string s = "Food: " + std::to_string(player.GetFood()) + " | Water: " + std::to_string(player.GetWater());
  Texts["RescInv"]->setString(s);
}

void UI::PositionUI(sf::RenderWindow &window) {
  float winX = window.getSize().x, winY = window.getSize().y;
  float posX_ctrls = winX *3 / 4;
  float posY_ctrls = winY *3 / 4;

  Texts["deadEnd"]->setPosition({20, 110});
  Texts["Missed"]->setPosition({200, 200});
  Texts["Enemy Missed"]->setPosition({250, 310});
  Texts["Bullets: "]->setPosition({10, 80});
  Texts["RescInv"]->setPosition({winX - 210, winY - 25});
  Texts["Cheat"]->setPosition({winX / 2 - 50, winY -35});

  Texts["AlrCheck: "]->setPosition({10, winY - 25});
  Texts["Resources"]->setPosition({10, winY - 25});
  Texts["FoundBull"]->setPosition({10, winY - 25});
  Texts["Nothing"]->setPosition({10, winY - 25});

  miscUIsprites["hpBar"]->setPosition({10, 10});
  miscUIsprites["hpBar_empty"]->setPosition({10, 10});
  miscUIsprites["accBar"]->setPosition({10, 45});
  miscUIsprites["accBar_empty"]->setPosition({10, 45});
  Texts["hp"]->setPosition({15, 15});
  Texts["acc"]->setPosition({15, 50});


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
      // Texts["Missed"]->setPosition({100 + RNG(), 100 + RNG()});
      return *(Texts["Missed"]);
      break;
    case 2:
      return *(Texts["Enemy Missed"]);
      break;
    case 3:
      return *(Texts["Bullets: "]);
      break;
    case 4:
      return *(Texts["AlrCheck: "]);
      break;
    case 5:
      return *(Texts["Resources"]);
      break;
    case 6:
      return *(Texts["FoundBull"]);
      break;
    case 7:
      return *(Texts["Nothing"]);
      break;
    case 8:
      return *(Texts["RescInv"]);
      break;
    case 9:
      return *(Texts["hp"]);
      break;
    case 10:
      return *(Texts["acc"]);
      break;
    case 11:
      return *(Texts["Cheat"]);
      break;
      default:
      break;
  }
  return *(Texts["deadEnd"]);
}



UI::UI() {
    for (int i = 0; i < 4; ++i) {
      DirectionActive[i].emplace(txl::TextureLoader::Instance().GetDefaultTexture());
      DirectionDeActive[i].emplace(txl::TextureLoader::Instance().GetDefaultTexture());
    }
  
    try {
      Texts["hp"].emplace(txl::TextureLoader::Instance().GetFont(), "Hp: ", 15);
      Texts["acc"].emplace(txl::TextureLoader::Instance().GetFont(), "Acc: ", 15);


      Texts["deadEnd"].emplace(txl::TextureLoader::Instance().GetFont(), ">Dead end", 15);
      Texts["Missed"].emplace(txl::TextureLoader::Instance().GetFont(), "Missed", 18);
      Texts["Enemy Missed"].emplace(txl::TextureLoader::Instance().GetFont(), "Enemy Missed", 18);
      Texts["Bullets: "].emplace(txl::TextureLoader::Instance().GetFont(), "Ammo:", 18);    
      
      Texts["AlrCheck: "].emplace(txl::TextureLoader::Instance().GetFont(), "You already checked here.", 15);    
      Texts["Resources"].emplace(txl::TextureLoader::Instance().GetFont(), "Found some resources.", 15);  
      Texts["FoundBull"].emplace(txl::TextureLoader::Instance().GetFont(), "Found some bullets.", 15);    
      Texts["Nothing"].emplace(txl::TextureLoader::Instance().GetFont(), "There's nothing here.", 15);    
    
      Texts["RescInv"].emplace(txl::TextureLoader::Instance().GetFont(), "Food: Water:", 15);  
      Texts["Cheat"].emplace(txl::TextureLoader::Instance().GetFont(), "Cheater!", 25);  


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