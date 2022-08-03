#include <iostream>
#include <memory>

#include "SFML_includes.h"
#include "Game.h"

int main(int argc, char** argv)
{
    //Init Game engine
    std::string levelFile = "Levels/level_1.txt";

    // Run test or other level than default
    // (filename of the level file is the first argument).
    if (argc > 1)
        levelFile = argv[1];

    std::string fontFile = "Fonts/arial.ttf";
    Game game(levelFile, fontFile, 5);


    float sleepIntervalSec = 0.01f;
    sf::Clock updateClock;
    updateClock.restart();

    //Game loop
    while (game.running())
    {
        //Update
        game.update(updateClock);

        //Render
        game.render();

        // Sleep for a while.
        sf::sleep(sf::seconds(
            sleepIntervalSec - updateClock.getElapsedTime().asSeconds()));
    }

    //End of application
    return 0;
}