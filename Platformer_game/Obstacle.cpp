#include "Obstacle.h"

/// <summary>
/// Initializes the object.
/// </summary>
/// <param name="size">Size of the window object.</param>
Obstacle::Obstacle(sf::Vector2f size)
{
	this->absObject_.winObject_.setSize(size);
	this->absObject_.winObject_.setFillColor(sf::Color::Red);
}
