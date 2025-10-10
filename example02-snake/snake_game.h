#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include "window.h"
#include <memory>
#include <iostream>
#include <functional>

#define WORLD_BLOCK_SIZE 20
#define SNAKE_MOVE_SPEED 5
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
	void Render(GameWindow& window);
	int getSpeed() { return m_speed; }
	bool isAlive() { return m_alive; }
	void setAlive(bool alive) { m_alive = alive; }
	void addSpeed(uint16_t add_speed);
	size_t getSnakeSegSize() { return snake_body_container.size(); }
	void checkSelfCollision();
	Direction getPhysicalDirection();
	uint32_t getScore() { return m_score; }
	void appendScore(uint32_t score) {  m_score += score; }

	using snake_container = std::vector<SnakeSegment>;
private:
	snake_container snake_body_container;
	int m_size;
	int m_speed{ SNAKE_MOVE_SPEED };
	bool m_alive{ false };
	Direction direction{ Direction::None };
	RectangleShape body_rect;
	Vector2f postion;
	uint32_t m_score { 0 };
};

class World {
public:
	World(Vector2f wind_size, uint16_t block_size);
	void RespawnApple();
	void RespawnReward();
	void Render(GameWindow& window);
	void update(std::shared_ptr<Snake> snake);
	void init();
private:
	Vector2f m_windowSize;
	Vector2f m_item;
	Vector2f m_rewardItem;

	RectangleShape m_bound[4];
	CircleShape m_apple;
	RectangleShape m_reward;
	std::shared_ptr<Snake> m_snake{ nullptr };
	uint16_t m_wall_block_size;
};

class StateStack;
struct SnakeContext {
    StateStack& state_stack;
    GameWindow& window;
} ;

enum class StateID {
    None,
    Menu,
    Game,
    Pause,
    Settings,
    GameOver,
};


class State {
public:
    State(SnakeContext& context);
    virtual ~State() = default;

    virtual void draw() = 0;
    virtual void update(sf::Event& event) = 0;
    virtual bool handleInput(sf::Event& event) = 0;
protected:
    SnakeContext& context_;
};

class SnakeMenuState : public State {
public:
    SnakeMenuState(SnakeContext& context);
    void draw() override;
    void update(sf::Event& event) override;
    bool handleInput(sf::Event& event) override;
};

class SnakePauseState : public State {
public:
    SnakePauseState(SnakeContext& context);
    void draw() override;
    void update(sf::Event& event) override;
    bool handleInput(sf::Event& event) override;
};

class SnakeGameState : public State {
public: 
    SnakeGameState(SnakeContext& context);
    void draw() override;
    void update(sf::Event& event) override;
    bool handleInput(sf::Event& event) override;
private:
	std::shared_ptr<World> world_;
    std::shared_ptr<Snake> snake_;
    sf::Clock clock_;
    float elapsedTime_;
};


class StateStack {
public:
    void draw();
    void clear();
    void update(sf::Event& event);

    template <typename T>
    void registerState(StateID id, SnakeContext& context)
    {
        factory_[id] = [context]() mutable {
            return std::make_unique<T>(context);
        };
    }

    std::unique_ptr<State> getStateInstance(StateID state_id);
    void pushState(StateID state_id);
    void popState();
    void handleInput(sf::Event& event);
	// bool isPaused() { return is_paused; }
	// void setPaused(bool pause) { is_paused = pause; }
	bool cur_paused_state { false };
	bool last_paused_state { false };

private:
    std::vector<std::unique_ptr<State>> stack_;
    std::map<StateID, std::function<std::unique_ptr<State>()>> factory_;

};






#endif