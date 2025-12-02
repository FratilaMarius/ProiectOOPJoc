#include "../Enemy.hpp"
#include "../UI.hpp"

////////////////////////////////////////////////////////////////////////////
// Shade
Shade::Shade(sf::Texture& _texture, sf::RenderWindow &_window) : Enemy(RNG() % 20 + 15, _texture), window(_window) {

  SetDamge(RNG() % 10);
  psichDmg = RNG() % 30 + 15;
  GetSpriteAddr()->setOrigin({60, 60});
  PlayAudio(1);
}

void Shade::PositionSprite() {
  float randomizerPosX = 1.9 + (RNG() % 200) / 1000;      ////////
  float randomizerPosY = 1.9 + (RNG() % 200) / 1000;      // These give the sprite some personality

  GetSpriteAddr()->setScale({3, 3});  
  GetSpriteAddr()->setPosition({static_cast<float>(window.getSize().x/ randomizerPosX), static_cast<float>(window.getSize().y/randomizerPosY)});
}
void Shade::attack(Player& player) {
  int chanceToHit = RNG() % 100;

  if(chanceToHit > 45) {
    player.DealDamage(GetDamage());
    player.HurtAccuracy(psichDmg);
    UI::Instance().UpdateTheHp(player);
    UI::Instance().UpdateTheAcc(player);
    PlayAudio(0);
  }
}
void Shade::PlaySounds(int which) {
  if(which == 0) {
    txl::TextureLoader::Instance().GetSound("ShadeAttack.mp3");
  }
  if(which == 1) {
    txl::TextureLoader::Instance().GetSound("ShadeSpawn.mp3");
  }      
  if(which == 2) {
    txl::TextureLoader::Instance().GetSound("ShadeHit.mp3");
  }
  if(which == 3) {
    txl::TextureLoader::Instance().GetSound("ShadeDeath.mp3");
  }
}
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// EncounterManager
EncounterManager &EncounterManager::Instance() {
  static EncounterManager instance;
  return instance;
}
int EncounterManager::Fight(Enemy& enemy, Player& player, int &RenderTextMissed) {
  // the player shoots
  if(player.GetAccuracy() - (RNG() % 100)> 0 && player.GetBullets() > 0) {
    enemy.HurtEnemy(30);
    enemy.PlayAudio(2);

    player.SetBullets(player.GetBullets() - 1);
  }
  else {
    RenderTextMissed = 1;
  }
  // then the enemy has a chance to hit back
  if(enemy.GetHp() > 0)
    enemy.attack(player);

  if(player.GetAccuracy() < 10) return 1;
  if(enemy.GetHp() < 0) {
    enemy.PlayAudio(3);
    return 1;
  }
  if(player.GetHp() < 0) return -1;
  return 0;
}
std::unique_ptr<Enemy> EncounterManager::GenerateAnEnemy(sf::RenderWindow &window, Player &player) {
    // int i = RNG() % 1;
        
    if (player.GetAccuracy() > 10) {
      return std::make_unique<Shade>(txl::TextureLoader::Instance().GetEnemyTexture("Shade.png"), window); 
    }
    // if (i == 1) {}
    // if (i == 2) {}

    return NULL;
}

/// daca shade ul il lasa sub 10 hp dispare
/// deci trb facut downcasting in EncMngr pentru fighturi diferite
/// QOL la stats urile shadeului, dmg/hp