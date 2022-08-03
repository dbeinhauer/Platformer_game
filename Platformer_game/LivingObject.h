#ifndef LIVINGOBJECT_H_
#define LIVINGOBJECT_H_


#include "AbstractObject.h"
#include "Obstacle.h"


class LivingObject
{
public:
	// Relative object borders in char level map.
	int leftBorderRel_ = 0;
	int rightBorderRel_ = 0;
	int topBorderRel_ = 0;
	int bottomBorderRel_ = 0;

	AbstractObject absObject_;

	// Init functions:
	LivingObject();
	void initRelativePos(int row, int column);

	// Moving object functions:
	void moveLeft(float speed, bool changeRel);
	void moveRight(float speed, bool changeRel);
	void jump(float speed, bool changeRel);
	void fall(float speed, bool changeRel);

	// Checking collistion:
	bool isCollidingWithObject(const AbstractObject& object);
};


#endif