#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <set>
#include "../include/Box.hpp"
#include <boost/geometry/index/rtree.hpp>

class Entity : public sf::Drawable
{
public:
    virtual void update() {}

    sf::FloatRect getGlobalBounds(){ return m_shape.getGlobalBounds(); }

protected:
    sf::RectangleShape m_shape;

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override{
        target.draw(m_shape, states);
    }
};

class Invader : public Entity
{
public:
    Invader(const sf::Vector2f& position){
        m_shape.setSize({20,20});
        m_shape.setPosition(position);
        m_shape.setFillColor(sf::Color::Green);
    }
};

class Bullet : public Entity
{
public:
    Bullet(const sf::Vector2f& position){
        m_shape.setSize({2,5});
        m_shape.setPosition(position);
        m_shape.setFillColor(sf::Color::Yellow);
    }

    virtual void update() override { m_shape.move(0,-10); }
};

class Ship : public Entity
{
public:
    Ship(const sf::Vector2f& position){
        m_shape.setSize({100,20});
        m_shape.setPosition(position);
        m_shape.setFillColor(sf::Color::Yellow);
    }

    void move(const sf::Vector2f& offset) { m_shape.move(offset); }

    sf::Vector2f getGunPosition() const{
        return m_shape.getPosition() + sf::Vector2f(m_shape.getSize().x/2, 0.f);
    }
};



int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Invaders");
    window.setFramerateLimit(60);

    //Vector that will hold ship, invaders and bullets
    using Entities = std::vector<std::unique_ptr<Entity>>;
    Entities entities;

    //Push ship into entities and get a pointer to it
    entities.push_back(std::make_unique<Ship>(sf::Vector2f{400,400}));
    auto ship = static_cast<Ship*>(entities[0].get());
    float shipSpeed = 10.f;

    //Push the invaders into entities
    for (int i=0; i<10; ++i)
        for (int j=0; j<5; ++j)
            entities.push_back(std::make_unique<Invader>(sf::Vector2f{i*30.f,j*30.f}));

    std::set<Entity*> destroyed; //to mark entities for destruction

    //Create rtree that will hold the bounding rectangles of entities
    //and an integer, identifying their position in entities vector
    namespace bgi = boost::geometry::index;
    using BoxIdPair = std::pair<sf::FloatRect, int>;
    bgi::rtree<BoxIdPair, bgi::quadratic<16>> rtree;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            ship->move({-shipSpeed, 0.f});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            ship->move({shipSpeed, 0.f});

        //if player presses 'fire' (spacebar), create a new bullet
        //at the gun's position (middle front of the ship)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            entities.push_back(std::make_unique<Bullet>(ship->getGunPosition()));

        //update
        for (auto& entity : entities)
            entity->update();

        //Collision detection excluding ship

        //clear rtree and refill with bounding rectangles of entities
        rtree.clear();
        for (std::size_t i=1; i<entities.size(); ++i)
            rtree.insert(std::make_pair(entities[i]->getGlobalBounds(),i));

        //Put all entities that collided in a vector
        for (std::size_t i=1; i<entities.size(); ++i){
            std::vector<BoxIdPair> collidedEntities;
            rtree.query(bgi::intersects(entities[i]->getGlobalBounds()),
                        std::back_inserter(collidedEntities));
            //mark all collided entities for destruction
            for (auto collidedEntity : collidedEntities){
                if (i!=collidedEntity.second){ //exclude collision with self
                    destroyed.insert(entities[i].get());
                    destroyed.insert(entities[collidedEntity.second].get());
                }
            }
        }

        //erase all entities marked for destruction
        entities.erase(std::remove_if(entities.begin(), entities.end(),
                                      [&destroyed](auto& entity){
                           return destroyed.count(entity.get());}),
                entities.end());
        destroyed.clear();

        //clear, draw and display
        window.clear();
        for (const auto& entity : entities)
            window.draw(*entity);
        window.display();
    }
}
