#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "SFML works!");
    sf::CircleShape shape(100.f);
    
    sf::Texture image;
    if (!image.loadFromFile ("../images/system/dragon_x.png"))
        return -1;

    sf::Texture image1;
    if (!image1.loadFromFile ("../images/system/blue_background.jpg"))
        return -1;
    
    sf::Sprite sprite;
    sprite.setTexture(image);
    // sprite.setColor (sf::Color (128, 128, 128));

    sf::Sprite sprite_trans;
    sprite_trans.setTexture(image1);
    sprite_trans.setPosition (200, 100);
    sprite_trans.setColor (sf::Color (128, 128, 128, 230));

    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.draw(sprite);
        window.draw(sprite_trans);
        window.display();
    }

    return 0;
}