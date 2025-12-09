#include "../Enemy.hpp"
#include "../Labyrinth.hpp"

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
int Shade::attack(Player& player) {
  int chanceToHit = RNG() % 100;

  if(chanceToHit > 45) {
    player.DealDamage(GetDamage());
    player.HurtAccuracy(psichDmg);
    PlayAudio(0);
    return 1;
  }
  return 0;
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
  SetDamge(60 + RNG() % 20);
  GetSpriteAddr()->setOrigin({125, 125});
}
void Minotaur::PositionSprite() {
  GetSpriteAddr()->setScale({2.3f, 2.3f});  
  GetSpriteAddr()->setPosition({static_cast<float>(window.getSize().x/ 2), static_cast<float>(window.getSize().y/ 2)});
};

int Minotaur::attack(Player& player) {
  int chanceToHit = RNG() % 100;

  if(chanceToHit > 35) {
    player.DealDamage(GetDamage());
    PlayAudio(0);
    return 1;
  }
  return 0;
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
}

int Blob::attack(Player& player) {
  int chanceToHit = RNG() % 100;

  if(chanceToHit > 70) {
    player.StealResources(RNG() % 3, RNG() % 3, RNG() % 2);
    PlayAudio(0);
    return 1;
  }
  return 0;
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
// Trader:
Trader::Trader(sf::Texture& _texture, sf::RenderWindow &_window) : Enemy( 1, _texture) , window(_window) {
  SetDamge(0);
  GetSpriteAddr()->setScale({3, 3});
  GetSpriteAddr()->setOrigin({ 100, 100});

  offer1T.emplace(txl::TextureLoader::Instance().GetFont(), "Offer1", 20);
  offer2T.emplace(txl::TextureLoader::Instance().GetFont(), "Offer2", 20);
}

void Trader::PositionSprite() {
  GetSpriteAddr()->setPosition({static_cast<float>(window.getSize().x/ 2), static_cast<float>(window.getSize().y/ 2)});
  offer1T->setPosition({static_cast<float>(window.getSize().x / 2) - 200, static_cast<float>(window.getSize().y / 2)});
  offer2T->setPosition({static_cast<float>(window.getSize().x / 2) - 200, static_cast<float>(window.getSize().y / 2 + 40)}); 
} 

int Trader::attack(Player& player) {
  a = RNG() % 3;
  if(a == 0) {
    b = RNG() % 10 + 3, c = RNG() % 8 + 8;
    offer1 = "Trade " +std::to_string(b)+ " hp for " +std::to_string(c)+ " bullets? (Q)";
    d = RNG() % 20 + 5, e = RNG() % 45 + 25;
    offer2 = "Trade " +std::to_string(d)+ " food for " +std::to_string(e)+ " hp? (E)";
 }
  if(a == 1) {
    b = RNG() % 20 + 20, c = RNG() % 20 + 10; // c will be the amount to get closer
    offer1 = "Trade " +std::to_string(b)+ " hp to get closer to the exit? (Q)";
    d = player.GetWater() - 10, e = RNG() % 45 +25;
    offer2 = "Trade " +std::to_string(d)+ " water for " +std::to_string(e)+ " hp? (E)";
  }
  if(a == 2) {
    b = RNG() % 15 + 4, c = RNG() % 50 + 35;
    offer1 = "Trade " +std::to_string(b)+ " hp for " +std::to_string(c)+ " accuracy? (Q)";
    d = player.GetFood() - 10, e = RNG() % 30 + 30;
    offer2 = "Trade " +std::to_string(d)+ " food for " +std::to_string(e)+ " bullets? (E)";
  }
  offer1T->setString(offer1);
  offer2T->setString(offer2);

  return 0;
}

void Trader::PlaySounds(int which) {
  if(which == 0) {
    txl::TextureLoader::Instance().GetSound("TraderDoneDeal.mp3");
  }
  if(which == 1) {
    txl::TextureLoader::Instance().GetSound("TraderSpawn.mp3");
  }      
  if(which == 3) {
    txl::TextureLoader::Instance().GetSound("TraderDeath.mp3");
  }
}

void Trader::applyOffer(Player& player, Labyrinth& lab, int selectedOffer) {
  if(a == 0) {
    if(selectedOffer == 1) {
      player.AddHp(-b);
      player.AddBullets(c);
    }
    if(selectedOffer == 2) {
      player.AddFood(-d);
      player.AddHp(e);
    }
  }
  if(a == 1) {
    if(selectedOffer == 1) {
      player.AddHp(-b);
      lab.GetCloserToExit(c);
    }
    if(selectedOffer == 2) {
      player.AddWater(-d);
      player.AddHp(e);
    } 
  }
  if(a == 2) {
      if(selectedOffer == 1) {
      player.AddHp(-b);
      player.AddToAcc(c);
    }
    if(selectedOffer == 2) {
      player.AddFood(-d);
      player.AddBullets(e);
    }
  }
}

////////////////////////////////////////////////////////////////////////////
// EncounterManager
EncounterManager &EncounterManager::Instance() {
  static EncounterManager instance;
  return instance;
}
int EncounterManager::Fight(Enemy* enemy, Player& player, Labyrinth& lab, int &RenderTextMissed, int &EnemyRenderTextMissed, int &Shots, int selectedOffer) {
  const Shade* s = dynamic_cast<Shade*>(enemy);
    if (s != nullptr) { // shade combat
      // the player shoots
      if(player.GetBullets() > 0) {
        txl::TextureLoader::Instance().GetSound("Pistol.mp3");
        if(player.GetAccuracy() - (RNG() % 100) > 0) {
          enemy->HurtEnemy(30);
          enemy->PlayAudio(2);
        }
        else {
          RenderTextMissed = 1;
        }
        player.SetBullets(player.GetBullets() - 1);
      }
      // then the enemy has a chance to hit back
      if(enemy->GetHp() > 0)
        if(!enemy->attack(player)) EnemyRenderTextMissed = 1;

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
      if(player.GetBullets() > 0) {
        txl::TextureLoader::Instance().GetSound("Pistol.mp3");
        if(player.GetAccuracy() - (RNG() % 100)> 0) {
          enemy->HurtEnemy(30);
          enemy->PlayAudio(0);
        }
        else {
          RenderTextMissed = 1;
        }
        player.SetBullets(player.GetBullets() - 1);
      }
      // then the enemy has a chance to hit back
      if(enemy->GetHp() > 0)
        if(!enemy->attack(player)) EnemyRenderTextMissed = 1;

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
      if(player.GetBullets() > 0 && Shots > 0) {
        txl::TextureLoader::Instance().GetSound("Pistol.mp3");
        if(player.GetAccuracy() - (RNG() % 100)> 0) {
          enemy->HurtEnemy(30);
          enemy->PlayAudio(2);
        }
        else {
          RenderTextMissed = 1;
        }
        player.SetBullets(player.GetBullets() - 1);
        Shots--;
      }
      if(Shots <= 0 || player.GetBullets() <= 0)      // then the enemy has a chance to hit back
        if(enemy->GetHp() > 0) {
          if(!enemy->attack(player)) EnemyRenderTextMissed = 1;
          Shots = 6;
          return 1;
        }
      if(enemy->GetHp() < 0) {
        enemy->PlayAudio(3);
        return 1;
      }
      if(player.GetHp() < 0) return -1;

      return 0;
    }

  const Trader* t = dynamic_cast<Trader*>(enemy);
    if (t != nullptr) { // Trader combat

      if(selectedOffer == 0) return 10; // the player did nothing, we ll call again
      if(selectedOffer == 3) {
        player.AddHp(RNG() % 10 + 8);
        enemy->PlayAudio(3); // death audio
        return 1; // the player shot the trader
      }
      if(selectedOffer == 1) {
        enemy->applyOffer(player, lab, selectedOffer);
        enemy->PlayAudio(0); // done deal audio
        return 1; // offer1
      }
      if(selectedOffer == 2) {
        enemy->applyOffer(player, lab, selectedOffer);
        enemy->PlayAudio(0); // done deal audio
        return 1; // offer2
      }
    }
  return 1;
}
std::unique_ptr<Enemy> EncounterManager::GenerateAnEnemy(sf::RenderWindow &window, const Player &player) {
    int i = RNG() % 7;
    // int i = 7;
    switch(i) {
      case 1: // blob
      case 2:
        if (player.GetBullets() > 2) {
          return std::make_unique<Blob>(txl::TextureLoader::Instance().GetEnemyTexture("Blob.png"), window); 
        }
        break;
 
      case 0:
      case 3:
      case 4: //shade
        if (player.GetAccuracy() > 10) {
          return std::make_unique<Shade>(txl::TextureLoader::Instance().GetEnemyTexture("Shade.png"), window); 
        }        
        break;

      case 5: // minotaur
          return std::make_unique<Minotaur>(txl::TextureLoader::Instance().GetEnemyTexture("Minotaur.png"), window);  
        break;
      case 6:
      // case 7: // trader
          return std::make_unique<Trader>(txl::TextureLoader::Instance().GetEnemyTexture("Trader.png"), window);
        break;

      default:
        break;
    }

    return NULL;
}