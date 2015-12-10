#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Boxes");
    window.setFramerateLimit(60);

    sf::CircleShape shape(3.f);
    shape.setOrigin(3.f, 3.f);
    shape.setFillColor(sf::Color::Yellow);
    window.setMouseCursorVisible(false);

    sf::RectangleShape rect({400, 150});
    rect.setPosition(10, 10);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineThickness(1.f);
    rect.setOutlineColor(sf::Color::Red);



    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        auto mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        shape.setPosition(mousePosition);
        if (rect.getGlobalBounds().contains(mousePosition))
            rect.setOutlineColor(sf::Color::Green);
        else
            rect.setOutlineColor(sf::Color::Red);

        window.clear();
        window.draw(shape);
        window.draw(rect);
        window.display();
    }
}
