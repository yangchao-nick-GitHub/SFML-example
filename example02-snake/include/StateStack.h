// StateStack.h
#ifndef STATESTACK_H
#define STATESTACK_H

#include "State.h"
#include <vector>
#include <map>
#include <functional>
#include <memory>

enum class States { Menu, Game, Pause };

class StateStack {
public:
    explicit StateStack(State::Context context);

    template <typename T>
    void registerState(States id) {
        m_factories[id] = [this]() {
            return std::make_unique<T>(*this, m_context);
        };
    }

    void update(sf::Time dt);
    void draw();
    void handleEvent(const sf::Event& event);

    void pushState(States id);
    void popState();
    void clearStates();

private:
    std::unique_ptr<State> createState(States id);

    std::vector<std::unique_ptr<State>> m_stack;
    std::map<States, std::function<std::unique_ptr<State>()>> m_factories;

    State::Context m_context;
};

#endif
