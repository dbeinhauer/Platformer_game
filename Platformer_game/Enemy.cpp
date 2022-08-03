#include "Enemy.h"

Enemy::Enemy() {}

/// <summary>
/// Initialises object.
/// </summary>
/// <param name="size">Size of the window object.</param>
Enemy::Enemy(sf::Vector2f size)
{
	this->orientation_ = false;

	this->absObject_.winObject_.setSize(size);
	this->absObject_.winObject_.setFillColor(sf::Color::Blue);
}

/// <summary>
/// </summary>
/// <returns>Returns orientation of the enemy (`false` - heading left, `true` - right).</returns>
bool Enemy::getOrientation()
{
	return this->orientation_;
}

/// <summary>
/// Changes the object orientation.
/// </summary>
/// <param name="newOrientation">New orientation to be set.</param>
void Enemy::changeOrientation(bool newOrientation)
{
	this->orientation_ = newOrientation;
}


/// <summary>
/// Randomly strikes (creates bullet object placed on the same position as enemy object is).
/// </summary>
/// <param name="bulletSize">Window object size.</param>
/// <param name="minSpeed">Minimal horizontal speed 
/// (should be greather or equal than enemy speed to prevent killing itself).</param>
/// <param name="maxSpeed">Maximal speed of the bullet (in any direction).</param>
/// <returns>Returns randomly generated	`Bullet` object.</returns>
Bullet Enemy::strike(sf::Vector2f bulletSize, float minSpeed, float maxSpeed)
{
	// Random horizontal speed generation:
	float randomSpeedX = minSpeed + static_cast <float> (rand()) / 
						(static_cast <float> (RAND_MAX / (maxSpeed - minSpeed)));
	
	float randomSpeedY = 0;
	// 50:50 if bullet should have also vertical speed
	if (rand() % 2 == 1)
		randomSpeedY = -maxSpeed/2 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxSpeed)));
	
	Bullet bullet;
	// bullet always goes the way the enemy is moving (to prevent collisions).
	if (!this->orientation_)
	// moving left
	{
		bullet = Bullet(std::move(bulletSize), { -randomSpeedX, randomSpeedY });
	}
	else
	// moving right
	{
		bullet = Bullet(std::move(bulletSize), { randomSpeedX, randomSpeedY });
	}

	bullet.initRelativePos(this->rightBorderRel_, this->bottomBorderRel_);
	bullet.absObject_.setPos({ this->absObject_.getLeftBorder(),
			this->absObject_.getTopBorder() + this->absObject_.winObject_.getSize().y / 2 });

	return bullet;
}


