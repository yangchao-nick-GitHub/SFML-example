#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <memory>
#include <random>

#include "window.h"
#include "snake_game.h"


using namespace sf;

#define WORLD_BLOCK_SIZE 20
#define SNAKE_MOVE_SPEED 10
#define SNAKE_MOVE_STEP_SIZE 20

enum class Direction
{
	None,
	Up,
	Down,
	Left,
	Right
};

struct SnakeSegment {
	SnakeSegment(int x, int y)
		:postion(x, y) {
	}
	Vector2f postion;
};

class Snake {
public:
	Snake(uint16_t window_block_size);
	~Snake() {}
	void reset();
	Direction getDirection() { return direction; }
	Vector2f getHeadPos();
	void setDirection(Direction dir) { direction = dir; }
	void ExtendSnake();
	void Tick();
	void move();
	void CheckCollision();
	void Render(std::shared_ptr<GameWindow> window);
	int getSpeed() { return m_speed; }
	bool isAlive() { return m_alive; }
	void setAlive(bool alive) { m_alive = alive; }
	void addSpeed(uint16_t add_speed);

	using snake_container = std::vector<SnakeSegment>;
private:
	snake_container snake_body_container;
	int m_size;
	int m_speed{ SNAKE_MOVE_SPEED };
	bool m_alive{ false };
	Direction direction{ Direction::None };
	RectangleShape body_rect;
	Vector2f postion;
};

class World {
public:
    World(Vector2f wind_size, uint16_t block_size);
    void RespawnApple();
    void RespawnReward();
    void Render(std::shared_ptr<GameWindow> window);
    void update(std::shared_ptr<Snake> snake);
    void init();
private:
    Vector2f m_windowSize;
    Vector2f m_item;
    Vector2f m_rewardItem;

    RectangleShape m_bound[4];
    CircleShape m_apple;
    RectangleShape m_reward;
    std::shared_ptr<Snake> m_snake { nullptr };
    uint16_t m_wall_block_size;
};

class GamePrompt
{
public:
	GamePrompt(std::shared_ptr<GameWindow> window, const Vector2u& windowSize);
	void initTexts(const Vector2u& windowSize);
	void render();

private:
	Font m_font;
	Text m_gameOverText;
	Text m_continueText;
	std::shared_ptr<GameWindow> m_window;
};


class SnakeGame {
public:
	SnakeGame(std::shared_ptr<GameWindow> window, uint16_t world_block_size);
	void handleInput();
	void render();
	std::shared_ptr<GameWindow> getWindow();
    void run();
    void update();
	
private:
	std::shared_ptr<GameWindow> window_ {nullptr};
	Clock m_clock;
	float m_elapsed { 0.0 };
	float m_timeStep{ 0.0 };
    std::shared_ptr<World> m_world;
	std::shared_ptr<Snake> m_snake;
	std::shared_ptr<GamePrompt> m_prompt;
};
