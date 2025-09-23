// command.h
#ifndef COMMAND_H
#define COMMAND_H

#include <functional>
#include <SFML/System.hpp>
#include <iostream>
#include <queue>
#include <memory>

/*

扩展命令模式 (Command Pattern)，让输入直接触发游戏行为。这样做的好处是：
输入和行为解耦（便于改键、多人输入、AI 模拟输入）。
命令可以排队、撤销、重放（适合回放/录像系统）。
不同游戏状态可以注册不同的命令集。

*/

class Entity; // 前置声明，表示游戏对象

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(Entity& entity) = 0;
};


class MoveLeftCommand : public Command {
public:
    void execute(Entity& entity) override {
        // entity.move(-1, 0); 假设 Entity 有 move()
        std::cout << "Entity Move Left\n";
    }
};


class FireCommand : public Command {
public:
    void execute(Entity& entity) override {
        // entity.fire();
        std::cout << "Entity Fire!\n";
    }
};

class CommandQueue {
public:
    void push(std::unique_ptr<Command> cmd) {
        m_queue.push(std::move(cmd));
    }

    bool isEmpty() const { return m_queue.empty(); }

    std::unique_ptr<Command> pop() {
        if (m_queue.empty()) return nullptr;
        auto cmd = std::move(m_queue.front());
        m_queue.pop();
        return cmd;
    }

private:
    std::queue<std::unique_ptr<Command>> m_queue;
};

#endif