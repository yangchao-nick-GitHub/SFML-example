#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "common.hpp"


int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Window");

    Entity entity;

    CommandQueue command_queue;

    Context context{window, command_queue, entity};

    StateStack game_stack(context);

    game_stack.registerState<MenuState>(StateID::Menu);
    game_stack.registerState<GameState>(StateID::Game);
    game_stack.registerState<PauseState>(StateID::Pause);
    game_stack.registerState<PauseState>(StateID::GameOver);


    game_stack.pushState(StateID::Menu);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            game_stack.handleEvents(event, entity);
        }

        game_stack.update();

        window.clear();
        game_stack.draw();

        window.display();

       
    }
    
    return 0;
}