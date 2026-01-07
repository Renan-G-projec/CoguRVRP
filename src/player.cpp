#include "player.hpp"

Player::Player(ResourceManager& resourceManager, PhysicsEngine& physicsEngine, float jumpStrength) : resourceManager(resourceManager), physicsEngine(physicsEngine), jumpStrength(jumpStrength), idleAnimation{1, 0.0f}, runLeftAnimation{2, 0.4f}, runRightAnimation{2, 0.4f} {
    sprite = resourceManager.getTexture("../assets/sprites/MainMush.png");
    currentAnimation = &idleAnimation;
}

void Player::jump(float jumpStrength) {
    velocity.x = -jumpStrength;
}

void Player::move() {
    // Checking for inputs
    bool right = IsKeyPressed(KEY_D);
    bool left = IsKeyPressed(KEY_A);
    bool jumpRequest = IsKeyDown(KEY_W);

    // Moving - Horizontal
    if (right) {
        // RIGHT
        velocity.x = 4;
        currentAnimation = &runRightAnimation;
    } else if (left) {
        // LEFT
        velocity.x = -4;
        currentAnimation = &runLeftAnimation;
    } else {
        // IDLE
        velocity.x = 0;
        currentAnimation = &idleAnimation;
    }

    // Moving - Vertical
    if (!onGround) {
        velocity.y += physicsEngine.gravity;
    } else {
        velocity.y = 0;
    }

    if (jumpRequest && onGround) {
        jump(jumpStrength);
    }
}

void Player::draw() {
    currentAnimation->update(GetFrameTime());

    // "Cutting" the spritesheet
    sourceX = 0; // For idle animation
    sourceY = 0; // For idle animation
    if (currentAnimation == &runRightAnimation) {
        sourceX = currentAnimation->currentFrame * 32;
        sourceY = 32;
    } else if (currentAnimation == &runLeftAnimation) {
        sourceX = 32 + (currentAnimation->currentFrame * 32);
        sourceY = 0;
    }

    Rectangle source = {sourceX, sourceY, 32, 32}; 
    Rectangle dest = {position.x, position.y, 32, 32};
    DrawTexturePro(sprite, source, dest, {0.0f, 0.0f}, 0.0f, WHITE);
}
