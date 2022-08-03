#include "FinishLine.h"

/// <summary>
/// Initializes the object.
/// </summary>
/// <param name="size">Size of the window object.</param>
FinishLine::FinishLine(sf::Vector2f size)
{
	this->absObject_.winObject_.setSize(size);
	this->absObject_.winObject_.setFillColor(sf::Color::Cyan);
}