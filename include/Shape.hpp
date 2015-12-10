#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/iterator_range.hpp>
#include <SFML/Graphics.hpp>
#include "Point.hpp"
#include <boost/geometry/geometries/register/ring.hpp>

class ShapeIterator : public boost::iterator_facade<ShapeIterator,
        sf::Vector2f const, boost::random_access_traversal_tag, sf::Vector2f> {
public:
    ShapeIterator(const sf::Shape* shape = nullptr, unsigned int index = 0):
        m_shape(shape), m_index(index)
    {

    }

private:
    friend class boost::iterator_core_access;

    void increment() {
        ++m_index;
    }

    void decrement() {
        --m_index;
    }

    sf::Vector2f dereference() const {
        return m_shape->getTransform().transformPoint(m_shape->getPoint(m_index));
    }

    bool equal(const ShapeIterator &other) const {
        return m_index == other.m_index;
    }

    void advance(int n) {
        m_index += n;
    }

    int distance_to(const ShapeIterator& other) const {
        return (other.m_index - static_cast<int>(m_index));
    }

    const sf::Shape* m_shape;
    unsigned int m_index;
};

auto points_of(const sf::Shape& shape)
{
    return boost::make_iterator_range(ShapeIterator(&shape), ShapeIterator(&shape, shape.getPointCount()));
}

using ShapePoints = decltype(points_of(sf::CircleShape()));


BOOST_GEOMETRY_REGISTER_RING(ShapePoints)
