#include "State.h"
#include "StateStack.h"

State::State(StateStack& stack, Context context)
: m_context(context), m_stack(stack) {}

void State::requestStackPush(int stateID) {
    m_stack.pushState(static_cast<States>(stateID));
}

void State::requestStackPop() {
    m_stack.popState();
}

void State::requestStackClear() {
    m_stack.clearStates();
}