#ifndef GAME_H_
#define GAME_H_



#include <iostream>
#include <vector>
#include <sstream>
#include <random>
#include <math.h>

#include "SFML_includes.h"

#include "Player.h"
#include "Level.h"
#include "Bullet.h"


enum class GameState
{
	STATE_START,
	STATE_GAME,
	STATE_END_MENU,
	STATE_END
};

class Game
{
public:
	Game(const std::string& levelFile, const std::string& fontFile, int lifes);

	const bool running() const;
	void update(sf::Clock& updateClock);
	void render();

private:
	// SFML variables:
	sf::RenderWindow window_;
	sf::VideoMode videoMode_;
	sf::Event event_;
	sf::View view_;

	// Text variables:
	sf::Font textFont_;

	sf::Text mainTitleText_;
	sf::Text playButtonText_;
	sf::Text exitButtonText_;
	sf::Text pointsText_;
	sf::Text lifesText_;

	// Main menu button pictures.
	sf::RectangleShape playButton_;
	sf::RectangleShape exitButton_;


	// Clocks:

	// Measures interval for changing the movement direction of the enemies.
	sf::Clock changingDirectionClock_;
	// Measures interval for next enemy strike.
	sf::Clock strikingClock_;

	// Time interval for each clock (in seconds).
	const float enemyLoopSec_;
	const float strikeLoopSec_;


	GameState gameState_;

	// Filenames necessary for the game.
	const std::string& levelFile_;
	const std::string& fontFile_;

	// Game setup:
	const float gravityAcceleration_;
	const float moveSpeed_;
	const float jumpSpeed_;
	const float enemySpeed_;
	const float maxBulletSpeed_;
	const int initLifes_;

	sf::Vector2f bulletSize_;


	// Level representation (map)
	Level level_;

	// Player object
	Player player_;

	// Game variables:
	int points_;
	int lifes_;
	// Flag if player was hit by the bullet.
	bool playerHit_;
	bool endGame_;

	// Inicialization functions:
	void initVariables(const std::string& filename, int healths);
	void initWindow(const std::string& fontFilename);
	void initText(const std::string& fontFilename);
	void initMainTitleText();
	void initButtonsText();
	void initGameText();
	void initGameTextPosition();
	void initMainMenu();

	void centerText(sf::Text& text, sf::RectangleShape& rectangle);

	// Update game functions (parts of `update`):
	void pollEvents(sf::Clock& updateClock);
	void updateMenu(sf::Clock& updateClock);
	void updateButtonPosition(sf::RectangleShape& object, sf::Text& text, float y_offset);
	void updateGame(sf::Clock& updateClock);
	void updateEndGame();
	void updateText(sf::Vector2f&& offset);

	void checkButtonClick(sf::Clock& updateClock);

	// Render window functions (parts of `render`):
	void renderMenu();
	void renderGame();

	// Functions which coordinates game objects movement and whole game logic:
	void moveEnemies(std::vector<Enemy>& allEnemies, float elapsedTime);
	void moveBullets(std::vector<Bullet>& allBullets, float elapsedTime);
	bool moveBullet(std::vector<Bullet>::iterator bullet, const sf::Vector2f& speed);

	void controlPlayerMovement(float elapsedTime);
	void controlEnemiesMovement(float elapsedTime);
	void controlBulletsMovement(float elapsedTime);

	void checkPlayerJumping(float elapsedTime);
	void checkPlayerGravity(float elapsedTime);
	void checkPlayerDeath();
	void checkCoinGain();

	void updatePlayerGravitySpeed(float elapsedTime);

	void changeEnemiesOrientation(std::vector<Enemy>& allEnemies);
	void strikeBulletEnemies(std::vector<Enemy>& allEnemies);
};


#endif