#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <memory>
#include <boost/geometry.hpp>
#include "../include/Shape.hpp"

#include <boost/geometry/io/wkt/write.hpp>
#include <iostream>

void dim(sf::Shape& shape) {
    auto color(shape.getFillColor());
    color.a = 128;
    shape.setFillColor(color);
}

void light(sf::Shape& shape) {
    auto color(shape.getFillColor());
    color.a = 255;
    shape.setFillColor(color);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Shape intersection");
    window.setFramerateLimit(60);

    std::vector<std::unique_ptr<sf::Shape>> shapes;

    shapes.emplace_back(new sf::RectangleShape({100, 100}));
    auto hexagon = new sf::ConvexShape(6);
    const float side_length = 40.f;

    hexagon->setPosition(400, 400);

    hexagon->setPoint(0, sf::Vector2f(0, 0));
    hexagon->setPoint(1, sf::Vector2f((side_length*sqrt(3.f))/2, side_length/2));
    hexagon->setPoint(2, sf::Vector2f((side_length*sqrt(3.f))/2, 3*side_length/2));
    hexagon->setPoint(3, sf::Vector2f(0,2*side_length));
    hexagon->setPoint(4, sf::Vector2f(-(side_length*sqrt(3.f))/2, 3*side_length/2));
    hexagon->setPoint(5, sf::Vector2f(-(side_length*sqrt(3.f))/2, side_length/2));

    shapes.emplace_back(hexagon);

    auto circle = new sf::CircleShape;
    circle->setPosition(700.f, 500.f);
    circle->setRadius(50.f);

    shapes.emplace_back(circle);

    unsigned int current = 0;


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::Space){
                    current = (current + 1) % shapes.size();
                }
            }


        }

        sf::Vector2f toMoveBy, toScaleBy(1.f, 1.f);
        float toRotateBy = 0.f, moveFactor = 5.f, rotationFactor = 10.f, scaleFactor = 0.1f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            toRotateBy -= rotationFactor;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            toRotateBy += rotationFactor;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            toRotateBy += rotationFactor;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            toRotateBy -= rotationFactor;

        shapes[current]->rotate(toRotateBy);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            toMoveBy.y -= moveFactor;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            toMoveBy.y += moveFactor;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            toMoveBy.x += moveFactor;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            toMoveBy.x -= moveFactor;

        shapes[current]->move(toMoveBy);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            toScaleBy.x += scaleFactor;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
            toScaleBy.y += scaleFactor;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
            toScaleBy.x -= scaleFactor;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
            toScaleBy.y -= scaleFactor;

        shapes[current]->scale(toScaleBy);

        for (auto& shape : shapes)
            shape->setFillColor(sf::Color(255, 0, 0, 128));

        for (std::size_t i=0; i<shapes.size(); ++i){
            for (std::size_t j=0; j<shapes.size(); ++j) {
                if (i==j) continue;
                if (boost::geometry::intersects(points_of(*shapes[i]),
                                                points_of(*shapes[j]))){
                    shapes[i]->setFillColor(sf::Color(0, 255, 0, 128));
                    shapes[j]->setFillColor(sf::Color(0, 255, 0, 128));
                }
            }
        }

        light(*shapes[current]);

        window.clear();
        for (const auto& shape : shapes)
            window.draw(*shape);
        window.display();
    }
}

