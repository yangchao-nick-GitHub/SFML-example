#include <SFML/Graphics.hpp>
#include <iostream>
#include "window.h"
#include "snake_game.h"

int main()
{
    GameWindow window("snake", Vector2u(800, 800));
    StateStack state_stack;
    SnakeContext snake_context({state_stack, window});

    state_stack.registerState<SnakeMenuState>(StateID::Menu, snake_context);
    state_stack.registerState<SnakeGameState>(StateID::Game, snake_context);
    state_stack.registerState<SnakePauseState>(StateID::Pause, snake_context);


    state_stack.pushState(StateID::Menu);


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            state_stack.handleInput(event);
        }

        state_stack.update(event);
        window.drawBegin();
        state_stack.draw();
        window.drawEnd();
    }

    return 0;
}