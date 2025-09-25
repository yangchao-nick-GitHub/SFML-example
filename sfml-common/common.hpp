#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <queue>
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <SFML/Window.hpp>

enum class InputType {
    Keyboard,
    MouseButton
};

class Entity {

};

struct InputBinding {
    InputType type;
    int code;
};

class Command;

using CommandFactory = std::function<std::unique_ptr<Command>()>;

struct ActionBinding {
    InputBinding input;
    CommandFactory command_factory;
};


class Command {
public:
    virtual void execute(Entity& entity) = 0;
};


class LeftMoveCommand : public Command {
public:
    void execute(Entity& entity) override
    {
        std::cout << "Left Moving" << std::endl;
    }
};

class RightMoveCommand : public Command {
public:
    void execute(Entity& entity) override
    {
        std::cout << "Right Moving" << std::endl;
    }
};

class UpMoveCommand : public Command {
public:
    void execute(Entity& entity) override
    {
        std::cout << "Up Moving" << std::endl;
    }
};

class DownMoveCommand : public Command {
public:
    void execute(Entity& entity) override
    {
        std::cout << "Right Moving" << std::endl;
    }
};

class PuaseGameCommand : public Command {
public:
    void execute(Entity& entity) override
    {
        std::cout << "Pause" << std::endl;
    }
};

class FireCommand : public Command {
public:
    void execute(Entity& entity) override
    {
        std::cout << "Firing" << std::endl;
    }
};

class CommandQueue {
public:
    void pushCommand(std::unique_ptr<Command> command)
    {
        commands_.push(std::move(command));
    }

    bool isEmpty()
    {
        return commands_.empty();
    }

    int size()
    {
        return commands_.size();
    }

    std::unique_ptr<Command> pop()
    {
        if (commands_.empty())
        {
            return nullptr;
        }

        auto command = std::move(commands_.front());
        commands_.pop();
        return command;
    }

private:
    std::queue<std::unique_ptr<Command>> commands_;
};


class InputManager {
public:
    void bindAction(std::string action, InputType type, int32_t code, CommandFactory command)
    {
        actions_[action] = { {type, code}, command };
    }

    void processEvent(CommandQueue& command_queue, sf::Event& event, Entity& entity)
    {
        for (const auto& pair : actions_)
        {
            bool is_pressed = false;
            bool is_released = false;
            std::string action_name =  pair.first;
            const ActionBinding& action_binding = pair.second;

            if (action_binding.input.type == InputType::Keyboard)
            {
                if (event.type == sf::Event::KeyPressed && event.key.code == action_binding.input.code)
                {
                    is_pressed = true;
                }

                if (event.type == sf::Event::KeyReleased && event.key.code == action_binding.input.code)
                {
                    is_released = true;
                }

            }
            if (action_binding.input.type == InputType::MouseButton)
            {   
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == action_binding.input.code)
                {
                    is_pressed = true;
                }
                if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == action_binding.input.code)
                {
                    is_released = true;
                }
            }

            if (is_pressed)
            {
                command_queue.pushCommand(action_binding.command_factory());
            }
        }
    }
private:
    std::unordered_map<std::string, ActionBinding> actions_;
    std::vector<sf::Keyboard::Key> is_pressed_;
    std::vector<sf::Keyboard::Key> is_released_;
};


class StateStack;


enum class StateID {
    None,
    Menu,
    Game,
    Pause,
    Settings,
    GameOver,
};

struct Context {
    sf::RenderWindow& window;
    CommandQueue& command_queue;
    Entity& player;
};


class State {
public:


    State(StateStack& stack, Context context)
        : stack_(stack),
            context_(context)
        {
        }

    ~State() = default;

    virtual void draw() = 0;
    virtual void update() = 0;
    virtual bool handleEvents(sf::Event& event, Entity& player) = 0;
protected:
    StateStack& stack_;
    Context context_;
};

class StateStack {
public:
    StateStack(Context& context):
    context_(context)
    {
    }

    void draw()
    {
        for (auto& state : stack_)
        {
            state->draw();
        }
    }

    void update()
    {
        if (!stack_.empty())
        {
            stack_.back()->update();
        }
    }

    template <typename T>
    void registerState(StateID id)
    {
        factory_[id] = [this]() {
            return std::make_unique<T>(*this, context_);
        };
    }

    std::unique_ptr<State> getStateInstance(StateID state_id)
    {
        auto found = factory_.find(state_id);
        if (found != factory_.end())
        {
            return found->second();    
        }

        return nullptr;
    }

    void pushState(StateID state_id)
    {
        auto state = getStateInstance(state_id);
        if (!state)
        {
            std::cout << "State not found" << std::endl;
        }

        stack_.push_back(std::move(state));
    }

    void popState()
    {
        if (stack_.empty())
        {
            return ;
        }
        stack_.pop_back();
    }

    void handleEvents(sf::Event& event, Entity& player)
    {
        if (!stack_.empty())
        {
            stack_.back()->handleEvents(event, context_.player);
        }
    }
private:
    std::vector<std::unique_ptr<State>> stack_;
    std::map<StateID, std::function<std::unique_ptr<State>()>> factory_;
    Context context_;
};



class MenuState : public State {
public:

    MenuState(StateStack& stack, Context context)
    :State(stack, context)
    {
    }

    void draw() override
    {
        // 绘制游戏菜单

    }

    void update() override
    {
        printMenu();
    }

    bool handleEvents(sf::Event& event, Entity& player) override
    {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
        {
            std::cout<< "start!" << std::endl;
            stack_.pushState(StateID::Game);
        }
        return true;
    }
private:
    void printMenu()
    {
        // 清屏（在实际应用中，你可能需要使用系统特定的清屏命令）
        std::cout << "\n\n\n";
        std::cout << "==============================\n";
        std::cout << "         SFML GAME            \n";
        std::cout << "==============================\n";
        std::cout << "\n";

        std::cout << "\n";
        std::cout << "Press ENTER to game\n";
        std::cout << "\n";
    }
};


class GameState : public State {
public: 
    GameState(StateStack& stack, Context context)
    :State(stack, context)
    {
        input_mgr_.bindAction("MoveLeft", InputType::Keyboard, sf::Keyboard::A, []() {
            return std::make_unique<LeftMoveCommand>();
        });

        input_mgr_.bindAction("MoveRight", InputType::Keyboard, sf::Keyboard::D, []() {
            return std::make_unique<RightMoveCommand>();
        });

        input_mgr_.bindAction("MoveUp", InputType::Keyboard, sf::Keyboard::W, []() {
            return std::make_unique<UpMoveCommand>();
        });

        input_mgr_.bindAction("MoveDown", InputType::Keyboard, sf::Keyboard::S, []() {
            return std::make_unique<DownMoveCommand>();
        });

        input_mgr_.bindAction("Pause", InputType::Keyboard, sf::Keyboard::P, []() {
            return std::make_unique<PuaseGameCommand>();
        });
    
        input_mgr_.bindAction("Shoot", InputType::MouseButton, sf::Mouse::Left, []() {
            return std::make_unique<FireCommand>();
        });
    }

    void draw() override
    {
        // 绘制游戏场景
        std::cout << "\n\n\n";
        std::cout << "==============================\n";
        std::cout << "         GAME START            \n";
        std::cout << "==============================\n";
        std::cout << "\n";
    }

    void update() override
    {
        while (!context_.command_queue.isEmpty())
        {
            auto cmd = context_.command_queue.pop();
            if (cmd)
            {
                cmd->execute(context_.player);
            }
        }
    }

    bool handleEvents(sf::Event& event, Entity& player) override
    {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
        {
            std::cout << "pause game debug! go to Pause state." << std::endl;
            stack_.pushState(StateID::Pause);
            return true;
        }
        input_mgr_.processEvent(context_.command_queue, event, player);
        return true;
    }
private:
    InputManager input_mgr_;
};

class PauseState : public State {
public:
    PauseState(StateStack& stack, Context context)
    :State(stack, context)
    {
    }

    void draw() override
    {
        // 绘制游戏场景
        std::cout << "\n\n\n";
        std::cout << "==============================\n";
        std::cout << "         GAME PAUSE            \n";
        std::cout << "==============================\n";
        std::cout << "\n";
    }

    void update() override
    {

    }

    bool handleEvents(sf::Event& event, Entity& player) override
    {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        {
            std::cout << "restore game!" << std::endl;
            stack_.popState();
        }
    }
};



#endif

