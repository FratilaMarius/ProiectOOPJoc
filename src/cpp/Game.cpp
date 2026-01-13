#include "../Game.hpp"


Game::Game(int _w, int _h, int _a, int _b, sf::RenderWindow &_window) : window(_window), w(_w), h(_h), a(_a), b(_b)
{
  //////////////////////////////////////////////////////////////////////////
  // Graphics:
  /// NOTE: sync with env variable APP_WINDOW from .github/workflows/cmake.yml:31
  window.create(sf::VideoMode({1920 / 2, 1080 / 2}), "Expedition", sf::Style::Default);

  /// NOTE: mandatory use one of vsync or FPS limit (not both)
  /// This is needed so we do not burn the GPU
  // window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(60);
  // window.setKeyRepeatEnabled(false);
  ///////////////////////////////////////////////////////////////////////////
  // Building the labyrinth and player:
  Labyrinth map(w, h, a, b);
  Player player(100);

  ///////////////////////////////////////////////////////////////////////////
  // Gameplay:
  static std::unique_ptr<Enemy> _enemy = NULL;
  static std::unique_ptr<Event> _event = NULL;
  FightContext context(player, map, RenderTextMissed, EnemyRenderTextMissed, shots, selectedOffer);

  while (window.isOpen())
  {
    bool shouldExit = false, isDead = false, isWinner = false;
    if (GeneratedEvent == 0)
    { // we need to deal with an event
      _event = EventGenerator::Instance().GenerateEvent(GeneratedEvent, player, window, map.GetCurrentRoomSprite(), map, affectNextEnemy);
      renderRequest = 1;
      // event->ApplyEvent();

      // GeneratedEvent = -1;
    }
    if (isFighting && !hasGeneratedEnemy)
    {
      _enemy = EncounterManager::Instance().GenerateAnEnemy(window, player, madeATrader);
      if (_enemy == NULL)
        isFighting = 0;
      else
      {
        hasGeneratedEnemy = 1;
        _enemy->PositionSprite();
        _enemy->PlayAudio(1);

        if (affectNextEnemy)
        {

          _enemy->SetDamage(_enemy->GetDamage() / 3);
          _enemy->SetHp(30 + RNG() % 2);

          affectNextEnemy = 0;
        }

        if (madeATrader)
        { // since the interaction with the trader differs, we cehck for it
          madeATrader = 1;
          _enemy->attack(player);
          RenderOffers = 1;
        }
      }
    }

    while (const std::optional event = window.pollEvent())
    {
      if (map.CheckIfFinished())
      {
        isWinner = true;
        break;
      }
      if (event->is<sf::Event::Closed>())
      {
        window.close();
      }
      else if (const auto *resized = event->getIf<sf::Event::Resized>())
      {
        sf::FloatRect visibleArea({0.f, 0.f}, sf::Vector2f(resized->size));
        window.setView(sf::View(visibleArea));
      }
      else if (event->is<sf::Event::MouseButtonPressed>())
      {
        //////////////////////////////////////////////////////////////////////////////////
        // handling the input for fighting
        const auto *buttonPressed = event->getIf<sf::Event::MouseButtonPressed>();
        if (isFighting && hasGeneratedEnemy)
        {
          if (buttonPressed->button == sf::Mouse::Button::Left)
          {
            context.selectedOffer = 3;
            int status = EncounterManager::Instance().Fight(context, _enemy.get());
            if (status == 1)
            { // the player won
              shots = 6;
              context.selectedOffer = 0;
              isFighting = 0;
              hasGeneratedEnemy = 0;
              RenderOffers = 0;
            }
            if (status == -1)
            { // the player lost
              shots = 6;
              context.selectedOffer = 0;
              isFighting = 0;
              hasGeneratedEnemy = 0;
              RenderOffers = 0;
            }
            if(status == 2) { // player crits
              CritHit crit = CritHit(*_enemy, 20, player.GetDamage(), window);
              crit.PlayAnimationOnEnemy(_enemy->GetSprite(), map.GetCurrentRoomSprite());
            }
            if(status == 3) { // enemy crits
              CritHit crit = CritHit(player, 20, _enemy->GetDamage(), window);
              crit.PlayAnimationOnPlayer(_enemy->GetSprite(), map.GetCurrentRoomSprite());
            }
            continue;
            // else the fight continues
          }
        }
      }
      else if (event->is<sf::Event::KeyPressed>())
      {
        const auto *keyPressed = event->getIf<sf::Event::KeyPressed>();

        //////////////////////////////////////////////////////////////////////////////////
        // Input for handling the trader interaction:
        if (isFighting && hasGeneratedEnemy && madeATrader)
        {
          if (keyPressed->scancode == sf::Keyboard::Scancode::Q)
          {
            context.selectedOffer = 1;
            Miscellaneous::Instance().Trade(context, _enemy.get(), RenderOffers, isFighting, hasGeneratedEnemy, madeATrader);
            continue;
          }
          if (keyPressed->scancode == sf::Keyboard::Scancode::E)
          {
            context.selectedOffer = 2;
            Miscellaneous::Instance().Trade(context, _enemy.get(), RenderOffers, isFighting, hasGeneratedEnemy, madeATrader);
            continue;
          }
        }

        //////////////////////////////////////////////////////////////////////////////////
        // Input for handling the events:
        if (!isFighting && GeneratedEvent > 0)
        {
          if (keyPressed->scancode == sf::Keyboard::Scancode::Q)
          {
            if (GeneratedEvent == 1)
            { // tent event
              renderRequest = 0;
              renderConclusion = 1;
              _event->ApplyEvent();
              GeneratedEvent = -1;
              continue;
            }
            if (GeneratedEvent == 2)
            { // hole event
              renderRequest = 0;
              renderConclusion = 1;
              _event->ApplyEvent();
              GeneratedEvent = -1;
              continue;
            }
            if (GeneratedEvent == 3)
            { // page event
              renderRequest = 0;
              renderConclusion = 1;
              _event->ApplyEvent();
              GeneratedEvent = -1;
              continue;
            }
          }
          if (keyPressed->scancode == sf::Keyboard::Scancode::E)
          {
            GeneratedEvent = -1;
            renderRequest = 0;
            continue;
          }
        }
        //////////////////////////////////////////////////////////////////////////////////
        // handling the input for moving
        if (!isFighting && GeneratedEvent == -1)
        {

          if (keyPressed->scancode == sf::Keyboard::Scancode::Up)
            if (Miscellaneous::Instance().EffectsOfMoving(player, "up", map, isWinner, isDead, isFighting, GeneratedEvent) == 1)
            {
              continue;
            }

          if (keyPressed->scancode == sf::Keyboard::Scancode::Down)
            if (Miscellaneous::Instance().EffectsOfMoving(player, "down", map, isWinner, isDead, isFighting, GeneratedEvent) == 1)
            {
              continue;
            }

          if (keyPressed->scancode == sf::Keyboard::Scancode::Left)
            if (Miscellaneous::Instance().EffectsOfMoving(player, "left", map, isWinner, isDead, isFighting, GeneratedEvent) == 1)
            {
              continue;
            }

          if (keyPressed->scancode == sf::Keyboard::Scancode::Right)
            if (Miscellaneous::Instance().EffectsOfMoving(player, "right", map, isWinner, isDead, isFighting, GeneratedEvent) == 1)
            {
              continue;
            }
        }

        //////////////////////////////////////////////////////////////////////////////////
        // Misc. input:
        if (keyPressed->scancode == sf::Keyboard::Scancode::Num0)
        {
          shouldExit = true;
          break;
        }

        if (keyPressed->scancode == sf::Keyboard::Scancode::Num1 && !isFighting && GeneratedEvent == -1)
        {
          int c = map.CheckForItems();

          timerPickupText = 0;
          if (c == -1)
            RenderPickupText = 1; // we alr checked
          if (c == 0)
            RenderPickupText = 2; // nothing
          if (c == 1)
          {
            RenderPickupText = 3; // resources
            player.RefillWater();
            player.RefillFood();
          }
          if (c == 2)
          {
            RenderPickupText = 4; // bullets
            player.SetBullets(8);
          }
          continue;
        }

        if (keyPressed->scancode == sf::Keyboard::Scancode::Equal && !isFighting && GeneratedEvent == -1)
        {
          map.GetCloserToExit(10);
          RenderCheat = 1;
          continue;
        }

        // if (keyPressed->scancode == sf::Keyboard::Scancode::Num2 && !isFighting) { // we display the inventory
        //   player.BackPack();
        //   continue;
        // }
      }
    }

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(10ms);

    //////////////////////////////////////////////////////////////////////////////////
    // Rendering / timers:
    window.clear();
    Miscellaneous::Instance().Renders_WithOut_timers(window, map, isFighting, player, GeneratedEvent);

    if (GeneratedEvent == 1)
    {
      _event->GetSpriteEvent().setPosition({static_cast<float>(window.getSize().x / 2 - 200), static_cast<float>(window.getSize().y / 2 - 150)});
      _event->GetSpriteEvent().setScale({3, 3});
      window.draw(_event->GetSpriteEvent());
    }

    if (isFighting && hasGeneratedEnemy)
    {
      _enemy->PositionSprite();
      window.draw(_enemy->GetSprite());
    }
    if (map.GetShouldDisplayDeadEndText())
      window.draw(UI::Instance().GetText(0));
    if (RenderTextMissed)
    {
      window.draw(UI::Instance().GetText(1));
      timerPlayer++;
      if (timerPlayer >= 120)
      {
        RenderTextMissed = 0;
        timerPlayer = 0;
      }
    }

    if (renderRequest)
      window.draw(_event->GetRequest());
    if (renderConclusion > 0 && renderConclusion < 180)
    {
      window.draw(_event->GetConclusion());
      renderConclusion++;
      if (renderConclusion > 178)
        renderConclusion = 0;
    }

    if (EnemyRenderTextMissed)
    {
      window.draw(UI::Instance().GetText(2));
      timerEnemy++;
      if (timerEnemy >= 120)
      {
        EnemyRenderTextMissed = 0;
        timerEnemy = 0;
      }
    }
    if (RenderPickupText)
    {
      timerPickupText++;

      if (RenderPickupText == 1)
        window.draw(UI::Instance().GetText(4));
      if (RenderPickupText == 2)
        window.draw(UI::Instance().GetText(7));
      if (RenderPickupText == 3)
        window.draw(UI::Instance().GetText(5));
      if (RenderPickupText == 4)
        window.draw(UI::Instance().GetText(6));

      if (timerPickupText >= 120)
      {
        RenderPickupText = 0;
        timerPickupText = 0;
      }
    }
    if (RenderCheat)
    {
      timerCheat++;
      window.draw(UI::Instance().GetText(11));

      if (timerCheat >= 120)
      {
        RenderCheat = 0;
        timerCheat = 0;
      }
    }
    if (RenderOffers)
    {
      const Trader *m = dynamic_cast<Trader *>(_enemy.get());
      if (m != nullptr)
      {
        window.draw(m->GetOffer1());
        window.draw(m->GetOffer2());
      }
    }
    if (UI::Instance().GetShiftingRooms())
    {
      timerShiftingRooms++;
      window.draw(UI::Instance().GetText(12));

      if (timerShiftingRooms >= 120)
      {
        UI::Instance().SetShiftingRooms(0);
        timerShiftingRooms = 0;
      }
    }
    window.display();



    // we pass the window and the should exit bool
    // if he restarts we just get the player back here
    // if he quits we just quit and we re done
    if (isWinner) {
      ResetVar(map, player, _enemy, _event);
      // winScreen

      // for now:
      shouldExit = 1;
    }
    if (isDead) {
      ResetVar(map, player, _enemy, _event);
      // deadScreen

      // for now:
      shouldExit = 1;
    }
    if (shouldExit)
    {
      window.close();
      break;
    }
  }
}

void Game::ResetVar(Labyrinth& map, Player& player, std::unique_ptr<Enemy>& enemyPtr, std::unique_ptr<Event>& eventPtr) {

  isFighting = 0;
  hasGeneratedEnemy = 0; 
  madeATrader = 0;
  GeneratedEvent = -1;
  affectNextEnemy = 0;
  renderRequest = 0;
  renderConclusion = 0;
  timerPlayer = 0;
  timerEnemy = 0;
  timerPickupText = 0;
  timerCheat = 0;
  timerShiftingRooms = 0;
  RenderTextMissed = 0;
  EnemyRenderTextMissed = 0;
  RenderPickupText = 0;
  RenderOffers = 0;
  RenderCheat = 0;

  shots = 6;         // this is used when we fight a mminotaur
  selectedOffer = 0; // this is used for the trader interactions  

  enemyPtr = NULL;
  eventPtr = NULL;

  map.Restart();
  player.Restart();
}