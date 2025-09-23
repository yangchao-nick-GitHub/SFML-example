// GameState.cpp
#include "GameState.h"
#include "StateStack.h"

GameState::GameState(StateStack& stack, Context context)
: State(stack, context) {
    // 在进入 Game 状态时绑定输入
    m_input.bindAction("MoveLeft", InputType::Keyboard, sf::Keyboard::A, [] {
        return std::make_unique<MoveLeftCommand>();
    });
    m_input.bindAction("Shoot", InputType::MouseButton, sf::Mouse::Left, [] {
        return std::make_unique<FireCommand>();
    });
}

void GameState::draw() {
    // 绘制游戏画面
}

bool GameState::update(sf::Time dt) {
    while (!m_context.commands->isEmpty()) {
        auto cmd = m_context.commands->pop();
        // TODO: cmd->execute(player); // 需要实现 Entity
        cmd->execute(*(Entity*)nullptr); // 仅演示
    }
    return true;
}

bool GameState::handleEvent(const sf::Event& event) {
    m_input.processEvent(event, *m_context.commands);
    return true;
}
