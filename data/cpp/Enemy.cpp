#include "../Enemy.hpp"

Enemy::Enemy(int _type, int _hp) : type(_type), hp(_hp) {
  if(type == 0) std::cout<<"\nInvalid enemy type generated\n";
  if(type == 1) {
    damage = 100;
  }
}

void Enemy::Attack(Player &jucator, int chance) {
  int hitOrNah = RNG() % chance;
  if(hitOrNah) {
    jucator.TakeDmg(damage);
  }
}