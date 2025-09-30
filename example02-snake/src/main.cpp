#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <ctime>
#include <window.h>

#include "common.hpp"
#include "snake_game.h"

using namespace sf;

int main()
{

    auto game_window = std::make_shared<SnakeGameWindow>("snake", Vector2u(800, 800));

    CommandQueue command_queue;
    Context context = {command_queue, game_window, nullptr};

    auto snake_game = std::make_shared<SnakeGame>(game_window, WORLD_BLOCK_SIZE);
    snake_game->run();

    return 0;
}


