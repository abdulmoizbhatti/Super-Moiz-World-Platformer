#pragma once
#include <SFML/Graphics.hpp>

class Platform {
public:
    Platform(const sf::Vector2f& size, const sf::Vector2f& position);
    void draw(sf::RenderWindow& window);
    sf::RectangleShape shape;
}; 