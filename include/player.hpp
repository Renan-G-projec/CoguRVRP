#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "resourceManager.hpp"
#include "animation.hpp"

class Player {
private:
    // Textures and animations
    Texture2D sprite;
    int sourceX;
    int sourceY;
    Animation idleAnimation;
    Animation runLeftAnimation;
    Animation runRightAnimation;

    // Current position
    Vector2 position;
    Vector2 positionInTiles;

    // Velocities and stuff
    Vector2 velocity;

    // Current pointers
    Animation* currentAnimation;

public:
    Player(ResourceManager& resourceManager);

    void jump(float jumpStrength);
    void move();
    
    void draw();
    void update();
};

#endif