#include "Bullet.h"

Bullet::Bullet() {}

/// <summary>
/// Initializes the object.
/// </summary>
/// <param name="size">Size of the window object.</param>
/// <param name="speed">Speed of the bullet.</param>
Bullet::Bullet(sf::Vector2f size, sf::Vector2f speed)
{

	this->absObject_.winObject_.setSize(size);
	this->absObject_.winObject_.setFillColor(sf::Color::Magenta);

	this->absObject_.winObject_.rotate(std::atan2f(speed.y, speed.x) * 180.0f / PI);


	this->isStrong_ = false;
	this->speed_ = std::move(speed);
}

/// <summary>
/// </summary>
/// <returns>Returns `true` if bullet is strong, else `false`.</returns>
bool Bullet::isStrong()
{
	return this->isStrong_;
}

/// <summary>
/// </summary>
/// <returns>Returns the bullet speed.</returns>
const sf::Vector2f& Bullet::getSpeed()
{
	return this->speed_;
}

/// <summary>
/// Changes status to strong (can kill enemy) and sets speed backwards.
/// </summary>
void Bullet::becomeStronger()
{
	this->absObject_.winObject_.setFillColor(sf::Color::White);
	this->isStrong_ = true;

	this->speed_ = { -this->speed_.x, -this->speed_.y };
}

