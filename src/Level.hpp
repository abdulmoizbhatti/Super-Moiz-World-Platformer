#pragma once
#include <vector>
#include "Platform.hpp"

class Level {
public:
    Level();
    void load(int levelNumber);
    std::vector<Platform> platforms;
}; 