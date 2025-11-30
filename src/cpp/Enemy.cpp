#include "../Enemy.hpp"

////////////////////////////////////////////////////////////////////////////
// Shade
Shade::Shade(sf::Texture& _texture, sf::RenderWindow &_window) : Enemy(RNG() % 20 + 13, _texture), window(_window) {
  SetDamge(RNG() % 15);
  psichDmg = RNG() % 30 + 12;
  PositionSprite();
}

void Shade::PositionSprite() {
  GetSpriteAddr()->setPosition({static_cast<float>(window.getSize().x/2), static_cast<float>(window.getSize().y/2)});
}
void Shade::attack(Player* player) {
  int chanceToHit = RNG() % 100;

  if(chanceToHit > 45) {
    player->DealDamage(GetDamage());
    player->HurtAccuracy(psichDmg);
  }
}
void Shade::PlaySounds(int which) {
  if(which == 0) {
    // attack sound, le pun asa deocamdata pana imi fac in manager
  }
  if(which == 1) {
    // spawn sound
  }      
  if(which == 2) {
    // death sound
  }
  if(which == 0) {
    // damaged sound
  }
}
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// EncounterManager
EncounterManager &EncounterManager::Instance() {
  static EncounterManager instance;
  return instance;
}
int EncounterManager::Fight(Enemy *enemy, Player* player) {
  // the player shoots
  if(player->GetAccuracy() - (RNG() % 100) > 0) {
    enemy->HurtEnemy(30);
  }
  // then the enemy has a chance to hit back
  enemy->attack(player);

  if(enemy->GetHp() < 0) return 1;
  if(player->GetHp() < 0) return -1;
  return 0;
}
std::unique_ptr<Enemy> EncounterManager::GenerateAnEnemy(sf::RenderWindow &window) {
    int i = RNG() % 1;
        
    if (i == 0) {
      return std::make_unique<Shade>(txl::TextureLoader::Instance().GetEnemyTexture("Shade.png"), window); 
    }
    if (i == 1) {}
    if (i == 2) {}

    return NULL;
}