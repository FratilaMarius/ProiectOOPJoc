#include "../Utility.hpp"

int RNG() {
  return rand();
}

const std::string _Input() {
  std::string a; 
  std::cout<<"\nDirection: ";
  std::cin>>a;
  return a;
}