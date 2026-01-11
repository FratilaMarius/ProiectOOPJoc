#include "../Events.hpp"

Event::Event(Player &_player, sf::Texture &texture, sf::RenderWindow &_window, sf::Sprite &spriteBackground, Labyrinth &_map, int &_affectTheNextEnemy)
     : player(_player), window(_window), spriteBackground(spriteBackground), map(_map) , affectTheNextEnemy(_affectTheNextEnemy) {
  spriteEvent.emplace(texture);
  request.emplace(txl::TextureLoader::Instance().GetFont(), "placeholder", 20);
  conclusion.emplace(txl::TextureLoader::Instance().GetFont(), "placeholder", 20);
};


EventTent::EventTent(Player &_player, sf::Texture &_texture, sf::RenderWindow &_window, sf::Sprite &_spriteBackground, Labyrinth &_map, int &_affectTheNextEnemy)
: Event(_player, _texture, _window, _spriteBackground, _map, _affectTheNextEnemy) {
  GetRequest().setString("Take a break to rest? (Q)\nMove along (E)");
  GetConclusion().setString("You feel reinvigorated...");
  GetRequest().setPosition({static_cast<float>(_window.getSize().x / 2 - 150), static_cast<float>(_window.getSize().y / 2)});
  GetConclusion().setPosition({static_cast<float>(_window.getSize().x / 2 - 150), static_cast<float>(_window.getSize().y / 2)});
}


void EventTent::ApplyEvent() {

  PlayAnimation(); 

  GetPlayer().AddHp(hpBonus);
  GetPlayer().AddToAcc(accBonus);

  GetPlayer().AddFood(-1);
  GetPlayer().AddWater(-1);

}

void EventTent::PlayAnimation() { // 

  txl::TextureLoader::Instance().GetSound("Zipper.mp3");

  sf::Color c1 = GetSpriteBackground().getColor();
  sf::Color c2 = GetSpriteEvent().getColor();

  while(1) {
    animationTimerFade ++;

    if(animationTimerFade > 180) { // making the sprite visible again
      c1.a = 255;
      c2.a = 255;
      GetSpriteBackground().setColor(c1);
      GetSpriteEvent().setColor(c2);

      return;
    }
    else {
      int newAlpha = static_cast<int>(c1.a) - fadeSpeed;

      if (newAlpha <= 0) {
          c1.a = 0;
          c2.a = 0;
      }
      else {
        c1.a = newAlpha;
        c2.a = newAlpha;
      }
      GetSpriteBackground().setColor(c1);
      GetSpriteEvent().setColor(c2);

      GetWindow().clear();

      GetWindow().draw(GetSpriteBackground());
      GetWindow().draw(GetSpriteEvent());

      GetWindow().display();
    }
  }
}


EventHole::EventHole(Player &_player, sf::Texture &_texture, sf::RenderWindow &_window, sf::Sprite &_spriteBackground, Labyrinth &_map, int &_affectTheNextEnemy)
: Event(_player, _texture, _window, _spriteBackground, _map, _affectTheNextEnemy) {
  GetRequest().setString("You feel the wall is weaker here...\nTake a peak? (Q)\nMove along (E)");
  GetConclusion().setString("You understand your surroundings better...");
  GetRequest().setPosition({static_cast<float>(_window.getSize().x / 2 - 150), static_cast<float>(_window.getSize().y / 2)});
  GetConclusion().setPosition({static_cast<float>(_window.getSize().x / 2 - 150), static_cast<float>(_window.getSize().y / 2)});
}


void EventHole::ApplyEvent() {

  PlayAnimation(); 

  if(GetPlayer().GetHp() <= 20) GetPlayer().AddHp(GetPlayer().GetHp() - GetPlayer().GetHp() + 1);
  else GetPlayer().AddHp(hpLoss);
  GetPlayer().AddToAcc(accLoss);

  GetLab().GetCloserToExit(10);
}


void EventHole::PlayAnimation() { 

  sf::Vector2u scale = GetSpriteBackground().getTexture().getSize();
  float WinScaleX = GetWindow().getSize().x;
  float WinScaleY = GetWindow().getSize().y;
  float TextureScaleX = GetSpriteEvent().getTexture().getSize().x;
  float TextureScaleY = GetSpriteEvent().getTexture().getSize().y;

  float spriteScale = std::min(WinScaleX/TextureScaleX, WinScaleY/TextureScaleY);
  GetSpriteEvent().setScale({spriteScale, spriteScale});

  GetSpriteEvent().setOrigin({GetSpriteEvent().getLocalBounds().getCenter().x, GetSpriteEvent().getLocalBounds().getCenter().y}); // center
  GetSpriteEvent().setPosition({WinScaleX/2, WinScaleY/2});


  // display a wall with a hole looking into another room:
  // sf::Sprite wall(txl::TextureLoader::Instance().GetUITexture("Wall.png"));
  sf::Sprite window(txl::TextureLoader::Instance().GetUITexture(windowNames[RNG() % 7]));

  window.setTextureRect(sf::IntRect({0, 0}, {static_cast<int>(scale.x), static_cast<int>(scale.y)}));
  window.setScale({.2, .2});
  window.setOrigin({GetSpriteEvent().getLocalBounds().getCenter().x, GetSpriteEvent().getLocalBounds().getCenter().y}); // center
  window.setPosition({static_cast<float>(GetWindow().getSize().x/2), static_cast<float>(GetWindow().getSize().y/2)});


  // we fade out the background:
  while(1) {
    sf::Color c = GetSpriteBackground().getColor();
    if(c.a <= fadeSpeed) break;
    else c.a -= fadeSpeed;

    GetSpriteBackground().setColor(c);
    GetWindow().clear();
    GetWindow().draw(GetSpriteBackground());
    GetWindow().display();
  }

      // wait a bit:
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(100ms);

  txl::TextureLoader::Instance().GetSound("Break.mp3");

  GetWindow().clear();
  GetWindow().draw(GetSpriteEvent());
  GetWindow().draw(window);
  GetWindow().display();
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(2200ms);

  sf::RectangleShape blackCurtain(sf::Vector2f(GetWindow().getSize().x, GetWindow().getSize().y));
  blackCurtain.setFillColor(sf::Color(0, 0, 0, 0));


  while (1) {
    sf::Color c = blackCurtain.getFillColor();
          
    if (c.a >= 255 - fadeSpeed) {
      sf::Color cB = GetSpriteBackground().getColor();
      cB.a = 255;
      GetSpriteBackground().setColor(cB);
      return; 
    } 
    else {
      c.a += fadeSpeed;
    }
    blackCurtain.setFillColor(c);

    GetWindow().clear();
          
    GetWindow().draw(GetSpriteEvent());
    GetWindow().draw(window);
    GetWindow().draw(blackCurtain);
    GetWindow().display();
  }
}


EventPage::EventPage(Player &_player, sf::Texture &_texture, sf::RenderWindow &_window, sf::Sprite &_spriteBackground, Labyrinth &_map, int &_affectTheNextEnemy)
: Event(_player, _texture, _window, _spriteBackground, _map, _affectTheNextEnemy) {
  GetRequest().setString("You feel a sense of dread overwhelm you...\nBurn a page of a book to light up the room? (Q)\nMove along (E)");
  GetConclusion().setString("You feel safer...");
  GetRequest().setPosition({static_cast<float>(_window.getSize().x / 2 - 150), static_cast<float>(_window.getSize().y / 2)});
  GetConclusion().setPosition({static_cast<float>(_window.getSize().x / 2 - 150), static_cast<float>(_window.getSize().y / 2)});
}

void EventPage::ApplyEvent() {

  PlayAnimation(); 

  GetPlayer().AddToAcc(accBonus);
  SetAffectTheNextEnemy();
}

void EventPage::PlayAnimation() { // 

    txl::TextureLoader::Instance().GetSound("Burning.mp3");

    GetWindow().clear();
    GetSpriteEvent().setPosition({static_cast<float>(GetWindow().getSize().x / 2),static_cast<float>(GetWindow().getSize().y / 2)});

    GetWindow().draw(GetLab().GetCurrentRoomSprite());
    GetWindow().draw(GetSpriteEvent());
    GetWindow().display();
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(500ms);

    for(int i = 1; i < 4; i++) {          
      std::string s = "PageBurning" + std::to_string(i) + ".png";
      GetWindow().clear();
      GetSpriteEvent().setTexture(txl::TextureLoader::Instance().GetUITexture(s));
      GetWindow().draw(GetLab().GetCurrentRoomSprite());
      GetWindow().draw(GetSpriteEvent());
      GetWindow().display();
          
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(500ms);
    }

  return;
}



EventGenerator& EventGenerator::Instance() {
  static EventGenerator eventGenerator;
  return eventGenerator;
} 

std::unique_ptr<Event> EventGenerator::GenerateEvent(int &GeneratedEvent, Player &_player, sf::RenderWindow &_window, sf::Sprite &spriteBackground, Labyrinth &_map, int &_affectTheNextEnemy) {
  int i = RNG() % 3;
  // int i = 1;
  if(i == 0) {
    GeneratedEvent = 1;
    return std::make_unique<EventTent>(_player, txl::TextureLoader::Instance().GetUITexture("Tent.png"), _window, spriteBackground, _map, _affectTheNextEnemy);
  }
  if(i == 1) {
    GeneratedEvent = 2;
    return std::make_unique<EventHole>(_player, txl::TextureLoader::Instance().GetUITexture("BackGround.png"), _window, spriteBackground, _map, _affectTheNextEnemy);
  }
  if(i == 2) {
    GeneratedEvent = 3;
    return std::make_unique<EventPage>(_player, txl::TextureLoader::Instance().GetUITexture("Page.png"), _window, spriteBackground, _map, _affectTheNextEnemy);
  }

  return NULL;
}