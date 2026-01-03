#ifndef ANIMATION_HPP
#define ANIMATION_HPP

struct Animation {
    const int numberOfFrames;   // Total Length.
    int currentFrame;           // Starts on 0.
    float frameTime;            // Time between frames
    float timer;                // Timer for control

    // Constructor
    Animation(const int numberOfFrames, float frameTime) : numberOfFrames(numberOfFrames), frameTime(frameTime) {};

    // Just updates the frame
    inline void update(float deltatime) {
        timer += deltatime;
        if (timer >= frameTime) {
            timer = 0.0f;
            currentFrame++;
            if (currentFrame >= numberOfFrames) {
                currentFrame = 0;
            }
        }
    }
};

#endif