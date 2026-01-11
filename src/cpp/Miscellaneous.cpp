#include "../Miscellaneous.hpp"

Miscellaneous &Miscellaneous::Instance()
{
  static Miscellaneous instance;
  return instance;
}

int Miscellaneous::EffectsOfMoving(Player &player, const std::string &where, Labyrinth &map, bool &shouldExit, int &isFighting, int &GeneratedEvent)
{
  try
  {
    int x = map.Move(where);
    if (x == 3)
    {
      shouldExit = true;
      return 1;
    }
    if (x == 2)
      isFighting = 1;
    if (x == 4)
    {
      GeneratedEvent = 0; // we'll grab an event
    }
    else
      GeneratedEvent = -1;

    int alive = player.PlayerStatus();
    if (alive < 0)
    {
      std::cout << "\nYou died\n\n";
      shouldExit = true;
    }
  }
  catch (LabExceptionCouldntMove &exp)
  {
    std::cout << exp.what();
  }

  return 1;
}

void Miscellaneous::Trade(FightContext &context, Enemy *enemy, int &RenderOffers, int &isFighting, int &hasGeneratedEnemy, int &MadeATrader)
{
  int status = EncounterManager::Instance().Fight(context, enemy);
  if (status == 1)
  {
    context.shots = 6;
    context.selectedOffer = 0;
    RenderOffers = 0;
    isFighting = 0;
    hasGeneratedEnemy = 0;
    MadeATrader = 0;
  }
}

void Miscellaneous::Renders_WithOut_timers(sf::RenderWindow &window, Labyrinth &map, const int &isFighting, const Player &player, const int &GeneratedEvent)
{
  map.SetCurrentRoomSprScale(window);
  window.draw(map.GetCurrentRoomSprite());

  UI::Instance().PositionUI(window);

  window.draw(UI::Instance().GetMiscUIsprite("hpBar"));
  window.draw(UI::Instance().GetMiscUIsprite("hpBar_empty"));
  window.draw(UI::Instance().GetMiscUIsprite("accBar"));
  window.draw(UI::Instance().GetMiscUIsprite("accBar_empty"));

  if (!isFighting && GeneratedEvent == -1)
    for (int i = 0; i < 4; i++)
    {
      const int *tempExitArray = map.FigureWhatUItoRender();
      if (tempExitArray[0] == 1)
        window.draw(UI::Instance().GetUI_Sprites(0));
      else
        window.draw(UI::Instance().GetEmptyUI_Sprites(0));
      if (tempExitArray[1] == 1)
        window.draw(UI::Instance().GetUI_Sprites(1));
      else
        window.draw(UI::Instance().GetEmptyUI_Sprites(1));
      if (tempExitArray[2] == 1)
        window.draw(UI::Instance().GetUI_Sprites(2));
      else
        window.draw(UI::Instance().GetEmptyUI_Sprites(2));
      if (tempExitArray[3] == 1)
        window.draw(UI::Instance().GetUI_Sprites(3));
      else
        window.draw(UI::Instance().GetEmptyUI_Sprites(3));
    }

  UI::Instance().UpdateTheHp(player);
  UI::Instance().UpdateTheAcc(player);
  window.draw(UI::Instance().GetText(9));
  window.draw(UI::Instance().GetText(10));

  UI::Instance().UpdateTheBullets(player);
  window.draw(UI::Instance().GetText(3));

  UI::Instance().UpdateTheInv(player);
  window.draw(UI::Instance().GetText(8));
}
