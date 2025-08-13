#include "snake_game.h"

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

void World::Render(std::shared_ptr<GameWindow> window)
{
    for (int i = 0; i < 4; i++)
    {
        window->draw(m_bound[i]);
    }
    window->draw(m_apple);
	window->draw(m_reward);
}

void World::update(std::shared_ptr<Snake> snake)
{
    if (snake->getHeadPos() == m_item)
    {
        snake->ExtendSnake();
        RespawnApple();
    }
    
    if (snake->getHeadPos() == m_rewardItem)
    {
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
	setDirection(Direction::None);
	m_speed = SNAKE_MOVE_SPEED;
	m_alive = true;
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
		return ;
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


void Snake::Render(std::shared_ptr<GameWindow> window)
{
	if (snake_body_container.empty())
	{
		return;
	}

	SnakeSegment& seg_head = snake_body_container[0];
	body_rect.setFillColor(sf::Color::Yellow);
	body_rect.setPosition(seg_head.postion.x * m_size, seg_head.postion.y * m_size);
	window->draw(body_rect);

	for (int i = 1; i < snake_body_container.size() ; i++)
	{
		SnakeSegment& seg = snake_body_container[i];
		body_rect.setFillColor(sf::Color::Green);
		body_rect.setPosition(seg.postion.x * m_size, seg.postion.y * m_size);
		window->draw(body_rect);
	}
}

void Snake::addSpeed(uint16_t add_speed)
{
    m_speed += add_speed;
}

GamePrompt::GamePrompt(std::shared_ptr<GameWindow> window, const Vector2u& windowSize)
{
	m_window = window;
	m_font.loadFromFile("arial.ttf");
	
	m_gameOverText.setFont(m_font);
	m_gameOverText.setCharacterSize(50);
    m_gameOverText.setFillColor(Color::Red);
    m_gameOverText.setString("Game Over");

	m_continueText.setFont(m_font);
	m_continueText.setCharacterSize(30);
    m_continueText.setFillColor(Color::White);
    m_continueText.setString("Press Enter to Continue");

	initTexts(windowSize);
}

void GamePrompt::initTexts(const Vector2u& windowSize)
{
	FloatRect gameOverBounds = m_gameOverText.getLocalBounds();
    m_gameOverText.setOrigin(gameOverBounds.left + gameOverBounds.width/2.0f,
                            gameOverBounds.top + gameOverBounds.height/2.0f);
    m_gameOverText.setPosition(windowSize.x/2.0f, windowSize.y/2.0f - 30);
    
    FloatRect continueBounds = m_continueText.getLocalBounds();
    m_continueText.setOrigin(continueBounds.left + continueBounds.width/2.0f,
                           continueBounds.top + continueBounds.height/2.0f);
    m_continueText.setPosition(windowSize.x/2.0f, windowSize.y/2.0f + 30);
}

void GamePrompt::render()
{
	m_window->draw(m_gameOverText);
	m_window->draw(m_continueText);
}

SnakeGame::SnakeGame(std::shared_ptr<GameWindow> window, uint16_t world_block_size) : window_(window)
{
    m_world = std::make_shared<World>(static_cast<Vector2f>(window_->getSize()), world_block_size);
    m_snake = std::make_shared<Snake>(world_block_size);
	m_prompt = std::make_shared<GamePrompt>(window, window_->getSize());

}

void SnakeGame::handleInput()
{
    Event event;
    while (window_->getWindow()->pollEvent(event))
    {
        if (event.type == Event::Closed)
        {
            window_->getWindow()->close();
        }

        if (event.type == Event::KeyPressed)
        {
            switch (event.key.code)
            {
				case Keyboard::Escape:
					window_->getWindow()->close();
					break;
                case Keyboard::Up:
					if (m_snake->getDirection() != Direction::Up && m_snake->getDirection() != Direction::Down)
					{ 
						m_snake->setDirection(Direction::Up); 
						std::cout << "Up" << std::endl;
					}
                    break;
                case Keyboard::Down:
                    if (m_snake->getDirection() != Direction::Up && m_snake->getDirection() != Direction::Down)
					{
						m_snake->setDirection(Direction::Down); 
                        std::cout << "Down" << std::endl;
					}
                    break;
                case Keyboard::Left:
                    if (m_snake->getDirection() != Direction::Left && m_snake->getDirection() != Direction::Right)
					{
						m_snake->setDirection(Direction::Left); 
                        std::cout << "Left" << std::endl;
					}
                    break;
                case Keyboard::Right:
                    if (m_snake->getDirection() != Direction::Left && m_snake->getDirection() != Direction::Right)
                    {
						m_snake->setDirection(Direction::Right);
                        std::cout << "Right" << std::endl;
					}
                    break;
                default:
                    break;

            }
        }
    }
}

std::shared_ptr<GameWindow> SnakeGame::getWindow()
{
	return window_;
}

void SnakeGame::render()
{
	window_->drawBegin();
    m_world->Render(window_);
    m_snake->Render(window_);
	window_->drawEnd();
}

void SnakeGame::run()
{
    while (window_->isOpen())
    { 
        handleInput();
        update();
        render();
    }
}

void SnakeGame::update()
{
	m_timeStep = 1.0f / m_snake->getSpeed();
    m_elapsed += m_clock.restart().asSeconds();

    if (m_elapsed >= m_timeStep)
    {
        m_elapsed -= m_timeStep;
		m_snake->Tick();
        m_world->update(m_snake);
        if (!m_snake->isAlive())
        {
            m_snake->reset();
        }
    }

}