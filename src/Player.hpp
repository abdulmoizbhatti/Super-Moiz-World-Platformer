#pragma once
#include <SFML/Graphics.hpp>

class Player {
public:
    Player(float x, float y);
    void handleInput();
    void update(float dt);
    void draw(sf::RenderWindow& window);
    sf::RectangleShape shape;
    sf::Texture texture;
    std::unique_ptr<sf::Sprite> sprite;
    sf::Vector2f velocity;
    bool onGround;
    float worldX; // Player's position in the world (for camera)
    float worldY; // Player's vertical position in the world
    // Animation state
    int currentFrame = 0;
    int frameCount = 6; // Idle.png has 6 frames
    int frameWidth = 128;
    int frameHeight = 128;
    float animationTimer = 0.f;
    float animationSpeed = 0.12f; // seconds per frame
}; 