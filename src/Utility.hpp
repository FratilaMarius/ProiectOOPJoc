#pragma once

#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <chrono>
#include <thread>

#include <SFML/Graphics.hpp>

#define NR_UNIC_PICKUPS 5
  
int RNG();

const std::string _Input(); // 0 (up) | 1 (down) | 2 left | 3 right
