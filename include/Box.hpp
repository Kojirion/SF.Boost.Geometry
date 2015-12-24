#pragma once
#include "Point.hpp"
#include <boost/geometry.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace boost
{
namespace geometry
{
namespace traits
{
template<> struct tag<sf::FloatRect> { typedef box_tag type; };
template<> struct point_type<sf::FloatRect > { typedef sf::Vector2f type; };

template <> struct indexed_access<sf::FloatRect, min_corner, 0>
{
    typedef coordinate_type<sf::Vector2f>::type ct;
    static inline ct get(sf::FloatRect const& b) { return b.left;  }
    static inline void set(sf::FloatRect& b, ct const& value) { b.left = value; }
};
template <> struct indexed_access<sf::FloatRect, min_corner, 1>
{
    typedef coordinate_type<sf::Vector2f>::type ct;
    static inline ct get(sf::FloatRect const& b) { return b.top-b.height;  }
    static inline void set(sf::FloatRect& b, ct const& value) { b.top = value+b.height; }
};
template <> struct indexed_access<sf::FloatRect, max_corner, 0>
{
    typedef coordinate_type<sf::Vector2f>::type ct;
    static inline ct get(sf::FloatRect const& b) { return b.left+b.width;  }
    static inline void set(sf::FloatRect& b, ct const& value) { b.width = value-b.left; }
};
template <> struct indexed_access<sf::FloatRect, max_corner, 1>
{
    typedef coordinate_type<sf::Vector2f>::type ct;
    static inline ct get(sf::FloatRect const& b) { return b.top; }
    static inline void set(sf::FloatRect& b, ct const& value) { b.top = value; }
};
}
}
}
