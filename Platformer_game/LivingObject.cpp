#include "LivingObject.h"

LivingObject::LivingObject() {}

/// <summary>
/// Sets relative borders in char map representation format (from `Level` object).
/// `leftBorderRel_` is equal to row index, where the left border is located,
/// 'topBorderRel_` is equal to column index, where the top border is located,
/// rest is same as function parameters.
/// </summary>
/// <param name="row">Row relative coordinate.</param>
/// <param name="column">Column relative coordinate.</param>
void LivingObject::initRelativePos(int row, int column)
{
	this->leftBorderRel_ = row - 1;
	this->rightBorderRel_ = row;
	this->topBorderRel_ = column - 1;
	this->bottomBorderRel_ = column;
}

/// <summary>
/// Moves the object to left by given offset (`speed`), 
/// additionaly sometimes changes relative coordinates.
/// </summary>
/// <param name="speed">How much to move.</param>
/// <param name="changeRel">Should the relative coordinates change (yes - `true`, no - `false`).</param>
void LivingObject::moveLeft(float speed, bool changeRel)
{
	if (changeRel)
	// Relative coordinates change.
	{
		this->leftBorderRel_--;
		this->rightBorderRel_--;
	}

	this->absObject_.move({ -speed, 0 });
}

/// <summary>
/// Moves the object to right by given offset (`speed`), 
/// additionaly sometimes changes relative coordinates.
/// </summary>
/// <param name="speed">How much to move.</param>
/// <param name="changeRel">Should the relative coordinates change (yes - `true`, no - `false`).</param>
void LivingObject::moveRight(float speed, bool changeRel)
{
	if (changeRel)
	// Relative coordinates change.
	{
		this->leftBorderRel_++;
		this->rightBorderRel_++;
	}

	this->absObject_.move({ speed, 0 });
}

/// <summary>
/// Moves the object up by given offset (`speed`), 
/// additionaly sometimes changes relative coordinates.
/// </summary>
/// <param name="speed">How much to move.</param>
/// <param name="changeRel">Should the relative coordinates change (yes - `true`, no - `false`).</param>
void LivingObject::jump(float speed, bool changeRel)
{
	if (changeRel)
	// Relative coordinates change.
	{
		this->topBorderRel_--;
		this->bottomBorderRel_--;
	}

	this->absObject_.move({ 0, -speed });
}

/// <summary>
/// Moves the object down by given offset (`speed`), 
/// additionaly sometimes changes relative coordinates.
/// </summary>
/// <param name="speed">How much to move.</param>
/// <param name="changeRel">Should the relative coordinates change (yes - `true`, no - `false`).</param>
void LivingObject::fall(float speed, bool changeRel)
{
	if (changeRel)
	// Relative coordinates change.
	{
		this->topBorderRel_++;
		this->bottomBorderRel_++;
	}

	this->absObject_.move({ 0, speed });
}

/// <summary>
/// Checks whether window objects of the given object and `this` object collides.
/// </summary>
/// <param name="object">Object to check collision.</param>
/// <returns>Returns `true` if objects collides (intesects), else `false`.</returns>
bool LivingObject::isCollidingWithObject(const AbstractObject& object)
{
	if (this->absObject_.winObject_.getGlobalBounds().intersects(
										object.winObject_.getGlobalBounds()))
		return true;

	return false;
}