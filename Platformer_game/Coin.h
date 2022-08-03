#ifndef COIN_H_
#define COIN_H_

#include "AbstractObject.h"

class Coin
{
public:
	AbstractObject absObject_;

	// Constructors:
	Coin();
	Coin(sf::Vector2f size);
};

#endif