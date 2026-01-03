#include "../Player.hpp"

Player::Player(int _hp) {
  this->hp = _hp;
}
Player::~Player() = default;
Player::Player(const Player& other) :
hp(other.hp),
accuracy(other.accuracy),
bullets(other.bullets),
lights(other.lights),
tent(other.tent),
meds(other.meds),
water(other.water),
food(other.food),
movesUntilDemise(other.movesUntilDemise)
{}
Player& Player::operator=(const Player& other) {
  if (this == &other) return *this;
  this->movesUntilDemise = other.movesUntilDemise;
  this->hp = other.hp;
  this->accuracy = other.accuracy;
  this->bullets = other.bullets;
  this->lights = other.lights;
  this->tent = other.tent;
  this->meds = other.meds;
  this->water = other.water;
  this->food = other.food;
  return *this;
}    
///////////////////////////////////////////////////////////////////////////////////////////////////
// Gameplay:  
void Player::RefillWater() {
  water = 25;
  if(food > 0) movesUntilDemise = 5;
}      
void Player::RefillFood() {
  food = 20;
  if(water > 0) movesUntilDemise = 5;
}

// Checks the status of the player: decrements his resources, and checks if he still has any left. if not, decrements movesUntilDemise and kills him if <0 (ret -1)
// if we return -1 we have died
//
int Player::PlayerStatus() {
  if(hp <= 0) return -1;
  water--;
  food--;
  if(water < 0) movesUntilDemise--;
  if(food < 0) movesUntilDemise--;
  if(food < 0) std::cout<<OutOfFood;
  if(water < 0) std::cout<<OutOfWater;
  if(food<0||water<0) std::cout<<outOfSupplies<<movesUntilDemise;
  if(movesUntilDemise < 0) return -1;
  return 1;
}
// prints out the contents of the backpack
// void Player::BackPack() const {
//   std::cout<<"Food: "<<food<<", Water: "<<water;
// }

// void Player::TakeDmg(int dmg) {
//   hp -= dmg;
// }


std::ostream& operator<<(std::ostream& os,  const Player& player) {
  os <<"\n"<< "Player("
  // << "hp=" << player.hp
  // << ", accuracy=" << player.accuracy
  // << ", bullets=" << player.bullets
  << ", food=" << player.food
  << ", water=" << player.water
  << ")";
  return os;
}