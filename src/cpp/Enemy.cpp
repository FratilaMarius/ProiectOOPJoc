#include "../Enemy.hpp"

////////////////////////////////////////////////////////////////////////////
// Shade
Shade::Shade(sf::Texture& _texture, sf::RenderWindow &_window) : Enemy(RNG() % 20 + 15, _texture), window(_window) {

  SetDamge(RNG() % 10);
  psichDmg = RNG() % 30 + 15;
  GetSpriteAddr()->setOrigin({60, 60});
}
Shade::Shade(const Shade& other) : Enemy(other), window(other.window), psichDmg(other.psichDmg) {};
// Shade& Shade::operator=(const Shade &other) {
//   Enemy::operator=(other);
//   this->psichDmg = other.psichDmg;  
// }

void Shade::PositionSprite() {
  float randomizerPosX = 1.9 + (RNG() % 200) / 1000;      ////////
  float randomizerPosY = 1.9 + (RNG() % 200) / 1000;      // These give the sprite some personality

  GetSpriteAddr()->setScale({3.f, 3.f});  
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
// Minotaur:
Minotaur::Minotaur(sf::Texture& _texture, sf::RenderWindow &_window) : Enemy(150, _texture), window(_window) {
  SetDamge(40 + RNG() % 20);
  GetSpriteAddr()->setOrigin({125, 125});
}
void Minotaur::PositionSprite() {
  GetSpriteAddr()->setScale({2.3f, 2.3f});  
  GetSpriteAddr()->setPosition({static_cast<float>(window.getSize().x/ 2), static_cast<float>(window.getSize().y/ 2)});
};

void Minotaur::attack(Player& player) {
  int chanceToHit = RNG() % 100;

  if(chanceToHit > 49) {
    player.DealDamage(GetDamage());
    UI::Instance().UpdateTheHp(player);
    PlayAudio(0);
  }
}

void Minotaur::PlaySounds(int which) {
  if(which == 0) {
    txl::TextureLoader::Instance().GetSound("MinotaurAttack.mp3");
  }
  if(which == 1) {
    txl::TextureLoader::Instance().GetSound("MinotaurSpawn.mp3");
  }      
  if(which == 2) {
    txl::TextureLoader::Instance().GetSound("MinotaurHit.mp3");
  }
  if(which == 3) {
    txl::TextureLoader::Instance().GetSound("MinotaurDeath.mp3");
  }
}

////////////////////////////////////////////////////////////////////////////
// Blob:
  Blob::Blob(sf::Texture& _texture, sf::RenderWindow &_window) : Enemy(RNG() % 30 + 20, _texture), window(_window) {
    SetDamge(3);
    GetSpriteAddr()->setOrigin({35, 35});
  }
void Blob::PositionSprite() {
  float randomizerPosX = 1.7 + (RNG() % 600) / 1000;      ////////
  float randomizerPosY = 1.7 + (RNG() % 600) / 1000;      // These give the sprite some personality

  GetSpriteAddr()->setScale({2.5f, 2.5f});  
  GetSpriteAddr()->setPosition({static_cast<float>(window.getSize().x/ randomizerPosX), static_cast<float>(window.getSize().y/ randomizerPosY)});
};

void Blob::attack(Player& player) {
  int chanceToHit = RNG() % 100;

  if(chanceToHit > 70) {
    player.StealResources(RNG() % 3, RNG() % 3, RNG() % 2);
    PlayAudio(0);
  }
}

void Blob::PlaySounds(int which) {
  if(which == 1) {
    txl::TextureLoader::Instance().GetSound("BlobAttack.mp3");
  }
  if(which == 0) {
    txl::TextureLoader::Instance().GetSound("BlobDeath.mp3");
  }
}

////////////////////////////////////////////////////////////////////////////
// EncounterManager
EncounterManager &EncounterManager::Instance() {
  static EncounterManager instance;
  return instance;
}
int EncounterManager::Fight(Enemy* enemy, Player& player, int &RenderTextMissed, int &Shots) {
  const Shade* s = dynamic_cast<Shade*>(enemy);
    if (s != nullptr) { // shade combat
      // the player shoots
      if(player.GetAccuracy() - (RNG() % 100)> 0 && player.GetBullets() > 0) {
        enemy->HurtEnemy(30);
        enemy->PlayAudio(2);

        player.SetBullets(player.GetBullets() - 1);
      }
      else {
        RenderTextMissed = 1;
      }
      // then the enemy has a chance to hit back
      if(enemy->GetHp() > 0)
        enemy->attack(player);

      if(player.GetAccuracy() < 10) return 1;
      if(enemy->GetHp() < 0) {
        enemy->PlayAudio(3);
        return 1;
      }
      if(player.GetHp() < 0) return -1;
      return 0;
    }

  const Blob* b = dynamic_cast<Blob*>(enemy);
    if (b != nullptr) { // Blob combat
      // the player shoots
      if(player.GetAccuracy() - (RNG() % 100)> 0 && player.GetBullets() > 0) {
        enemy->HurtEnemy(30);
        enemy->PlayAudio(0);

        player.SetBullets(player.GetBullets() - 1);
      }
      else {
        RenderTextMissed = 1;
      }
      // then the enemy has a chance to hit back
      if(enemy->GetHp() > 0)
        enemy->attack(player);

      if(enemy->GetHp() < 0) {
        enemy->PlayAudio(0);
        return 1;
      }
      if(player.GetHp() < 0) return -1;
      if(player.GetBullets() < 2) return -1;

      return 0;
    }

  const Minotaur* m = dynamic_cast<Minotaur*>(enemy);
    if (m != nullptr) { // Minotaur combat
      // the player shoots 6 times
      if(player.GetAccuracy() - (RNG() % 100)> 0 && player.GetBullets() > 0 && Shots > 0) {
        enemy->HurtEnemy(30);
        enemy->PlayAudio(2);

        player.SetBullets(player.GetBullets() - 1);
      }
      else {
        RenderTextMissed = 1;
      }
      if(Shots < 0 || player.GetBullets() < 0)      // then the enemy has a chance to hit back
        if(enemy->GetHp() > 0) {
          enemy->attack(player);
          enemy->PlayAudio(0);
          Shots = 6;
        }
      if(enemy->GetHp() < 0) {
        enemy->PlayAudio(3);
        return 1;
      }
      if(player.GetHp() < 0) return -1;

      return 0;
    }
  return 1;
}
std::unique_ptr<Enemy> EncounterManager::GenerateAnEnemy(sf::RenderWindow &window, const Player &player) {
    int i = RNG() % 6;
    // int i = 1;
    switch(i) {
      case 0:
      case 1: // blob
      case 2:
        if (player.GetBullets() > 2) {
          return std::make_unique<Blob>(txl::TextureLoader::Instance().GetEnemyTexture("Blob.png"), window); 
        }
        break;

      case 3:
      case 4: //shade
        if (player.GetAccuracy() > 10) {
          return std::make_unique<Shade>(txl::TextureLoader::Instance().GetEnemyTexture("Shade.png"), window); 
        }        
        break;

      case 5: // minotaur
          return std::make_unique<Minotaur>(txl::TextureLoader::Instance().GetEnemyTexture("Minotaur.png"), window);  
        break;
      
      default:
        break;
    }

    return NULL;
}

/// daca shade ul il lasa sub 10 hp dispare
/// deci trb facut downcasting in EncMngr pentru fighturi diferite
/// QOL la stats urile shadeului, dmg/hp