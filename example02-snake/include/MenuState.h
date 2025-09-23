// MenuState.h
#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "State.h"
#include <iostream>

class MenuState : public State {
public:
    MenuState(StateStack& stack, Context context);

    void draw() override;
    bool update(sf::Time dt) override;
    bool handleEvent(const sf::Event& event) override;
};

#endif
