#include "Level.h"


// Public functions:

Level::Level() {}

/// <summary>
/// Initializes `Level` object.
/// </summary>
/// <param name="filename">Filename of the file, where level reprezentation is stored.</param>
/// <param name="player">Object reprezenting the player.</param>
Level::Level(const std::string& filename, Player& player)
{
	// Sizes setup:
	this->obstacleSize_ = {40, 40};
	this->coinSize_ = { 40, 40 };
	this->enemySize_ = { 40, 40 };

	// Object variables init:
	this->error_ = false;
	this->bottomMapBorder_ = 0;
	this->rightMapBorder_ = 0;
	this->startPlayerPosition_ = { 0, 0 };

	// Loading the map. If failed -> error
	if (!this->loadMap(filename, player))
		this->error_ = true;
}

/// <summary>
/// Reads map in format:
///			1st line :	width heigth
///			rest :		length of the line is `width`
///						number of lines is `height`
/// 
///			symbols:	`.` - free space
///						`#` - obstacle
///						`P` - player
///						`E` - enemy
///						`C` - coin
///						`F` - finish
/// </summary>
/// <param name="filename">Filename of file with map reprezentaion.</param>
/// <param name="player">Object reprezentiog the game player.</param>
/// <returns>Returns `true` if loading was succesfull, else `false`.</returns>
bool Level::loadMap(const std::string& filename, Player& player)
{
	std::ifstream file(filename);

	// Read width and height and check if data format is ok.
	if (!(file >> this->width_ >> this->height_ >> std::ws))
	{
		return false;
	}
	if (this->width_ <= 0 || this->height_ <= 0)
	{
		return false;
	}

	// Init variables:
	this->bottomMapBorder_ = this->height_ * this->obstacleSize_.y;
	this->rightMapBorder_ = this->width_ * this->obstacleSize_.x;

	// Read the map:
	if (!readMapRepresentation(file, player))
		return false;

	return true;
}

/// <summary>
/// Moves player to start position and sets proper relative coordinates.
/// </summary>
/// <param name="player">Player object to move.</param>
void Level::movePlayerToStart(Player& player)
{
	player.absObject_.setPos(this->startPlayerPosition_);
	player.initRelativePos(this->startPlayerRel_.x, this->startPlayerRel_.y);
}


/// <summary>
/// Adds `Bullet` object to proper container (for future manipulation).
/// </summary>
/// <param name="bullet">Object to be added.</param>
void Level::addBullet(Bullet&& bullet)
{
	if (bullet.isStrong())
		// Strong bullet
		this->allStrongBullets_.push_back(bullet);
	else
		// Weak bullet
		this->allWeakBullets_.push_back(bullet);
}

/// <summary>
/// Draws all the Level object on the window.
/// </summary>
/// <param name="window">Window where to draw the objects</param>
void Level::drawMap(sf::RenderWindow& window)
{
	// Obstacles
	for (auto&& obstacle : this->allObstacles_) 
	{
		obstacle.second.absObject_.drawTo(window);
	}

	// Finish positions
	for (auto&& finishPos : this->finishPositions_)
	{
		finishPos.second.absObject_.drawTo(window);
	}

	// Coins
	for (auto&& coin : this->allCoins_)
	{
		coin.second.absObject_.drawTo(window);
	}

	// Enemies
	for (auto&& enemy : this->allEnemies_)
	{
		enemy.absObject_.drawTo(window);
	}

	// Weak bullets
	for (auto&& weakBullet : this->allWeakBullets_)
	{
		weakBullet.absObject_.drawTo(window);
	}

	// Strong bullets
	for (auto&& strongBullet : this->allStrongBullets_)
	{
		strongBullet.absObject_.drawTo(window);
	}
}

/// <summary>
/// </summary>
/// <returns>Returns obstacle size.</returns>
const sf::Vector2f& Level::getObstacleSize()
{
	return this->obstacleSize_;
}

/// <summary>
/// </summary>
/// <returns>Returns const reference to all obstacle objects container.</returns>
const std::map<int, Obstacle>& Level::getAllObstacles()
{
	return this->allObstacles_;
}
 
/// <summary>
/// </summary>
/// <returns>Returns reference to all Enemy objects.</returns>
std::vector<Enemy>& Level::getAllEnemies()
{
	return this->allEnemies_;
}

/// <summary>
/// </summary>
/// <returns>Returns reference to all weak bullet objects.</returns>
std::vector<Bullet>& Level::getAllWeakBullets()
{
	return this->allWeakBullets_;
}

/// <summary>
/// </summary>
/// <returns>Returns reference to all strong bullet objects.</returns>
std::vector<Bullet>& Level::getAllStrongBullets()
{
	return this->allStrongBullets_;
}


/// <summary>
/// Tries to move `livObject` to left by the offset `speed`.
/// </summary>
/// <param name="livObject">Object to move.</param>
/// <param name="speed">Offset to move the object.</param>
/// <returns>Returns `true` if movement is possible, else `false`.</returns>
bool Level::moveLivingObjectLeft(LivingObject& livObject, float speed)
{
	// Coordinates of the border to check (of obstacles). 
	auto prevLeftBorderAbs = this->convertColumnFromRelToAbsolute(livObject.leftBorderRel_);

	if (livObject.absObject_.getLeftBorder() - speed <= prevLeftBorderAbs)
	// It is neccesary to leave actual relative coordinates
	{
		livObject.moveLeft(speed, true);
		if (this->checkObstacleCollision(livObject))
		// Movement not possible (obstacle there) -> move to init position
		{
			livObject.moveRight(speed, true);
			return false;
		}
		return true;
	}

	// Staying at same relative coordinates
	livObject.moveLeft(speed, false);
	if (this->checkObstacleCollision(livObject))
	// Movement not possible (obstacle there) -> move to init position
	{
		livObject.moveRight(speed, false);
		return false;
	}
	return true;
}

/// <summary>
/// Tries to move `livObject` to right by the offset `speed`.
/// </summary>
/// <param name="livObject">Object to move.</param>
/// <param name="speed">Offset to move the object.</param>
/// <returns>Returns `true` if movement is possible, else `false`.</returns>
bool Level::moveLivingObjectRight(LivingObject& livObject, float speed)
{
	// Coordinates of the border to check (of obstacles). 
	auto prevRightBorderAbs = this->convertColumnFromRelToAbsolute(livObject.rightBorderRel_) + this->obstacleSize_.x;
	
	if (livObject.absObject_.getRightBorder() + speed > prevRightBorderAbs)
	// It is neccesary to leave actual relative coordinates
	{
		livObject.moveRight(speed, true);
		if (this->checkObstacleCollision(livObject))
		// Movement not possible (obstacle there) -> move to init position
		{
			livObject.moveLeft(speed, true);
			return false;
		}
		return true;
	}

	// Staying at same relative coordinates.
	livObject.moveRight(speed, false);
	if (this->checkObstacleCollision(livObject))
	// Movement not possible (obstacle there) -> move to init position
	{
		livObject.moveLeft(speed, false);
		return false;
	}
	return true;
}

/// <summary>
/// Tries to move `livObject` up by the offset `speed`.
/// </summary>
/// <param name="livObject">Object to move.</param>
/// <param name="speed">Offset to move the object.</param>
/// <returns>Returns `true` if movement is possible, else `false`.</returns>
bool Level::jumpLivingObject(LivingObject& livObject, float speed)
{
	// Coordinates of the border to check (of obstacle). 
	auto prevTopBorderAbs = this->convertRowFromRelToAbsolute(livObject.topBorderRel_);

	if (livObject.absObject_.getTopBorder() - speed <= prevTopBorderAbs)
	// It is neccesary to leave actual relative coordinates
	{
		livObject.jump(speed, true);
		if (this->checkObstacleCollision(livObject))
		// Movement not possible (obstacle there) -> move to init position
		{
			livObject.fall(speed, true);
			return false;
		}
		return true;
	}

	// Staying at same relative coordinates.
	livObject.jump(speed, false);
	if (this->checkObstacleCollision(livObject))
	// Movement not possible (obstacle there) -> move to init position
	{
		livObject.fall(speed, false);
		return false;
	}
	return true;
}


/// <summary>
/// Tries to move `livObject` down by the offset `speed`.
/// </summary>
/// <param name="livObject">Object to move.</param>
/// <param name="speed">Offset to move the object.</param>
/// <returns>Returns `true` if movement is possible, else `false`.</returns>
bool Level::fallLivingObject(LivingObject& livObject, float speed)
{
	// Coordinates of the border to check (of obstacle). 
	auto prevBottomBorderAbs = this->convertRowFromRelToAbsolute(
								livObject.bottomBorderRel_) + this->obstacleSize_.y;

	if (livObject.absObject_.getBottomBorder() + speed > prevBottomBorderAbs)
	// It is neccesary to leave actual relative coordinates
	{
		livObject.fall(speed, true);
		if (this->checkObstacleCollision(livObject))
		// Movement not possible (obstacle there) -> move to init position
		{
			livObject.jump(speed, true);
			livObject.fall(prevBottomBorderAbs - livObject.absObject_.getBottomBorder(), false);
			return false;
		}
		return true;
	}

	// Staying at same relative coordinates.
	livObject.fall(speed, false);
	if (this->checkObstacleCollision(livObject))
	// Movement not possible (obstacle there) -> move to init position
	{
		livObject.jump(speed, false);
		livObject.fall(prevBottomBorderAbs - livObject.absObject_.getBottomBorder(), false);
		return false;
	}
	return true;
}


/// <summary>
/// Checks wheter `livObject` collides with some obstacle object. 
/// </summary>
/// <param name="livObject">Object to check.</param>
/// <returns>Returns `true` if object collides with obstacle object, else `false`.</returns>
bool Level::checkObstacleCollision(LivingObject& livObject)
{
	// Choose only neighbour coordinates to check the collision.
	auto bordersToCheck = this->getBordersToCheck(livObject);

	for (auto&& border : bordersToCheck)
	{
		if (this->allObstacles_.find(border) != this->allObstacles_.end())
		// On this coordinate is an obstacle.
		{
			// Check obstacle collision.
			if (livObject.isCollidingWithObject(this->allObstacles_.find(border)->second.absObject_))
				return true;
		}
	}

	return false;
}

/// <summary>
/// Checks wheter `livObject` collides with some coin object. 
/// </summary>
/// <param name="livObject">Object to check.</param>
/// <returns>Returns `true` if object collides with coin object, else `false`.</returns>
bool Level::checkCoinCollision(LivingObject& livObject)
{
	auto bordersToCheck = this->getBordersToCheck(livObject);
	for (auto&& border : bordersToCheck)
	{
		if (this->allCoins_.find(border) != this->allCoins_.end())
		// On this coordinate is a coin.
		{
			// Check coin collision.
			if (livObject.isCollidingWithObject(this->allCoins_.find(border)->second.absObject_))
			{
				this->allCoins_.erase(border);
				return true;
			}
		}
	}
	return false;
}


/// <summary>
/// Checks whether player collides with enemy. 
/// If so kills either player (not enough high) or enemy (player is high enough).
/// </summary>
/// <param name="player">Player object to check.</param>
/// <returns>Returns `true` if enemy kills the player, 
/// else `false` (enemy killed or no collision).</returns>
bool Level::checkEnemyPlayerCollistion(LivingObject& player)
{
	for (auto it = this->allEnemies_.begin(); it != this->allEnemies_.end(); it++)
	// Check all enemies for collision.
	{
		if (player.isCollidingWithObject(it->absObject_))
		// Enemy-player collision
		{
			if (player.absObject_.getBottomBorder() <=
				it->absObject_.getBottomBorder() - this->obstacleSize_.y / 2)
			// Player high enough to kill the enemy.
			{
				std::cout << "Enemy is death" << std::endl;
				this->allEnemies_.erase(it);
				return false;
			}
			else
			// Player not high enough -> player is killed
			{
				return true;
			}
		}
	}

	return false;
}

/// <summary>
/// Checks whether some enemy collides with the `bullet`. If so does proper action.
/// </summary>
/// <param name="bullet">Bullet object to check.</param>
/// <returns>Returns `true` if bullet collides with some enemy, else `false`.</returns>
bool Level::checkEnemyBulletCollision(Bullet& bullet)
{
	for (auto enemy = this->allEnemies_.begin(); enemy != this->allEnemies_.end(); enemy++)
	{
		if (bullet.isCollidingWithObject(enemy->absObject_))
		// Bullet colliding with enemy.
		{
			if (bullet.isStrong())
			// bullet is strong, then kill the enemy
			{
				this->allEnemies_.erase(enemy);
			}

			return true;
		}
	}

	return false;
}

/// <summary>
/// Checks whether player reached the finish.
/// </summary>
/// <param name="player">Player to check.</param>
/// <returns>Returns `true` if player reached the finish, else `false`.</returns>
bool Level::checkFinishCollistion(Player& player)
{
	// Search only neighbour relative coordinates.
	auto bordersToCheck = this->getBordersToCheck(player);
	for (auto&& border : bordersToCheck)
	{
		if (this->finishPositions_.find(border) != this->finishPositions_.end())
		// On searche position is finish object.
		{
			// Check if player reach the finish.
			if (player.isCollidingWithObject(this->finishPositions_.find(border)->second.absObject_))
				return true;
		}
	}

	return false;
}

/// <summary>
/// Checks whether player fell of the playground (under last relative coordinate).
/// </summary>
/// <param name="player">Player object to check.</param>
/// <returns>Returns `true` if player fell off the map (under bottom map border), else `false`</returns>
bool Level::checkPlayerFellOfMap(LivingObject& player)
{
	if (player.absObject_.getTopBorder() > this->bottomMapBorder_)
	// Player left the map.
	{
		std::cout << "Player is death" << std::endl;
		return true;
	}

	return false;
}

/// <summary>
/// Checks whether `object` left the map (outside the relative coordinates).
/// </summary>
/// <param name="object">Object to check.</param>
/// <returns>Checks whether object left game map, if so return `true`, else `false`</returns>
bool Level::checkObjectLeftMap(LivingObject& object)
{
	if (object.absObject_.getTopBorder() > this->bottomMapBorder_ ||
		object.absObject_.getBottomBorder() < 0 ||
		object.absObject_.getRightBorder() < 0 ||
		object.absObject_.getLeftBorder() > this->rightMapBorder_)
		return true;

	return false;
}

/// <summary>
/// Checks whether left part of the `livObject` is in empty space (no ground under it).
/// </summary>
/// <param name="livObject">Object to check.</param>
/// <returns>Returns `true` if to the left is no ground, else `false`.</returns>
bool Level::checkLeftEdgeFall(LivingObject& livObject)
{
	// check index oveflow
	if (livObject.leftBorderRel_ < 0 || livObject.bottomBorderRel_ >= this->height_ - 1)
		return true;

	return !(this->charMap_[livObject.bottomBorderRel_ +1 ][livObject.leftBorderRel_] == this->obstacleCell_);
}


/// <summary>
/// Checks whether right part of the `livObject` is in empty space (no ground under it).
/// </summary>
/// <param name="livObject">Object to check.</param>
/// <returns>Returns `true` if to the right is no ground, else `false`.</returns>
bool Level::checkRightEdgeFall(LivingObject& livObject)
{
	// check index oveflow
	if (livObject.rightBorderRel_ >= this->width_ || livObject.bottomBorderRel_ >= this->height_ - 1)
		return true;

	return !(this->charMap_[livObject.bottomBorderRel_ + 1][livObject.rightBorderRel_] == this->obstacleCell_);
}


// Private functions:

/// <summary>
/// Reads map reprezentaion from given stream and stores all neccesary info about it.
/// </summary>
/// <param name="stream">Stream to read map from.</param>
/// <param name="player">Player object from the game.</param>
/// <returns>Returns `true` if reading was succesful, else `false`.</returns>
bool Level::readMapRepresentation(std::ifstream& stream, Player& player)
{
	std::string line;
	for (int i = 0; i < this->height_; i++)
	// Read rows
	{
		// Bad inputs -> error
		if (!(std::getline(stream, line))) return false;
		if ((int)line.size() < this->width_) return false;

		auto row = std::vector<char>(this->width_);
		fill(row.begin(), row.end(), this->emptyCell_);
		this->charMap_.push_back(std::move(row));

		for (int j = 0; j < this->width_; j++)
		// Check each collumn in row.
		{
			char actCell = line[j];

			// Symbol not known.
			if (actCell != this->emptyCell_ && actCell != this->obstacleCell_ &&
				actCell != this->playerCell_ && actCell != this->coinCell_ &&
				actCell != this->enemyCell_ && actCell != this->finishCell_)
				return false;

			if (actCell == this->obstacleCell_)
			// Obstacle -> add proper reprezentation
			{
				this->charMap_[i][j] = this->obstacleCell_;
				auto actObstacle = Obstacle(this->obstacleSize_);
				actObstacle.absObject_.setPos(this->convertRelToAbsoluteCoord(i, j));

				this->allObstacles_.insert(std::make_pair(
						this->convertCoordinatesToInt(i, j), std::move(actObstacle)));
			}

			else if (actCell == this->playerCell_)
			// Player -> set position of the player and init relative positon
			{
				this->startPlayerPosition_ = this->convertRelToAbsoluteCoord(i, j);
				this->startPlayerRel_ = { j, i };
				this->movePlayerToStart(player);
			}

			else if (actCell == this->enemyCell_)
			// Enemy -> set position and init
			{
				this->charMap_[i][j] = this->enemyCell_;
				auto actEnemy = Enemy(this->enemySize_);
				actEnemy.absObject_.setPos(this->convertRelToAbsoluteCoord(i, j));
				actEnemy.initRelativePos(j, i);
				this->allEnemies_.push_back(std::move(actEnemy));
			}

			else if (actCell == this->coinCell_)
			// Coin set position and init
			{
				this->charMap_[i][j] = this->coinCell_;
				auto actCoin = Coin(this->coinSize_);
				actCoin.absObject_.setPos(this->convertRelToAbsoluteCoord(i, j));
				this->allCoins_.insert(std::make_pair(
							this->convertCoordinatesToInt(i, j), std::move(actCoin)));
			}

			else if (actCell == this->finishCell_)
			// Finish set position and init
			{
				this->charMap_[i][j] = this->finishCell_;
				auto actFinish = FinishLine(this->obstacleSize_);
				actFinish.absObject_.setPos(this->convertRelToAbsoluteCoord(i, j));
				this->finishPositions_.insert(std::make_pair(
							this->convertCoordinatesToInt(i, j), actFinish));
			}

		}
	}

	return true;
}


/// <summary>
/// Converts 2D coordinate to `int` representation (for map storage).
/// Convertion algorithm:		result = row * map_width + column;
/// </summary>
/// <param name="row">Relative row coordinate.</param>
/// <param name="column">Relative column coordinate.</param>
/// <returns>Returns `int` representation of the give coordinates.</returns>
int Level::convertCoordinatesToInt(int row, int column)
{
	return row * this->width_ + column;
}


/// <summary>
/// Converts from `int` coordinate representation to 2D representation.
/// Convertion algorithm:		row = coordInt / map_width
///								column = coordInt % map_width
/// </summary>
/// <param name="coordInt">Integer representation of 2D map coordinates.</param>
/// <returns>Returns 2D coordinates { row, column }.</returns>
sf::Vector2i Level::convertIntToCoordinates(int coordInt)
{
	return { coordInt / this->width_, coordInt % this->width_ };
}

/// <summary>
/// Converts relative coordinates to absolute (in window sizes).
/// </summary>
/// <param name="x">Column coordinate to convert.</param>
/// <returns>Absolute representation of the relative column coordinate.</returns>
float Level::convertColumnFromRelToAbsolute(int x)
{
	return x * this->obstacleSize_.x;
}


/// <summary>
/// Converts relative coordinates to absolute (in window sizes).
/// </summary>
/// <param name="y">Row coordinate to convert.</param>
/// <returns>Absolute representation of the relative row coordinate.</returns>
float Level::convertRowFromRelToAbsolute(int y)
{
	return y * this->obstacleSize_.y;
}

/// <summary>
/// Converts relative coordinates to absolute (in window sizes).
/// </summary>
/// <param name="row">Row coordinate to convert.</param>
/// <param name="column">Column coordinate to convert.</param>
/// <returns>Returns 2D absolute coordinates of the given relative coordinates {column, row}.</returns>
sf::Vector2f Level::convertRelToAbsoluteCoord(int row, int column)
{
	return { this->convertColumnFromRelToAbsolute(column),
			this->convertRowFromRelToAbsolute(row) };
}


/// <summary>
/// Finds relative neighbour coordinates of the given object (for collistion detection).
/// </summary>
/// <param name="livObject">Object to check.</param>
/// <returns>Returns vector of relative neighbour coordinates in `int` form.</returns>
std::vector<int> Level::getBordersToCheck(LivingObject& livObject)
{
	// Relative coordinates of all the sides of the searched object.
	int left = livObject.leftBorderRel_;
	int right = livObject.rightBorderRel_;
	int top = livObject.topBorderRel_;
	int bottom = livObject.bottomBorderRel_;

	// Get the coordinates to check.
	std::vector<int> bordersToCheck;
	bordersToCheck.push_back(this->convertCoordinatesToInt(top, left));				// (-1, -1)
	bordersToCheck.push_back(this->convertCoordinatesToInt(top, right));			// (-1, 0)
	bordersToCheck.push_back(this->convertCoordinatesToInt(bottom, left));			// (0, -1)
	bordersToCheck.push_back(this->convertCoordinatesToInt(bottom, right));			// (0, 0)

	return bordersToCheck;
}