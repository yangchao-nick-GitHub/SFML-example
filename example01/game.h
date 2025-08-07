#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

#include "window.h"

class Game {
public:
	Game(std::shared_ptr<GameWindow> window);
	void handleInput();
	void update();
	void render();
	std::shared_ptr<GameWindow> getWindow();
	void clockRestart();
	
private:
	std::shared_ptr<GameWindow> window_ {nullptr};
	Sprite sprite_;
	Texture texture_;
	Vector2f increment{ 400, 400 };
	Clock clock_;
	Time m_elapsed;
};