#include "Platform.hpp"
#include <SFML/Graphics.hpp>
#include "Constants.hpp"

Platform::Platform(const sf::Vector2f& size, const sf::Vector2f& position) {
    shape.setSize(size);
    shape.setPosition(position);
    // main ground platform =green, otherwise, keep it brown
    if (size.y == 50.0f && position.y >= GAME_HEIGHT - 50.0f) {
        shape.setFillColor(sf::Color(20, 70, 20)); // dark green
    } else {
        shape.setFillColor(sf::Color(139, 69, 19)); // brown for other platforms
    }
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(2.0f);
}

void Platform::draw(sf::RenderWindow& window) {
    window.draw(shape);
} 