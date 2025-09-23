// GameState.h
#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "State.h"
#include "InputManager.h"
#include "command.h"
#include <iostream>

class GameState : public State {
public:
    GameState(StateStack& stack, Context context);

    void draw() override;
    bool update(sf::Time dt) override;
    bool handleEvent(const sf::Event& event) override;

private:
    InputManager m_input;
};

#endif
