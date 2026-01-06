#ifndef MAP_HPP
#define MAP_HPP

#include "resourceManager.hpp"

enum areaIndex {
    OVERWORLD,
    STONE_CAVE,
    NEON_CAVE
};

struct Map {
private:
    ResourceManager& resourceManager;

    Texture2D tileset; 
    Texture2D background;
    areaIndex index; // Defines the tileset and background.

public:
    int mapArray[20][32]; // mapArray is public because player.cpp uses them for check collisions.
    
    Map(ResourceManager& resourceManager, areaIndex areaIndex, int mapData[20][32]);

    void draw();
};

#endif