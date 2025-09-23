#ifndef STATE_H
#define STATE_H

#include <SFML/Graphics.hpp>

class StateStack; // 前置声明

class State {
public:
    struct Context {
        sf::RenderWindow* window;
        class CommandQueue* commands;
    };

    State(StateStack& stack, Context context);
    virtual ~State() = default;

    virtual void draw() = 0;
    virtual bool update(sf::Time dt) = 0;
    virtual bool handleEvent(const sf::Event& event) = 0;

protected:
    void requestStackPush(int stateID);
    void requestStackPop();
    void requestStackClear();

    Context m_context;
    StateStack& m_stack;
};

#endif