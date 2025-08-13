#pragma once

#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;


class GameWindow {
public:
	GameWindow();
	GameWindow(const std::string title, const sf::Vector2u size, bool fullscreen = false);
	virtual ~GameWindow();
	void draw(Drawable& drawable);
	virtual void drawBegin();
	virtual void drawEnd();
	Vector2u getSize() const;
	bool isOpen();
	RenderWindow* getWindow();
	virtual void handleInput() = 0;

protected:
	RenderWindow window_;
private:
	void destroy();
	void create();

	bool is_fullscreen_{ false };
	std::string title_;
	Vector2u size_;

};

class SnakeGameWindow : public GameWindow {
public:
	SnakeGameWindow();
	SnakeGameWindow(const std::string title, const sf::Vector2u size, bool fullscreen = false);
	~SnakeGameWindow() = default;
	void handleInput() override;
};



