#include "../Enemy.hpp"

Enemy::Enemy(int _type, int _hp) : type(_type), hp(_hp) {
  if(type == 0) std::cout<<"\nInvalid enemy type generated\n";
  if(type == 1) {
    damage = 100;
  }
}
Enemy::~Enemy() {};
Enemy::Enemy(const Enemy& other) :
type(other.type),
hp(other.hp),
damage(other.damage),
texture(other.texture)
{}
// void Enemy::Attack(Player &jucator, int chance) {
//   int hitOrNah = RNG() % chance;
//   if(hitOrNah) {
//     jucator.TakeDmg(damage);
//   }
// }
std::ostream& operator<<(std::ostream& os,  const Enemy& enemy) {
  os << "Enemy Type: " << enemy.type << ", HP: " << enemy.hp
     << ", Damage: " << enemy.damage << ", Texture: " << enemy.texture;
  return os;
}