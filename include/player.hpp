#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "resourceManager.hpp"
#include "physicsEngine.hpp"
#include "animation.hpp"

class Player {
private:
    // Dependencies
    ResourceManager& resourceManager;
    PhysicsEngine& physicsEngine;

    // Textures and animations
    Texture2D sprite;
    int sourceX;
    int sourceY;
    Animation idleAnimation;
    Animation runLeftAnimation;
    Animation runRightAnimation;

    // Flags
    bool onGround = false; // Initializing to avoid garbage data

    // Current position
    Vector2 position;
    Vector2 positionInTiles;

    // Velocities and stuff
    Vector2 velocity;
    float jumpStrength;

    // Current pointers
    Animation* currentAnimation;

public:
    Player(ResourceManager& resourceManager, PhysicsEngine& physicsEngine, float jumpStrength);

    void jump(float jumpStrength);
    void move();

    void draw();
    void update();
};

#endif