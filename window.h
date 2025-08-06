#pragma once

#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;

class GameWindow {
public:
    GameWindow();
    GameWindow(const std::string title, const sf::Vector2u size, bool fullscreen = false);
    ~GameWindow();

	void Draw(Drawable& drawable);


private:

	void setUp(const std::string title, const Vector2u size);
	void Destroy();
	void create();

	bool is_fullscreen_{ false };
	std::string title_;
	Vector2u size_;
	RenderWindow window_;
};



