#pragma once

#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <chrono>
#include <thread>

#define NR_UNIC_PICKUPS 5
#define TEXTURE_NAME_SIZE 15
  
int RNG();

std::string _Input(); // 0 (up) | 1 (down) | 2 left | 3 right