#ifndef LEVEL_H_
#define LEVEL_H_


#include<iostream>
#include<string>
#include<fstream>
#include<sstream>

#include "SFML_includes.h"
#include "Obstacle.h"
#include "Player.h"
#include "Coin.h"
#include "Enemy.h"
#include "FinishLine.h"

// for reprezenting level map
using vectorChar2D = std::vector<std::vector<char>>;

class Level
{
public:
	// Flag if error during loading happened.
	bool error_ = true;

	// Relative size of the map 
	//(number of chars (objects) in each direction)):
	int height_ = 0;
	int width_ = 0;

	// Relative map reprezentation:
	vectorChar2D charMap_;

	// Intial functions and setup functions:
	Level();
	Level(const std::string& filename, Player& player);

	bool loadMap(const std::string& filename, Player& player);
	void movePlayerToStart(Player& player);
	void addBullet(Bullet&& bullet);

	// Display function:
	void drawMap(sf::RenderWindow& window);

	// Functions to return level objects:
	const sf::Vector2f& getObstacleSize();
	const std::map<int, Obstacle>& getAllObstacles();
	std::vector<Enemy>& getAllEnemies(); 
	std::vector<Bullet>& getAllWeakBullets();
	std::vector<Bullet>& getAllStrongBullets();

	// Functions for object movent:
	bool moveLivingObjectLeft(LivingObject& livObject, float speed);
	bool moveLivingObjectRight(LivingObject& livObject, float speed);
	bool jumpLivingObject(LivingObject& livObject, float speed);
	bool fallLivingObject(LivingObject& livObject, float speed);

	// Functions to check map events:
	bool checkObstacleCollision(LivingObject& livObject);
	bool checkCoinCollision(LivingObject& livObject);
	bool checkEnemyPlayerCollistion(LivingObject& player);
	bool checkEnemyBulletCollision(Bullet& bullet);
	bool checkFinishCollistion(Player& player);
	bool checkPlayerFellOfMap(LivingObject& player);
	bool checkObjectLeftMap(LivingObject& object);
	bool checkLeftEdgeFall(LivingObject& livObject);
	bool checkRightEdgeFall(LivingObject& livObject);

private:
	// Static symbols for map reprezentation:
	static const char emptyCell_ = '.';
	static const char obstacleCell_ = '#';
	static const char playerCell_ = 'P';
	static const char enemyCell_ = 'E';
	static const char coinCell_ = 'C';
	static const char finishCell_ = 'F';

	// Sizes of the level objects:
	sf::Vector2f obstacleSize_;
	sf::Vector2f coinSize_;
	sf::Vector2f enemySize_;

	// Map borders in window coordinates (top and left are 0):
	float bottomMapBorder_ = 0.0f;
	float rightMapBorder_ = 0.0f;

	// Starting position:
	sf::Vector2f startPlayerPosition_;
	sf::Vector2i startPlayerRel_;

	// Containers containing all necessary map objects:
	std::map<int, Obstacle> allObstacles_;
	std::map<int, Coin> allCoins_;
	std::vector<Enemy> allEnemies_;
	std::vector<Bullet> allWeakBullets_;
	std::vector<Bullet> allStrongBullets_;
	std::map<int, FinishLine> finishPositions_;

	// Initial functions:
	bool readMapRepresentation(std::ifstream& stream, Player& player);

	// Convertion between 2D `int` coordinates and single `int` value:
	int convertCoordinatesToInt(int row, int column);
	sf::Vector2i convertIntToCoordinates(int coordInt);

	// Convertion between relative and absolute coordinates;
	float convertColumnFromRelToAbsolute(int x);
	float convertRowFromRelToAbsolute(int y);
	sf::Vector2f convertRelToAbsoluteCoord(int row, int column);

	// Getting necessary relative coordinates:
	std::vector<int> getBordersToCheck(LivingObject& livObject);
};

#endif