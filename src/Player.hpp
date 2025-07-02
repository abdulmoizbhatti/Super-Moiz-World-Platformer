#pragma once
#include <SFML/Graphics.hpp>

class Player {
public:
    Player(float x, float y);
    void handleInput();
    void update(float dt);
    void draw(sf::RenderWindow& window);
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    bool onGround;
    float worldX; // Player's position in the world (for camera)
    float worldY; // Player's vertical position in the world
}; 