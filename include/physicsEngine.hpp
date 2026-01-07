#ifndef PHYSICSENGINE_HPP
#define PHYSICSENGINE_HPP


// It is an auxiliar class to make some physics logic.
// Examples: Gravity, Colisions etc.
// It is instanced just one time for te entire game. and *must* be passed in every entities constructor.
class PhysicsEngine {
public:
    float gravity;
};

#endif