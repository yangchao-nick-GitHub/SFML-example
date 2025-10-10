#include "window.h"

using namespace sf;

GameWindow::GameWindow(const std::string title, const Vector2u size, bool fullscreen)
	: is_fullscreen_(fullscreen), size_(size), title_(title)
{
	create();
}

GameWindow::GameWindow()
	: is_fullscreen_(false), size_(800, 600), title_("test")
{
	create();
}

void GameWindow::create()
{
	auto style = is_fullscreen_ ? Style::Fullscreen : Style::Default;
	window_.create(VideoMode(size_.x, size_.y), title_, style);
	window_.setFramerateLimit(60);
}


Vector2u GameWindow::getSize() const
{
	return size_;
}

RenderWindow* GameWindow::getWindow()
{
	return &window_;
}

bool GameWindow::pollEvent(Event& event)
{
	return window_.pollEvent(event);
}

void GameWindow::close()
{
	window_.close();
}

GameWindow::~GameWindow()
{
	close();
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

bool GameWindow::isOpen()
{
	return window_.isOpen();
}




