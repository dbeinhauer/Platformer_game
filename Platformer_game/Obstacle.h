#ifndef OBSTACLE_H_
#define OBSTACLE_H_

#include "AbstractObject.h"

class Obstacle
{
public:
	AbstractObject absObject_;
	Obstacle(sf::Vector2f size);
};


#endif