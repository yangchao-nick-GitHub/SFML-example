#include <SFML/Graphics.hpp>

#include "window.h"

using namespace sf;

GameWindow::GameWindow(const std::string title, const Vector2u size, bool fullscreen)
	: is_fullscreen_(fullscreen), size_(size), title_(title)
{
	create();
}

GameWindow::GameWindow()
	: is_fullscreen_(false),size_(800, 600), title_("test")
{
	create();
}

void GameWindow::create()
{
	auto style = is_fullscreen_ ? Style::Fullscreen : Style::Default;
	window_.create(VideoMode(size_.x, size_.y), title_, style);
}

bool GameWindow::isOpen()
{
	return window_.isOpen();
}

void GameWindow::update()
{
	Event event;
	while (window_.pollEvent(event))
	{
		if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window_.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Num9))
		{
			triggerFullscreen();
		}
	}
}

void GameWindow::triggerFullscreen()
{
	is_fullscreen_ = !is_fullscreen_;
	destroy();
	create();
}

Vector2u GameWindow::getSize() const
{
	return size_;
}

RenderWindow* GameWindow::getWindow()
{
	return &window_;
}

void GameWindow::destroy()
{
	window_.close();
}

GameWindow::~GameWindow()
{
	destroy();
}

void GameWindow::drawBegin()
{
	window_.clear(Color::Black);
}

void GameWindow::drawEnd()
{
	window_.display();
}

void GameWindow::draw(Drawable& drawable)
{
	window_.draw(drawable);
}
