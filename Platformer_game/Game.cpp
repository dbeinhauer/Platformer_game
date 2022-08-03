#include "Game.h"

// Public functions:

/// <summary>
/// Creates game object and inicializes all variables.
/// </summary>
/// <param name="filename">Name of the file, where the level setup is stored.</param>
/// <param name="lifes">Initial number of player lifes.</param>
Game::Game(const std::string& levelFile, const std::string& fontFile, int lifes)
	: gravityAcceleration_(320.0f), moveSpeed_(150.0f), enemySpeed_(150.0f),
	  enemyLoopSec_(3.1f), strikeLoopSec_(1.1f), maxBulletSpeed_(500.0f), 
	  bulletSize_({ 40, 10 }), jumpSpeed_(550.0f), initLifes_(std::move(lifes)),
	  levelFile_(levelFile), fontFile_(fontFile)
{
	this->gameState_ = GameState::STATE_START;
	this->initVariables(this->levelFile_, this->initLifes_);
	this->initWindow(this->fontFile_);
	this->initMainMenu();

}


/// <summary>
/// Checks whether program is still running. 
/// </summary>
/// <returns>Returns `true` if game window is still open and 
/// player did not press exit button, else `false`.</returns>
const bool Game::running() const
{
	if (this->gameState_ == GameState::STATE_END)
		return false;
	return this->window_.isOpen();
}


/// <summary>
/// Updates the game and prepares window for the rendering.
/// </summary>
/// <param name="updateClock">Clock for measuring time interval between last update and actual time.</param>
void Game::update(sf::Clock& updateClock)
{
	if (this->gameState_ == GameState::STATE_GAME)
	// Game is running -> update the game.
	{
		this->updateGame(updateClock);
	}
	else
	// Game no running -> update Main menu.
	{
		this->updateMenu(updateClock);
	}
}


/// <summary>
/// Renders the window (based on the `gameState_`).
/// 	- clear old frame
///		- render objects
///		- display playground or main menu
/// </summary>
void Game::render()
{
	if (this->gameState_ == GameState::STATE_GAME)
		this->renderGame();
	else
		this->renderMenu();
}


// Private functions:

/// <summary>
/// Initializes game variables.
/// </summary>
/// <param name="filename">Filename of the file with level setup.</param>
/// <param name="lifes">Initial number of player lifes.</param>
void Game::initVariables(const std::string& filename, int lifes)
{
	// Game logic
	this->endGame_ = false;
	this->points_ = 0;
	this->lifes_ = std::move(lifes);
	this->playerHit_ = false;

	this->player_ = Player({ 40, 40 });

	this->level_ = Level(filename, this->player_);
	if (this->level_.error_)
	// Error happened -> end whole program
	{
		std::cout << "Level file loading error..." << std::endl;
		this->gameState_ = GameState::STATE_END;
	}

	// Start measure game intervals.
	this->changingDirectionClock_.restart();
	this->strikingClock_.restart();
}

/// <summary>
/// Initializes program window.
/// </summary>
void Game::initWindow(const std::string& fontFilename)
{
	// Window size:
	this->videoMode_.height = 800;
	this->videoMode_.width = 900;

	// Set position to the center of the screen.
	sf::Vector2i centerWindow = sf::Vector2i(
		(sf::VideoMode::getDesktopMode().width / 2) - 450,
		(sf::VideoMode::getDesktopMode().height / 2) - 480);

	// Window setup:
	this->window_.create(this->videoMode_, "Platform Game",
		sf::Style::Titlebar | sf::Style::Close);
	this->window_.setPosition(centerWindow);

	// View setup:
	this->view_.reset(sf::FloatRect(0.0f, 0.0f,
			(float)this->window_.getSize().x, (float)this->window_.getSize().y));
	this->view_.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));

	// Font init:
	this->initText(fontFilename);
}

/// <summary>
/// Initializes all used texts in the window.
/// </summary>
/// <param name="fontFilename">Filename where of file where the font is stored.</param>
void Game::initText(const std::string& fontFilename)
{
	if (!this->textFont_.loadFromFile(fontFilename))
	// Font cann't be loaded.
	{
		std::cout << "Cann't read the font file." << std::endl;
	}

	// Init the texts:
	this->initButtonsText();
	this->initGameText();
}

/// <summary>
/// Initializes the main title text (chooses title text and other setups).
/// </summary>
void Game::initMainTitleText()
{
	// Text setup:
	this->mainTitleText_.setFont(this->textFont_);
	this->mainTitleText_.setCharacterSize(80);
	this->mainTitleText_.setFillColor(sf::Color::White);
	this->mainTitleText_.setStyle(sf::Text::Bold);

	// Text content setup:
	switch (this->gameState_)
	{
		case GameState::STATE_START:
			this->mainTitleText_.setString("Platformer game");
			break;
		case GameState::STATE_END_MENU:
			this->mainTitleText_.setCharacterSize(60);
			if (this->lifes_ > 0)
				this->mainTitleText_.setString("You won!\n Score: " + std::to_string(this->points_));
			else
				this->mainTitleText_.setString("You are death!\n   Score: " + std::to_string(this->points_));
			break;
	}
}

/// <summary>
/// Initializes all texts on all the buttons.
/// </summary>
void Game::initButtonsText()
{
	// Text parameter setup:
	int charSize = 50;
	const auto& fontColor = sf::Color::White;
	const auto& fontStyle = sf::Text::Bold;


	// Play button text:
	this->playButtonText_.setFont(this->textFont_);
	this->playButtonText_.setString("Play");
	this->playButtonText_.setCharacterSize(charSize);
	this->playButtonText_.setFillColor(fontColor);
	this->playButtonText_.setStyle(fontStyle);

	// Exit button text:
	this->exitButtonText_.setFont(this->textFont_);
	this->exitButtonText_.setString("Exit");
	this->exitButtonText_.setCharacterSize(charSize);
	this->exitButtonText_.setFillColor(fontColor);
	this->exitButtonText_.setStyle(fontStyle);
}

/// <summary>
/// Initializes texts displayed durring the game (score and lifes).
/// </summary>
void Game::initGameText()
{
	// Text setup:
	int charSize = 24;
	const auto& fontColor = sf::Color::White;
	const auto& fontStyle = sf::Text::Bold;


	// Points text:
	this->pointsText_.setFont(this->textFont_);
	this->pointsText_.setString("Points: " + std::to_string(this->points_));
	this->pointsText_.setCharacterSize(charSize);
	this->pointsText_.setFillColor(fontColor);
	this->pointsText_.setStyle(fontStyle);

	// Lifes text:
	this->lifesText_.setFont(this->textFont_);
	this->lifesText_.setString("Lifes: " + std::to_string(this->lifes_));
	this->lifesText_.setCharacterSize(charSize);
	this->lifesText_.setFillColor(fontColor);
	this->lifesText_.setStyle(fontStyle);

	this->initGameTextPosition();
}

void Game::initGameTextPosition()
{
	// Position variables:
	float x_center = this->player_.absObject_.getLeftBorder();
	float x_offset = this->view_.getSize().x / 3;
	float y_coordinate = this->player_.absObject_.getTopBorder() - this->view_.getSize().y / 3;

	this->pointsText_.setPosition(x_center - x_offset, y_coordinate);
	this->lifesText_.setPosition(x_center + x_offset, y_coordinate);
}

/// <summary>
/// Initalizes main menu (mainly its positioning).
/// </summary>
void Game::initMainMenu()
{
	this->initMainTitleText();

	// Button variables:
	const sf::Vector2f& buttonSize = { 300.0f, 100.0f };
	const sf::Color& buttonColor = { 40, 154, 210 };

	// Button boundaries setup:
	this->playButton_.setSize(buttonSize);
	this->playButton_.setFillColor(buttonColor);

	this->exitButton_.setSize(buttonSize);
	this->exitButton_.setFillColor(buttonColor);
}


/// <summary>
/// Centers the `text` inside the `object`.
/// </summary>
/// <param name="text">Text to be centered.</param>
/// <param name="object">Object creating boundaries for the text.</param>
void Game::centerText(sf::Text& text, sf::RectangleShape& object)
{
	const sf::FloatRect bounds(text.getLocalBounds());

	text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	text.setPosition(object.getPosition());
}



/// <summary>
/// Manages basic pollEvents (window closed, ESC pressed and Mouse Button pressed).
/// </summary>
/// <param name="updateClock">Clock for measuring time interval between last update and actual time.</param>
void Game::pollEvents(sf::Clock& updateClock)
{
	//Event polling
	while (this->window_.pollEvent(this->event_))
	{
		switch (this->event_.type)
		{
			case sf::Event::Closed:
				// End Game
				this->window_.close();
				this->gameState_ = GameState::STATE_END;
				break;

			case sf::Event::KeyPressed:
				if (this->event_.key.code == sf::Keyboard::Escape)
				// End Game
				{
					this->window_.close();
					this->gameState_ = GameState::STATE_END;
				}
				break;

			case sf::Event::MouseButtonPressed:
				if (this->gameState_ != GameState::STATE_GAME &&
					this->event_.key.code == sf::Mouse::Left)
				// Main menu -> check if some button was pressed.
				{
					this->checkButtonClick(updateClock);
				}
				break;
		}
	}
}


/// <summary>
/// Checks poll Events (button pressed etc.) and sets the correct positioning.
/// </summary>
/// <param name="updateClock">Clock for measuring time interval between last update and actual time.</param>
void Game::updateMenu(sf::Clock& updateClock)
{
	this->pollEvents(updateClock);

	// Sets the correct view.
	this->view_.reset(sf::FloatRect(0.0f, 0.0f, 
					(float)this->window_.getSize().x, (float)this->window_.getSize().y));
	this->window_.setView(this->view_);

	// Main title position settings:
	this->mainTitleText_.setOrigin(this->mainTitleText_.getLocalBounds().width / 2,
								this->mainTitleText_.getLocalBounds().height / 2);
	this->mainTitleText_.setPosition(this->view_.getSize().x / 2,
									this->view_.getSize().y / 2 - 150.f);


	// Buttons positioning:
	this->updateButtonPosition(this->playButton_, this->playButtonText_, 0.0f);
	this->updateButtonPosition(this->exitButton_, this->exitButtonText_, 150.0f);
}

/// <summary>
/// Updates position of the button (text and picture).
/// </summary>
/// <param name="object">Button picture.</param>
/// <param name="text">Button text content.</param>
/// <param name="y_offset">Y coordinate offset from the view center.</param>
void Game::updateButtonPosition(sf::RectangleShape& object, sf::Text& text, float y_offset)
{
	object.setOrigin(object.getLocalBounds().width / 2, 
					object.getLocalBounds().height / 2);
	object.setPosition(this->view_.getSize().x / 2,
					this->view_.getSize().y / 2 + y_offset);

	this->centerText(text, object);
}


/// <summary>
/// Updates the game state and checks for the poll events (closing window etc.).
/// </summary>
/// <param name="updateClock">Clock for measuring time interval between last update and actual time.</param>
void Game::updateGame(sf::Clock& updateClock)
{
	// Check if window closed etc.
	this->pollEvents(updateClock);
	
	// Measure time interval between last update and actual time -> restart timer
	float elapsedTime = updateClock.getElapsedTime().asSeconds();
	updateClock.restart();

	// Update the player movement.
	this->controlPlayerMovement(elapsedTime);
	this->checkPlayerJumping(elapsedTime);
	this->checkPlayerGravity(elapsedTime);

	// Update the enemies and bullet movents.
	this->controlEnemiesMovement(elapsedTime);
	this->controlBulletsMovement(elapsedTime);

	this->checkPlayerDeath();

	// Player reached the finish.
	if (this->level_.checkFinishCollistion(this->player_))
		this->endGame_ = true;

	// End of the game -> do proper actions.
	if (this->endGame_ == true)
		this->updateEndGame();


	// View update -> center of the view is the player.
	this->view_.setCenter(this->player_.absObject_.getX() + this->window_.getSize().x / 16,
		this->player_.absObject_.getY() + this->window_.getSize().y / 16);
	this->window_.setView(this->view_);
}

/// <summary>
/// Does proper actions when game ends (sets new game and displays main menu).
/// </summary>
void Game::updateEndGame()
{
	// Logs for debugging
	if (this->lifes_ > 0)
		std::cout << "You won the game!" << std::endl;
	else
		std::cout << "You are death!" << std::endl;

	// Init of the new game.
	this->gameState_ = GameState::STATE_END_MENU;
	this->initMainMenu();
	this->initVariables(this->levelFile_, this->initLifes_);
	this->initWindow(this->fontFile_);
}

/// <summary>
/// Updates (points, lifes) and moves by the `offset` (to be at same position in view).
/// </summary>
/// <param name="offset">How to move game texts.</param>
void Game::updateText(sf::Vector2f&& offset)
{
	this->pointsText_.setString("Points: " + std::to_string(this->points_));
	this->lifesText_.setString("Lifes: " + std::to_string(this->lifes_));

	this->pointsText_.move(offset);
	this->lifesText_.move(offset);
}

/// <summary>
/// Checks whether some main menu button was pressed -> does proper actions.
/// </summary>
/// <param name="updateClock">Clock for measuring time interval between last update and actual time.</param>
void Game::checkButtonClick(sf::Clock& updateClock)
{
	sf::Vector2f pos = this->window_.mapPixelToCoords(sf::Mouse::getPosition(this->window_));
	if (this->playButton_.getGlobalBounds().contains(pos))
		// Play button pressed -> start new game
	{
		this->gameState_ = GameState::STATE_GAME;
		updateClock.restart();
	}

	else if (this->exitButton_.getGlobalBounds().contains(pos))
		// Exit button pressed -> end the program
		this->gameState_ = GameState::STATE_END;
}

/// <summary>
/// Renders the Main Menu (start or end of the game loop (death/win)).
/// </summary>
void Game::renderMenu()
{
	this->window_.clear();
	this->window_.draw(this->mainTitleText_);
	this->window_.draw(this->playButton_);
	this->window_.draw(this->playButtonText_);
	this->window_.draw(this->exitButton_);
	this->window_.draw(this->exitButtonText_);
	this->window_.display();
}


/// <summary>
/// Renders the playground (while game is on).
/// </summary>
void Game::renderGame()
{
	this->window_.clear();
	this->level_.drawMap(this->window_);
	this->player_.absObject_.drawTo(this->window_);
	this->window_.draw(this->pointsText_);
	this->window_.draw(this->lifesText_);
	this->window_.display();
}


/// <summary>
/// Moves with all the enemies in the actual direction and with default speed.
/// </summary>
/// <param name="allEnemies">Vector of all living enemies.</param>
/// <param name="elapsedTime">Elapsed time since the last update (for proper update).</param>
void Game::moveEnemies(std::vector<Enemy>& allEnemies, float elapsedTime)
{
	for (auto&& enemy : allEnemies)
	{
		if (!enemy.getOrientation())
		// move left
		{
			if (!this->level_.moveLivingObjectLeft(enemy, this->enemySpeed_ * elapsedTime) ||
				this->level_.checkLeftEdgeFall(enemy))
			// Obstacle or hole in the actual direction -> change direction
			{
				enemy.changeOrientation(true);
			}
		}
		else
		// move right
		{
			if (!this->level_.moveLivingObjectRight(enemy, this->enemySpeed_ * elapsedTime) ||
				this->level_.checkRightEdgeFall(enemy))
			// Obstacle or hole in the actual direction -> change direction
			{
				enemy.changeOrientation(false);
			}
		}
	}
}

/// <summary>
/// Coordinates bullets movement.
/// </summary>
/// <param name="allBullets">Vector of bullets to coordinate (weak/strong).</param>
/// <param name="elapsedTime">Elapsed time since the last update (for proper update).</param>
void Game::moveBullets(std::vector<Bullet>& allBullets, float elapsedTime)
{
	// No bullet to check.
	if (allBullets.size() <= 0)
		return;

	bool areStrong = allBullets.begin()->isStrong();
	for (auto bullet = allBullets.begin(); bullet != allBullets.end(); bullet++)
	// Check collision, if collistion -> return (we need to erase the item from container).
	{
		const auto& speed = bullet->getSpeed();

		if (!this->moveBullet(bullet, speed * elapsedTime))
		// Bullet crashed with the obstacle -> destroy it
		{
			allBullets.erase(bullet);
			return;
		}

		if (this->level_.checkObjectLeftMap(*bullet))
		// bullet is outside of the map -> destroy it
		{
			allBullets.erase(bullet);
			return;
		}
		else if (this->player_.isCollidingWithObject(bullet->absObject_))
		// Player hit -> propagate the info
		{
			this->playerHit_ = true;
			allBullets.erase(bullet);
			return;
		}
		else if (this->level_.checkEnemyBulletCollision(*bullet))
		// Enemy hit -> kill him (if strong) / revange (if weak)
		{
			if (!bullet->isStrong())
			// Weak bullet -> strike strong bullet in opposite direction
			{
				bullet->becomeStronger();
				const auto& speed = bullet->getSpeed();

				// Operations to avoid killing itself (sometimes can happen).
				if (this->moveBullet(bullet, speed * elapsedTime))
				// bullet can live first move.
				{
					if (speed.x < 0)
					// moving left
					{
						if (this->level_.moveLivingObjectLeft(*bullet, this->bulletSize_.x))
						// Move one time the bullet size -> if possible strike strong
						{
							auto bul = *bullet;
							this->level_.addBullet(std::move(bul));
							std::cout << "Striking strong bullet as revange!!!" << std::endl;
						}
					}
					else
					// moving right
					{
						if (this->level_.moveLivingObjectRight(*bullet, this->bulletSize_.x))
						// Move one time the bullet size -> if possible strike strong
						{
							auto bul = *bullet;
							this->level_.addBullet(std::move(bul));
							std::cout << "Striking strong bullet as revange!!!" << std::endl;
						}
					}
				}
			}
			else
			// Strong bullet -> enemy is killed
			{
				std::cout << "Enemy was hit by his mates!!!" << std::endl;
			}

			allBullets.erase(bullet);

			return;
		}
	}
}


/// <summary>
/// Moves with the bullet in the given direction (`speed`).
/// </summary>
/// <param name="bullet">Iterator to bullet object to move with.</param>
/// <param name="speed">Bullet offset (how to move it).</param>
/// <returns>Returns `true` if movement is possible (not obstacle in the way), else `false`.</returns>
bool Game::moveBullet(std::vector<Bullet>::iterator bullet, const sf::Vector2f& speed)
{
	if (speed.x < 0)
	// Move left
	{
		if (!this->level_.moveLivingObjectLeft(*bullet, -speed.x))
			return false;
	}
	else
	// Move right
	{
		if (!this->level_.moveLivingObjectRight(*bullet, speed.x))
			return false;
	}

	if (speed.y < 0)
	// Move up
	{
		if (!this->level_.jumpLivingObject(*bullet, -speed.y))
			return false;
	}
	else
	// Move down
	{
		if (!this->level_.fallLivingObject(*bullet, speed.y))
			return false;
	}

	return true;
}

/// <summary>
/// Checks whether movent keys was pressed -> if so does proper actions
/// </summary>
/// <param name="elapsedTime">Elapsed time since the last update (for proper update).</param>
void Game::controlPlayerMovement(float elapsedTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		// Move Left
	{
		if (this->level_.moveLivingObjectLeft(this->player_, this->moveSpeed_ * elapsedTime))
		{
			this->checkCoinGain();
			this->updateText({ -this->moveSpeed_ * elapsedTime, 0 });
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		// Move Right
	{
		if (this->level_.moveLivingObjectRight(this->player_, this->moveSpeed_ * elapsedTime))
		{
			this->checkCoinGain();
			this->updateText({ this->moveSpeed_ * elapsedTime, 0 });
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		// Start Jumping (not falling or jumping and key `Up`/`W` pressed).
	{
		if (this->player_.canJump() && !this->player_.isJumping())
		{
			if (this->level_.jumpLivingObject(this->player_, this->jumpSpeed_ * elapsedTime))
				// Jumping is possible (not obstacle in the way).
			{
				this->player_.startJumping(this->jumpSpeed_);
				this->checkCoinGain();
				this->updateText({ 0, -this->jumpSpeed_ * elapsedTime });
			}
		}
	}
}



/// <summary>
/// Controls enemies movement, checks it to not fall from the edge 
/// and randomly strikes bullets and changes movement direction.
/// </summary>
/// <param name="elapsedTime">Elapsed time since the last update (for proper update).</param>
void Game::controlEnemiesMovement(float elapsedTime)
{
	auto& allEnemies = this->level_.getAllEnemies();
	// If no enemies left, no neccesary action
	if (allEnemies.size() <= 0)
		return;

	this->moveEnemies(allEnemies, elapsedTime);

	// Random direction change.
	if (this->changingDirectionClock_.getElapsedTime().asSeconds() >= this->enemyLoopSec_)
	{
		this->changeEnemiesOrientation(allEnemies);

		this->changingDirectionClock_.restart();
		std::cout << "Direction timeout" << std::endl;
	}

	// Random striking bulltets.
	if (this->strikingClock_.getElapsedTime().asSeconds() >= this->strikeLoopSec_)
	{
		this->strikeBulletEnemies(allEnemies);

		this->strikingClock_.restart();
		std::cout << "Strike timeout" << std::endl;
	}
}

/// <summary>
/// Controls the movement of the bullets.
/// </summary>
void Game::controlBulletsMovement(float elapsedTime)
{
	auto& allStrongBullets = this->level_.getAllStrongBullets();
	this->moveBullets(allStrongBullets, elapsedTime);
	auto& allWeakBullets = this->level_.getAllWeakBullets();
	this->moveBullets(allWeakBullets, elapsedTime);
}

/// <summary>
/// Checks whether player is jumping. If so then actualizes actual player's 
/// jumping speed if time interval for gravity update lasted (- gravity acceleration).
/// </summary>
/// <param name="elapsedTime">Elapsed time since the last update (for proper update).</param>
void Game::checkPlayerJumping(float elapsedTime)
{
	if (this->player_.isJumping())
	{
		this->updatePlayerGravitySpeed(elapsedTime);

		float speed = this->player_.getActualSpeed();
		if (speed < 0)
		// Still jumping (negative falling speed).
		{
			if (this->level_.jumpLivingObject(this->player_, -speed * elapsedTime))
			// Still jumping
			{
				this->checkCoinGain();
				this->updateText({ 0, speed * elapsedTime });
				return;
			}
		}

		// End of jumping -> reset all jumping setup.
		this->player_.stopJumping();
		this->player_.startFalling(0.0f);
	}
}

/// <summary>
/// Checks whether player is falling. If so then actualizes actual player's
/// jumping speed if time interval for gravity update lasted (- gravity acceleration).
/// </summary>
/// <param name="elapsedTime">Elapsed time since the last update (for proper update).</param>
void Game::checkPlayerGravity(float elapsedTime)
{
	this->updatePlayerGravitySpeed(elapsedTime);
	if (!this->player_.isJumping())
	{
		float speed = this->player_.getActualSpeed();
		while (!this->level_.fallLivingObject(this->player_, speed * elapsedTime))
		// Repeat until player falls down (if actual falling speed is to high)
		{
			speed -= this->gravityAcceleration_ * elapsedTime;
			if (speed < this->gravityAcceleration_ * elapsedTime)
			// Player is down.
			{
				this->player_.stopFalling();
				return;
			}
		}

		// Player is still falling.
		this->checkCoinGain();
		this->updateText({ 0, speed * elapsedTime });
		this->player_.startFalling(speed);
	}
}

/// <summary>
/// Checks whether player died (killed or fell off the map). 
/// If so actualizes game state and move player to the starting position.
/// </summary>
void Game::checkPlayerDeath()
{
	if (this->level_.checkEnemyPlayerCollistion(this->player_) ||
		this->level_.checkPlayerFellOfMap(this->player_) || this->playerHit_)
	// Player died
	{
		// Actualize game status
		this->playerHit_ = false;
		this->lifes_--;

		std::cout << "Player is death" << std::endl;
		std::cout << "Remaining lifes: " << this->lifes_ << std::endl;

		this->level_.movePlayerToStart(this->player_);

		this->initGameTextPosition();


		if (this->lifes_ <= 0)
		// End of the game check.
		{
			this->endGame_ = true;
		}
	}
}


/// <summary>
/// Checks whether player collects the coin and actualizes game state.
/// </summary>
void Game::checkCoinGain()
{
	if (this->level_.checkCoinCollision(this->player_))
	{
		this->points_++;
		std::cout << "Total points: " << this->points_ << std::endl;
	}
}


/// <summary>
/// Updates player falling speed based on the interval since the last update.
/// </summary>
/// <param name="elapsedTime">Elapsed time since the last update (for proper update).</param>
void Game::updatePlayerGravitySpeed(float elapsedTime)
{
	this->player_.updateSpeed(this->gravityAcceleration_ * elapsedTime);
}


/// <summary>
/// Randomly changes the orientation of the enemies.
/// </summary>
/// <param name="allEnemies">Container of enemies for the action.</param>
void Game::changeEnemiesOrientation(std::vector<Enemy>& allEnemies)
{
	for (auto&& enemy : allEnemies)
	{
		// Generate new random orientation
		auto randOrientation = rand() % 2;
		enemy.changeOrientation(randOrientation == 1);
	}
}

/// <summary>
/// Randomly strikes bullets (from random enemy with random speed and random orientation).
/// </summary>
/// <param name="allEnemies">Container of enemies to choose who will strike.</param>
void Game::strikeBulletEnemies(std::vector<Enemy>& allEnemies)
{
	// Randomly choose the enemy.
	auto enemy = allEnemies.begin();
	int randomOffset = static_cast <int> (rand()) / (static_cast <int> (RAND_MAX / (allEnemies.size())));
	std::advance(enemy, randomOffset);

	auto bullet = enemy->strike(this->bulletSize_, this->enemySpeed_, this->maxBulletSpeed_);

	if (!enemy->getOrientation())
	// moving left -> striking left
	{
		// Move bullet outside of the enemy.
		if (this->level_.moveLivingObjectLeft(bullet, enemy->absObject_.winObject_.getSize().x))
		{
			this->level_.addBullet(std::move(bullet));
		}
	}

	else
	// moving right -> striking right
	{
		// Move bullet outside of the enemy.
		if (this->level_.moveLivingObjectRight(bullet, enemy->absObject_.winObject_.getSize().x))
		{
			this->level_.addBullet(std::move(bullet));
		}
	}
}
