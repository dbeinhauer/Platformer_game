#ifndef BULLET_H_
#define BULLET_H_

#include "LivingObject.h"

#define PI 3.14159f

class Bullet : public LivingObject
{
public:
	// Constructors:
	Bullet();
	Bullet(sf::Vector2f size, sf::Vector2f speed);

	// Obtaining info:
	bool isStrong();
	const sf::Vector2f& getSpeed();

	// Property setup:
	void becomeStronger();
private:
	bool isStrong_ = false;
	sf::Vector2f speed_;
};

#endif