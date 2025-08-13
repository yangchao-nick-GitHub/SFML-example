#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <windows.h>

#include "window.h"
#include "game.h"

using namespace sf;

int main()
{
	auto window = std::make_shared<GameWindow>("test", Vector2u(800, 600));
    Game game(window);

    while (window->isOpen())
    {
        window->update();

        game.update();
    	game.render();
        sf::sleep(sf::milliseconds(10));
        game.clockRestart(); // 控制帧率

    }

    
    return 0;
}
