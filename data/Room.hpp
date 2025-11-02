#pragma once

#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <chrono>
#include <thread>

#include "Enemy.hpp"

//////////////////////////////////////////////////////////////////////
//  Clasa Room retine informatii despre o anumita camera din labirint
//
  class Room {
    public:
//////////////////////////////////////////////////////////////////////  Constructori, destructori, copiatori:
      Room() : id(0), texture(""), hasEnemy(0), hasPlayer(0) {}
      explicit Room(int _id);                                       //  constructor daca am nevoie de id predefinit
      Room(const Room& other);
      Room& operator=(const Room& other); 
      ~Room() = default;
//////////////////////////////////////////////////////////////////////
      Enemy GenerateEnemy(int _type, int _hp);
//////////////////////////////////////////////////////////////////////  printari si setari:
      int Id() const{ return id;}
      void Id(int _id) { id =_id;}
      int HasEnemy() const{ return hasEnemy;}      
      int HasPlayer() const{ return hasPlayer;}
      void HasPlayer(int yesOrNo) { hasPlayer = yesOrNo;}
      int NrRoutes() const{ return exits[0] + exits[1] + exits[2] + exits[3];}
      int HasExit(int which) const{ return exits[which];}
      int TimesVisited() const{ return timesVisited;}
////////////////////////////////////////////////////////////////////// setari si getari pentru iesiri:
      int Exits(std::string where);           // pentru setat prin string | up, down, left, right
      int Exits(int where);                   // pentru setat prin index | 0-up,1-down,2-left,3-right
      void Exits(std::string where, int val); // ca mai sus dar pentru setat, index string
      void Exits(int where, int val);         // ca mai sus dar pentru setat, index int
//////////////////////////////////////////////////////////////////////
      void TimesVisited(int x) { timesVisited += x;} // incrementeaza numarul de vizitari
      void ResetRoom();                              // reseteaza complet o camera, fara iesiri, etc
      int FindPickup(int what);                      // cauta un pickup in camera. 0-2 nimic, 3-apa,4-mancare
      
//////////////////////////////////////////////////////////////////////  O camera la start are peste tot 1 la iesiri
    private:
      int id;
      std::string texture = "";
      int exits[4] = {1,1,1,1};       // up, down, left, right
      int hasEnemy;                   // hasEnemy si hasPlayer sunt 0 default, 1 la nevoie
      int hasPlayer;                  // 
      int timesVisited = 0;

      int checkedForPickups = 0;
  };

std::ostream& operator<<(std::ostream& os,  const Room& room);
