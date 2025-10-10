#include "snake_game.h"
#include <assert.h>
#include <string.h>


World::World(Vector2f wind_size, uint16_t world_block_size)
	: m_windowSize(wind_size), m_wall_block_size(world_block_size)

{
	static bool initialized = false;
	
	if (!initialized) {
		srand(static_cast<unsigned>(time(nullptr)));
		initialized = true;
	}
	init();
}

void World::RespawnApple()
{

	int maxX = (m_windowSize.x / m_wall_block_size) - 2; // 减少2格边界
	int maxY = (m_windowSize.y / m_wall_block_size) - 2; // 减少2格边界
	m_item = sf::Vector2f(
		rand() % (maxX - 2) + 2,  // 从第2格开始，避免靠近左/上墙壁
		rand() % (maxY - 2) + 2); // 从第2格开始，避免靠近上/下墙壁
	m_apple.setPosition(m_item.x * m_wall_block_size, m_item.y * m_wall_block_size);
}

void World::RespawnReward()
{
	int maxX = (m_windowSize.x / m_wall_block_size) - 2; // 减少2格边界
	int maxY = (m_windowSize.y / m_wall_block_size) - 2; // 减少2格边界
	m_rewardItem = sf::Vector2f(
		rand() % (maxX - 2) + 2,  // 从第2格开始
		rand() % (maxY - 2) + 2); // 从第2格开始
	m_reward.setPosition(m_rewardItem.x * m_wall_block_size, m_rewardItem.y * m_wall_block_size);
}

void World::Render(GameWindow& window)
{
	for (int i = 0; i < 4; i++)
	{
		window.draw(m_bound[i]);
	}
	window.draw(m_apple);
	window.draw(m_reward);
}

void World::update(std::shared_ptr<Snake> snake)
{
	if (snake->getHeadPos() == m_item)
	{
		snake->appendScore(2);
		snake->ExtendSnake();
		RespawnApple();
	}

	if (snake->getHeadPos() == m_rewardItem)
	{
		snake->appendScore(5);
		snake->ExtendSnake();
		RespawnReward();
		snake->addSpeed(5);
	}

	/* 检测snake是否碰撞到墙壁 */
	if (snake->getHeadPos().x <= 0 || snake->getHeadPos().x >= m_windowSize.x / m_wall_block_size - 1 ||
		snake->getHeadPos().y <= 0 || snake->getHeadPos().y >= m_windowSize.y / m_wall_block_size - 1)
	{
		snake->setAlive(false);
	}

}

void World::init()
{
	m_apple.setRadius(m_wall_block_size / 2);
	m_reward.setSize(Vector2f(m_wall_block_size, m_wall_block_size));
	RespawnApple();
	RespawnReward();

	m_bound[0].setSize(Vector2f(m_windowSize.x, m_wall_block_size));
	m_bound[1].setSize(Vector2f(m_wall_block_size, m_windowSize.y));
	m_bound[2].setSize(Vector2f(m_windowSize.x, m_wall_block_size));
	m_bound[3].setSize(Vector2f(m_wall_block_size, m_windowSize.x));

	m_bound[0].setPosition(0, 0);
	m_bound[1].setPosition(m_windowSize.x - m_wall_block_size, 0);
	m_bound[2].setPosition(0, m_windowSize.x - m_wall_block_size);
	m_bound[3].setPosition(0, 0);

	m_bound[0].setFillColor(Color(135, 89, 64));
	m_bound[1].setFillColor(Color(135, 89, 64));
	m_bound[2].setFillColor(Color(135, 89, 64));
	m_bound[3].setFillColor(Color(135, 89, 64));

	m_apple.setFillColor(Color::Red);
	m_reward.setFillColor(Color::Magenta);

}


Snake::Snake(uint16_t window_block_size)
{
	m_size = window_block_size;
	body_rect.setSize(sf::Vector2f(m_size - 1, m_size - 1));
	reset();
}

void Snake::reset()
{
	snake_body_container.clear();
	snake_body_container.push_back(SnakeSegment(20, 15));
	snake_body_container.push_back(SnakeSegment(20, 16));
	snake_body_container.push_back(SnakeSegment(20, 17));
	setDirection(Direction::Up);
	m_speed = SNAKE_MOVE_SPEED;
	m_alive = true;
	m_score = 0;
}

Vector2f Snake::getHeadPos()
{
	if (!snake_body_container.empty())
	{
		return snake_body_container[0].postion;
	}
	return Vector2f(1, 1);
}


void Snake::ExtendSnake()
{
	if (snake_body_container.empty())
	{
		return;
	}

	int size = snake_body_container.size();
	SnakeSegment& seg_head = snake_body_container[0];

	if (size > 1)
	{
		SnakeSegment& seg_tail = snake_body_container[size - 1];
		if (seg_head.postion.x == seg_tail.postion.x)
		{
			if (seg_head.postion.y > seg_tail.postion.y)
			{
				snake_body_container.push_back(SnakeSegment(seg_tail.postion.x, seg_tail.postion.y - 1));
			}
			else
			{
				snake_body_container.push_back(SnakeSegment(seg_tail.postion.x, seg_tail.postion.y + 1));
			}
		}
		else if (seg_head.postion.y == seg_tail.postion.y)
		{
			if (seg_head.postion.x > seg_tail.postion.x)
			{
				snake_body_container.push_back(SnakeSegment(seg_tail.postion.x - 1, seg_tail.postion.y));
			}
			else
			{
				snake_body_container.push_back(SnakeSegment(seg_tail.postion.x + 1, seg_tail.postion.y));
			}
		}
	}
	else
	{
		if (direction == Direction::Up)
		{
			snake_body_container.push_back(SnakeSegment(seg_head.postion.x, seg_head.postion.y + 1));
		}
		else if (direction == Direction::Down)
		{
			snake_body_container.push_back(SnakeSegment(seg_head.postion.x, seg_head.postion.y - 1));
		}
		else if (direction == Direction::Left)
		{
			snake_body_container.push_back(SnakeSegment(seg_head.postion.x + 1, seg_head.postion.y));
		}
		else if (direction == Direction::Right)
		{
			snake_body_container.push_back(SnakeSegment(seg_head.postion.x - 1, seg_head.postion.y));
		}
	}
}



void Snake::Tick()
{
	// std::cout << "Tick" << std::endl;
	if (snake_body_container.empty() || direction == Direction::None)
	{
		return;
	}

	move();
	CheckCollision();
}

void Snake::move()
{
	if (snake_body_container.empty())
	{
		return;
	}

	int size = snake_body_container.size();

	for (int i = snake_body_container.size() - 1; i > 0; i--)
	{
		snake_body_container[i] = snake_body_container[i - 1];
	}

	if (direction == Direction::Up)
	{
		snake_body_container[0].postion.y--;
	}
	else if (direction == Direction::Down)
	{
		snake_body_container[0].postion.y++;
	}
	else if (direction == Direction::Left)
	{
		snake_body_container[0].postion.x--;
	}
	else if (direction == Direction::Right)
	{
		snake_body_container[0].postion.x++;
	}
}

void Snake::CheckCollision()
{
	int size = snake_body_container.size();
	if (size < 5)
	{
		return;
	}

	SnakeSegment& seg_head = snake_body_container[0];
	for (int i = 1; i < snake_body_container.size(); i++)
	{
		if (snake_body_container[i].postion == seg_head.postion)
		{
			setAlive(false);
			break;
		}
	}
}


void Snake::Render(GameWindow& window)
{
	if (snake_body_container.empty())
	{
		return;
	}

	SnakeSegment& seg_head = snake_body_container[0];
	body_rect.setFillColor(sf::Color::Yellow);
	body_rect.setPosition(seg_head.postion.x * m_size, seg_head.postion.y * m_size);
	window.draw(body_rect);

	for (int i = 1; i < snake_body_container.size(); i++)
	{
		SnakeSegment& seg = snake_body_container[i];
		body_rect.setFillColor(sf::Color::Green);
		body_rect.setPosition(seg.postion.x * m_size, seg.postion.y * m_size);
		window.draw(body_rect);
	}
}

void Snake::addSpeed(uint16_t add_speed)
{
	m_speed += add_speed;
}

void Snake::checkSelfCollision()
{
	if (snake_body_container[0].postion == snake_body_container[1].postion)
	{
		std::cout << "0 == 1" << std::endl;
		assert(false);
	}
	else if (snake_body_container[0].postion == snake_body_container[2].postion)
	{
		std::cout << "0 == 2" << std::endl;
		assert(false);
	}
	else if (snake_body_container[1].postion == snake_body_container[2].postion)
	{
		std::cout << "1 == 2" << std::endl;
		assert(false);
	}
}

Direction Snake::getPhysicalDirection()
{
	SnakeSegment& head = snake_body_container[0];
	SnakeSegment& head_next = snake_body_container[1];

	if (head.postion.x == head_next.postion.x)
	{
		if (head.postion.y > head_next.postion.y)
		{
			return Direction::Down;
		}
		else
		{
			return Direction::Up;
		}
	}
	else if (head.postion.y == head_next.postion.y)
	{ 
        if (head.postion.x > head_next.postion.x)
		{
			return Direction::Right;
		}
		else
		{
			return Direction::Left;
		}
	}
	
	return Direction::None;
}


State::State(SnakeContext& context)
    : context_(context)
{
}

SnakeMenuState::SnakeMenuState(SnakeContext& context)
	: State(context)
{
}

void SnakeMenuState::draw()
{
	RectangleShape rect(Vector2f(800, 800));
	rect.setFillColor(Color(135, 89, 64));
	context_.window.getWindow()->draw(rect);
}

void SnakeMenuState::update(sf::Event& event)
{ 
	
}

bool SnakeMenuState::handleInput(sf::Event& event)
{
	if (event.type == Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
	{
		context_.window.getWindow()->close();
		return true;
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
	{
		std::cout<< "enter the Enter Key!" << std::endl;
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
	{
		std::cout<< "game pause!" << std::endl;
		context_.state_stack.pushState(StateID::Pause);
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
	{
		std::cout<< "game start!" << std::endl;
		context_.state_stack.pushState(StateID::Game);
	}

    return true;
}

SnakePauseState::SnakePauseState(SnakeContext& context)
	: State(context)
{
}

void SnakePauseState::update(sf::Event& event)
{
	
}

void SnakePauseState::draw()
{
	RectangleShape rect(Vector2f(800, 800));
	rect.setFillColor(Color(0, 255, 0));
	context_.window.getWindow()->draw(rect);
}

bool SnakePauseState::handleInput(sf::Event& event)
{
	if (event.type == Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
	{
		context_.window.getWindow()->close();
		return true;
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
	{
		std::cout<< "SnakePauseState Space!" << std::endl;

	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
	{
		std::cout<< "exit SnakePauseState!" << std::endl;
		context_.state_stack.last_paused_state = true;
		context_.state_stack.cur_paused_state = false;
		memset(&event, 0, sizeof(event));
		context_.state_stack.popState();
	}

    return true;
}



SnakeGameState::SnakeGameState(SnakeContext& context)
    : State(context)
{
	world_ = std::make_shared<World>(static_cast<Vector2f>(context.window.getSize()), WORLD_BLOCK_SIZE);
	snake_ = std::make_shared<Snake>(WORLD_BLOCK_SIZE);
}

void SnakeGameState::draw()
{
    snake_->Render(context_.window);
    world_->Render(context_.window);
}

void SnakeGameState::update(sf::Event& event)
{
    float step = 1.0f / snake_->getSpeed();

	// Pause
	if (context_.state_stack.cur_paused_state == true)
	{
		return ;
	}

	// Puase to game
	if (context_.state_stack.last_paused_state == true && 
		context_.state_stack.cur_paused_state == false)
	{
		context_.state_stack.last_paused_state = false;
		// std::cout << "update clock_.restart()" << std::endl;
		clock_.restart();
		return ;
	}

	// Menu to game
	if (context_.state_stack.last_paused_state == false && 
		context_.state_stack.cur_paused_state == false)
	{ 
		elapsedTime_ += clock_.restart().asSeconds();
	}


	// std::cout << "elapsedTime_:" << elapsedTime_ << " step:" << step <<std::endl;
	if (elapsedTime_ >= step)
	{
		// std::cout << "elapsedTime_ >= step" << std::endl;
		elapsedTime_ -= step;
		snake_->Tick();
		world_->update(snake_);
		if (!snake_->isAlive())
		{
			snake_->reset();
		}
	}

	
}

bool SnakeGameState::handleInput(sf::Event& event)
{
	if (event.type == Event::KeyPressed)
	{
		switch (event.key.code)
		{
		case Keyboard::Escape:
			context_.window.getWindow()->close();
			break;
		case Keyboard::Up:
			if (snake_->getPhysicalDirection() != Direction::Up && snake_->getPhysicalDirection() != Direction::Down)
			{
				snake_->setDirection(Direction::Up);
			}
			break;
		case Keyboard::Down:
			if (snake_->getPhysicalDirection() != Direction::Up && snake_->getPhysicalDirection() != Direction::Down)
			{
				snake_->setDirection(Direction::Down);
			}
			break;
		case Keyboard::Left:
			if (snake_->getPhysicalDirection() != Direction::Left && snake_->getPhysicalDirection() != Direction::Right)
			{
				snake_->setDirection(Direction::Left);
			}
			break;
		case Keyboard::Right:
			if (snake_->getPhysicalDirection() != Direction::Left && snake_->getPhysicalDirection() != Direction::Right)
			{
				snake_->setDirection(Direction::Right);
			}
			break;
		case Keyboard::P:
			std::cout << "Pause-------------------------------------------------" << std::endl;
			context_.state_stack.last_paused_state = false;
			context_.state_stack.cur_paused_state = true;
			context_.state_stack.pushState(StateID::Pause);
			break;
		default:
			break;

		}
	}

	return true;
}


std::unique_ptr<State> StateStack::getStateInstance(StateID state_id)
{
    auto found = factory_.find(state_id);
    if (found != factory_.end())
    {
        return found->second();    
    }

    return nullptr;
}

void StateStack::pushState(StateID state_id)
{
    auto state = getStateInstance(state_id);
    if (!state)
    {
        std::cout << "State not found" << std::endl;
    }

    stack_.push_back(std::move(state));
}

void StateStack::popState()
{
    if (stack_.empty())
    {
        return ;
    }
    stack_.pop_back();
}

void StateStack::handleInput(sf::Event& event)
{
    if (!stack_.empty())
    {
        stack_.back()->handleInput(event);
    }
}

void StateStack::update(sf::Event& event)
{
    if (!stack_.empty())
    {
        stack_.back()->update(event);
    }
}

void StateStack::clear()
{
    stack_.clear();
}

void StateStack::draw()
{
    if (!stack_.empty())
    {
        stack_.back()->draw();
    }

}







