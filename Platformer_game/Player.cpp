#include "Player.h"

Player::Player() {}

/// <summary>
/// Initializes the object.
/// </summary>
/// <param name="size">Size of the window object.</param>
Player::Player(sf::Vector2f&& size)
{
    // Variable setup:
    this->jumping_ = false;
    this->canJump_ = false;
    this->actFallingSpeed_ = 0;

    // Window object setup:
    this->absObject_.winObject_.setSize(size);
    this->absObject_.winObject_.setFillColor(sf::Color::Green);
}

/// <summary>
/// </summary>
/// <returns>Returns `true` if player is jumping, else `false`.</returns>
bool Player::isJumping()
{
    return this->jumping_;
}

/// <summary>
/// </summary>
/// <returns>Returns `true` if player can jump, else `false`.</returns>
bool Player::canJump()
{
    return this->canJump_;
}

/// <summary>
/// </summary>
/// <returns>Returns actual falling speed (negative if player is jumping).</returns>
float Player::getActualSpeed()
{
    return this->actFallingSpeed_;
}


/// <summary>
/// Starts jumping (sets proper variables to manage the other actions).
/// </summary>
/// <param name="initSpeed">Initial jumping speed.</param>
void Player::startJumping(float initSpeed)
{
    this->jumping_ = true;
    this->canJump_ = false;
    // negative falling speed
    this->actFallingSpeed_ = -initSpeed;

}

/// <summary>
/// Starts falling (sets proper variables to manage the other actions).
/// </summary>
/// <param name="speed">Initial falling speed.</param>
void Player::startFalling(float speed)
{
    this->canJump_ = false;
    this->actFallingSpeed_ = speed;
}

/// <summary>
/// Stops jumping (sets proper variables to manage the other actions).
/// </summary>
void Player::stopJumping()
{
    this->jumping_ = false;
    this->actFallingSpeed_ = 0;
}

/// <summary>
/// Stops falling (sets proper variables to manage the other actions).
/// </summary>
void Player::stopFalling()
{
    this->canJump_ = true;
    this->actFallingSpeed_ = 0;
}

/// <summary>
/// Updates falling speed (when default delta time passed).
/// </summary>
/// <param name="gravityAcceleration">How much change the falling speed.</param>
void Player::updateSpeed(float gravityAcceleration)
{
    this->actFallingSpeed_ += gravityAcceleration;
}