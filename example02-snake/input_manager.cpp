// InputManager.cpp
#include "input_manager.h"

void InputManager::bindAction(const std::string& action, InputType type, int code, CommandFactory factory) {
    m_actions[action] = { {type, code}, factory };
}

void InputManager::processEvent(const sf::Event& event, CommandQueue& commands) {
    for (auto& [action, ab] : m_actions) {
        auto& binding = ab.input;

        bool pressed = false;
        bool released = false;

        if (binding.type == InputType::Keyboard) {
            if (event.type == sf::Event::KeyPressed && event.key.code == binding.code) pressed = true;
            if (event.type == sf::Event::KeyReleased && event.key.code == binding.code) released = true;
        }
        if (binding.type == InputType::MouseButton) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == binding.code) pressed = true;
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == binding.code) released = true;
        }

        if (pressed) {
            m_pressed[action] = true;
            m_held[action] = true;
            commands.push(ab.factory()); // 直接生成 Command 放入队列
        }
        if (released) {
            m_released[action] = true;
            m_held[action] = false;
        }
    }
}


void InputManager::update() {
    // 每帧清空按下/释放，只保留持续状态
    m_pressed.clear();
    m_released.clear();
}


