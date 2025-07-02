#include "Platform.hpp"
#include <SFML/Graphics.hpp>

Platform::Platform(const sf::Vector2f& size, const sf::Vector2f& position) {
    shape.setSize(size);
    shape.setPosition(position);
    shape.setFillColor(sf::Color(139, 69, 19));
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(2.0f);
}

void Platform::draw(sf::RenderWindow& window) {
    window.draw(shape);
} 