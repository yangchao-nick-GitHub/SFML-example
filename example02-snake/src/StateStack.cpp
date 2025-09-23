// StateStack.cpp
#include "StateStack.h"

StateStack::StateStack(State::Context context)
: m_context(context) {}

void StateStack::update(sf::Time dt) {
    if (!m_stack.empty())
        m_stack.back()->update(dt);
}

void StateStack::draw() {
    for (auto& state : m_stack)
        state->draw();
}

void StateStack::handleEvent(const sf::Event& event) {
    if (!m_stack.empty())
        m_stack.back()->handleEvent(event);
}

void StateStack::pushState(States id) {
    m_stack.push_back(createState(id));
}

void StateStack::popState() {
    if (!m_stack.empty())
        m_stack.pop_back();
}

void StateStack::clearStates() {
    m_stack.clear();
}

std::unique_ptr<State> StateStack::createState(States id) {
    auto found = m_factories.find(id);
    return found->second();
}
