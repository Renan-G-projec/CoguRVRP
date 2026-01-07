#include "map.hpp"

Map::Map(ResourceManager& resourceManager, areaIndex areaIndex, int mapData[20][32]) : resourceManager(resourceManager), index(areaIndex) {
    // Loading Textures
    switch(areaIndex) {
        case areaIndex::OVERWORLD:
            tileset = resourceManager.getTexture("../assets/1-overworld/tileset.png");
            background = resourceManager.getTexture("../assets/1-overworld/background.png");
            break;
        case areaIndex::STONE_CAVE:
            tileset = resourceManager.getTexture("../assets/1-overworld/tileset.png");
            background = resourceManager.getTexture("../assets/1-overworld/background.png");
            break;
        case areaIndex::NEON_CAVE:
            tileset = resourceManager.getTexture("../assets/1-overworld/tileset.png");
            background = resourceManager.getTexture("../assets/1-overworld/background.png");
            break;
        default:
            break;
    }

    // mapData decays to a pointer, in order to deal with that we copy the values into the struct member.
    for (int row = 0; row < 20; row++) {
        for (int column = 0; column < 32; column++) {
            this->mapArray[row][column] = mapData[row][column];
        }
    }
}

void Map::draw() {
    DrawTexture(background, 0, 0, WHITE);
    
    for (int row = 0; row < 20; row++) {
        for (int column = 0; column < 32; column++) {
            // The current number goes from 1 to 64, because this is the size of the tileset.
            int current = mapArray[row][column];

            if (current == 0) continue; // Air

            current--; // The math only works if the array starts on 0.

            int sourceSpriteX = (current % 8) * 32;
            int sourceSpriteY = (current / 8) * 32;

            Rectangle sourceRectangle = {sourceSpriteX, sourceSpriteY, 32, 32};
            Rectangle destinyRectangle = {column * 32, row * 32, 32, 32};

            DrawTexturePro(tileset, sourceRectangle, destinyRectangle, {0, 0}, 0.0f, WHITE);
        }
    }
};