#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <ctime>
#include <window.h>

#include "StateStack.h"
#include "MenuState.h"
#include "GameState.h"
#include "command.h"

using namespace sf;

class Entity {
public:
    void update() {};
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "StateStack Example");
    CommandQueue commands;

    State::Context context{ &window, &commands };
    StateStack stack(context);

    stack.registerState<MenuState>(States::Menu);
    stack.registerState<GameState>(States::Game);

    stack.pushState(States::Menu);

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Time dt = clock.restart();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            stack.handleEvent(event);
        }

        stack.update(dt);

        window.clear();
        stack.draw();
        window.display();
    }


    return 0;
}


