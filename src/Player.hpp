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
    float spriteYOffset = 14.f; // tweak this value for ground alignment
    // Run animation
    int runFrameCount = 8; // Run.png has 8 frames
    int runFrameWidth = 128;
    int runFrameHeight = 128;
    bool isRunning = false;
    bool facingRight = true;
    // Jump animation
    int jumpFrameCount = 10; // Jump.png has 10 frames
    int jumpFrameWidth = 128;
    bool isJumping = false;
    enum class AnimationType { Idle, Run, Jump };
    AnimationType currentAnim = AnimationType::Idle;
    void setAnimationState(bool running, bool force = false);
}; 