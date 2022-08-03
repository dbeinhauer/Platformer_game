#ifndef ABSTRACTOBJECT_H_
#define ABSTRACTOBJECT_H_

#include <SFML/Graphics.hpp>

class AbstractObject
{
public:
	// Window object representation
	sf::RectangleShape winObject_;

	// Initial and setup functions:
	AbstractObject();
	~AbstractObject();
	
	void move(sf::Vector2f distance);
	void setPos(sf::Vector2f newPos);

	// Display function:
	void drawTo(sf::RenderWindow& window);

	// Functions to obtain borders and coordinates:
	float getLeftBorder();
	float getRightBorder();
	float getTopBorder();
	float getBottomBorder();

	float getX();
	float getY();
};


#endif 