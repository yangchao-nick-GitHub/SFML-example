// MenuState.cpp
#include "MenuState.h"
#include "StateStack.h"

MenuState::MenuState(StateStack& stack, Context context)
: State(stack, context) {}

void MenuState::draw() {
    // 这里可以绘制菜单
}

bool MenuState::update(sf::Time dt) {
    return true;
}

bool MenuState::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
        std::cout << "进入游戏状态\n";
        requestStackPush((int)States::Game);
    }
    return true;
}
