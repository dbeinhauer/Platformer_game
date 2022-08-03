#include "Coin.h"

Coin::Coin() {}

/// <summary>
/// Initializes the object.
/// </summary>
/// <param name="size">Size of the window object.</param>
Coin::Coin(sf::Vector2f size)
{
	//this->absObject_.
	this->absObject_.winObject_.setSize(size);
	this->absObject_.winObject_.setFillColor(sf::Color::Yellow);
}