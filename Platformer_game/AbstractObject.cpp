#include "AbstractObject.h"

/// <summary>
/// Initializes the object.
/// </summary>
AbstractObject::AbstractObject()
{
    this->winObject_.setSize({ 40, 40 });
    this->winObject_.setFillColor(sf::Color::Blue);
}

AbstractObject::~AbstractObject(){}

/// <summary>
/// Moves the own window object (calls sf::move function).
/// </summary>
/// <param name="distance">How to move the object.</param>
void AbstractObject::move(sf::Vector2f distance)
{
    this->winObject_.move(distance);
}

/// <summary>
/// Sets position of the own window object (calls sf::setPosition function).
/// </summary>
/// <param name="newPos">New position.</param>
void AbstractObject::setPos(sf::Vector2f newPos)
{
    this->winObject_.setPosition(newPos);
}

/// <summary>
/// Draws own window object to given window (calls sf::draw function).
/// </summary>
/// <param name="window">Window to draw own object.</param>
void AbstractObject::drawTo(sf::RenderWindow& window)
{
    window.draw(this->winObject_);
}

/// <summary>
/// </summary>
/// <returns>Returns global left border of the own window object.</returns>
float AbstractObject::getLeftBorder()
{
    return this->winObject_.getGlobalBounds().left;
}

/// <summary>
/// </summary>
/// <returns>Returns global coordinate of the 
/// right border of the own window object.</returns>
float AbstractObject::getRightBorder()
{
    return this->winObject_.getGlobalBounds().left + this->winObject_.getGlobalBounds().width;
}

/// <summary>
/// </summary>
/// <returns>Returns global coordinate of the 
/// top border of the own window object.</returns>
float AbstractObject::getTopBorder()
{
    return this->winObject_.getGlobalBounds().top;
}

/// <summary>
/// </summary>
/// <returns>Returns global coordinate of the 
/// bottom border of the own window object.</returns>
float AbstractObject::getBottomBorder()
{
    return this->winObject_.getGlobalBounds().top + this->winObject_.getGlobalBounds().height;
}

/// <summary>
/// </summary>
/// <returns>Returns the x position of the own window object.</returns>
float AbstractObject::getX()
{
    return this->winObject_.getPosition().x;
}

/// <summary>
/// </summary>
/// <returns>Returns the y position of the own window object.</returns>
float AbstractObject::getY()
{
    return this->winObject_.getPosition().y;
}

