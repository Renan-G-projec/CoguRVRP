#include "player.hpp"

Player::Player(ResourceManager& resourceManager) : idleAnimation{1, 0.0f}, runLeftAnimation{2, 0.4f}, runRightAnimation{2, 0.4f} {
    sprite = resourceManager.getTexture("../assets/sprites/MainMush.png");
    currentAnimation = &idleAnimation;
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
