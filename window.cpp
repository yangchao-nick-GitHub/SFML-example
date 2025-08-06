#include <SFML/Graphics.hpp>
#include "window.h"

using namespace sf;

GameWindow::GameWindow(const std::string title, const Vector2u size, bool fullscreen)
: is_fullscreen_(fullscreen)
{
	setUp(title, size);
}

GameWindow::GameWindow()
	: is_fullscreen_(false)
{
	setUp("test", Vector2u(800, 600));
}

void GameWindow::setUp(const std::string title, const Vector2u size)
{
	title_ = title;
    size_ = size;
	create();
}

void GameWindow::create()
{
	auto style = is_fullscreen_ ? Style::Fullscreen : Style::Default;
	window_.create(VideoMode(size_.x, size_.y), title_, style);
}

void GameWindow::Destroy()
{
	window_.close();
}

GameWindow::~GameWindow()
{
	Destroy();
}

//void Draw(Drawable& drawable)
//{
//	//void Draw(Drawable & drawable)
//
//}
