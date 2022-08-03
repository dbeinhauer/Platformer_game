#ifndef ENEMY_H_
#define ENEMY_H_


#include "LivingObject.h"
#include "Bullet.h"

class Enemy : public LivingObject
{
public:
	// Constructors:
	Enemy();
	Enemy(sf::Vector2f size);

	// Obtain info about the object:
	bool getOrientation();

	// Setup object:
	void changeOrientation(bool newOrientation);

	// Game functions:
	Bullet strike(sf::Vector2f bulletSize, float minSpeed, float maxSpeed);

private:
	// false - left, true - right
	bool orientation_ = false;
};

#endif