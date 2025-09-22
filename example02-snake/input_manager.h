// InputManager.h
#pragma once
#include <SFML/Window.hpp>
#include <unordered_map>
#include <string>
#include "command.h"

enum class InputType { Keyboard, MouseButton, JoystickButton };

using CommandFactory = std::function<std::unique_ptr<Command>()>;

struct InputBinding {
    InputType type;
    int code;   // 对于Keyboard就是sf::Keyboard::Key，对于Mouse就是sf::Mouse::Button
};

struct ActionBinding {
    InputBinding input;
    CommandFactory factory;
};

class InputManager {
public:
    void bindAction(const std::string& action, InputType type, int code, CommandFactory factory);
    void processEvent(const sf::Event& event, CommandQueue& commands);
    void update(); 

private:
    std::unordered_map<std::string, ActionBinding> m_actions;
    std::unordered_map<std::string, bool> m_pressed;
    std::unordered_map<std::string, bool> m_held;
    std::unordered_map<std::string, bool> m_released;
};

