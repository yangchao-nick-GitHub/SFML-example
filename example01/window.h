#pragma once

#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;

class GameWindow {
public:
	GameWindow();
	GameWindow(const std::string title, const sf::Vector2u size, bool fullscreen = false);
	~GameWindow();

	void draw(Drawable& drawable);
	void drawBegin();
	void drawEnd();
	bool isOpen();
	void update();
	void triggerFullscreen();
	Vector2u getSize() const;
	RenderWindow* getWindow();




private:
	void destroy();
	void create();

	bool is_fullscreen_{ false };
	std::string title_;
	Vector2u size_;
	RenderWindow window_;
};



