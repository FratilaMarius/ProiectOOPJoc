#include "../Player.hpp"

Player::Player(int _hp) {
  this->hp = _hp;
}
Player::~Player() = default;
Player::Player(const Player& other) {
  *this = other;
}
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
void Player::RefillWater() {
  water = 15;
  if(food > 0) movesUntilDemise = 5;
}      
void Player::RefillFood() {
  food = 12;
  if(water > 0) movesUntilDemise = 5;
}
      
int Player::PlayerStatus() {
  if(hp < 0) return -1;
  water--;
  food--;
  if(water < 0) movesUntilDemise--;
  if(food < 0) movesUntilDemise--;
  if(food < 0) std::cout<<OutOfFood;
  if(water < 0) std::cout<<OutOfWater;
  if(food<0||water<0) std::cout<<outOfSupplies<<movesUntilDemise<<"\n";
  if(movesUntilDemise < 0) return -1;
  return 1;
}
void Player::BackPack() const {
  std::cout<<"\n"<<"Food: "<<food<<", Water: "<<water<<"\n";
}
int Player::Hp() const { 
  return hp;
}
int Player::Bullets()const {
  return bullets;
}
int Player::Water()const {
  return water;
}
int Player::Food()const {
  return food;
}

// void Player::TakeDmg(int dmg) {
//   hp -= dmg;
// }
std::ostream& operator<<(std::ostream& os,  const Player& player) {
  os << "Player("
  << "hp=" << player.Hp()
  << ", accuracy=" << player.Bullets()
  << ", bullets=" << player.Water()
  << ", water=" << player.Food()
  << ")";
  return os;
}