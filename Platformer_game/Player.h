#ifndef PLAYER_H_
#define PLAYER_H_

#include <iostream>

#include "SFML_includes.h"
#include "LivingObject.h"
#include "Obstacle.h"

class Player : public LivingObject
{
public:
    // Constructors:
    Player();
    Player(sf::Vector2f&& size);

    // Functions to obtain info about object:
    bool isJumping();
    bool canJump();
    float getActualSpeed();

    // Action managing funtions:
    void startJumping(float initSpeed);
    void startFalling(float speed=0);
    void stopJumping();
    void stopFalling();

    // Gravity function:
    void updateSpeed(float gravityAccelaration);

private:
    // Player state parameters:
    bool jumping_ = false;
    bool canJump_ = false;

    // Falling speed at actual moment (if negative then jumping speed).
    float actFallingSpeed_ = 0;
};


#endif