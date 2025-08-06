#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

using namespace sf;

int main()
{
    int windowWidth = 800;
    int windowHeight = 600;
      
    RenderWindow window(VideoMode(windowWidth, windowHeight), "test");

    Texture texture;
    if (!texture.loadFromFile("mary.png"))
    {
        std::cerr << "Error loading texture" << std::endl;
        return -1;
    }
    Sprite sprite(texture);

    Vector2u tex_size = texture.getSize();
    sprite.setOrigin(tex_size.x / 2, tex_size.y / 2);

    Vector2f increment(0.2f, 0.2f);







    

    while (window.isOpen())
    { 
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed ||
               Keyboard::isKeyPressed(Keyboard::Escape))
            {
                window.close();
            }

        }


        // 更新移动坐标
        if (sprite.getPosition().x + tex_size.x / 2 > window.getSize().x && increment.x > 0 ||
            sprite.getPosition().x - tex_size.x / 2 < 0 && increment.x < 0)
        {
            increment.x = -increment.x;
        }

        if (sprite.getPosition().y + tex_size.y / 2 > window.getSize().y && increment.y > 0 ||
            sprite.getPosition().y - tex_size.y / 2 < 0 && increment.y < 0)
        {
            increment.y = -increment.y;
        }

        sprite.setPosition(sprite.getPosition() + increment);

		window.clear(sf::Color::Black);
        window.draw(sprite);
        window.display();
    }

    
    

    return 0;
}
