#include <iostream>
#include "game.h"


Game::Game(std::shared_ptr<GameWindow> window) : window_(window) 
{
    if (!texture_.loadFromFile("bob.png"))
    {
        std::cerr << "Error loading texture" << std::endl;
        return;
    }
	sprite_.setTexture(texture_);
    sprite_.setOrigin(texture_.getSize().x / 2, texture_.getSize().y / 2);
}

void Game::handleInput()
{
	// ....
}

void Game::update()
{
    if (sprite_.getPosition().x + texture_.getSize().x / 2 > window_->getSize().x && increment.x > 0 ||
        sprite_.getPosition().x - texture_.getSize().x / 2 < 0 && increment.x < 0)
    {
        increment.x = -increment.x;
    }

    if (sprite_.getPosition().y + texture_.getSize().y / 2 > window_->getSize().y && increment.y > 0 ||
        sprite_.getPosition().y - texture_.getSize().y / 2 < 0 && increment.y < 0)
    {
        increment.y = -increment.y;
    }

    float fElapsed = m_elapsed.asSeconds();

    sprite_.setPosition(sprite_.getPosition().x + increment.x * fElapsed, sprite_.getPosition().y + increment.y * fElapsed);
}

std::shared_ptr<GameWindow> Game::getWindow()
{
	return window_;
}

void Game::clockRestart()
{
    m_elapsed = clock_.restart();
}

void Game::render()
{
	window_->drawBegin();
	window_->draw(sprite_);
	window_->drawEnd();
}