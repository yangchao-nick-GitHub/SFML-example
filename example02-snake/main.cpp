#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <ctime>
#include <windows.h>

#include "window.h"
#include "snake_game.h"
#include "input_manager.h"

using namespace sf;

class Entity {
public:
    void update() {};
};

int main()
{
    // auto game_window = std::make_shared<SnakeGameWindow>("snake", Vector2u(800, 800));

    // /* 网格宽是10个像素 */
    // auto snake_game = std::make_shared<SnakeGame>(game_window, WORLD_BLOCK_SIZE);
    // snake_game->run();



    InputManager input;
    CommandQueue commands;

    input.bindAction("MoveLeft", InputType::Keyboard, sf::Keyboard::A, [] {
        return std::make_unique<MoveLeftCommand>();
    });
    input.bindAction("Shoot", InputType::MouseButton, sf::Mouse::Left, [] {
        return std::make_unique<FireCommand>();
    });

    sf::RenderWindow window(sf::VideoMode(800, 600), "Command Pattern");

    Entity player; // 假设有一个玩家实体

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            input.processEvent(event, commands);
        }

        // 消费命令队列
        while (!commands.isEmpty()) {
            auto cmd = commands.pop();
            cmd->execute(player);
        }

        input.update();
    }


    return 0;
}


