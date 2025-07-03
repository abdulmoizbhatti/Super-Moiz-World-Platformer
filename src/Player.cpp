#include "Player.hpp"
#include "Constants.hpp"
#include <SFML/Graphics.hpp>

Player::Player(float x, float y) {
    shape.setSize(sf::Vector2f(40.0f, 60.0f));
    shape.setOrigin(shape.getSize() / 2.0f);
    shape.setPosition(sf::Vector2f(GAME_WIDTH / 2, y));
    shape.setFillColor(sf::Color(255, 87, 51));
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(2.0f);
    onGround = false;
    velocity = sf::Vector2f(0, 0);
    worldX = x;
    worldY = y;
    // Load fighter sprite
    if (!texture.loadFromFile("sprites/craftpix-net-453698-free-shinobi-sprites-pixel-art/Fighter/Idle.png")) {
        // Handle error (optional: fallback or error message)
    }
    sprite = std::make_unique<sf::Sprite>(texture);
    sprite->setOrigin(sf::Vector2f(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f));
    sprite->setPosition(sf::Vector2f(worldX, worldY));
}

void Player::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        velocity.x = -PLAYER_MOVE_SPEED;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        velocity.x = PLAYER_MOVE_SPEED;
    } else {
        velocity.x = 0;
    }
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) && onGround) {
        velocity.y = PLAYER_JUMP_VELOCITY;
        onGround = false;
    }
}

void Player::update(float dt) {
    worldX += velocity.x * dt;
    velocity += GRAVITY * dt;
    worldY += velocity.y * dt;
    shape.setPosition(sf::Vector2f(worldX, worldY));
    if (sprite) sprite->setPosition(sf::Vector2f(worldX, worldY));

    // Animate idle
    animationTimer += dt;
    if (animationTimer >= animationSpeed) {
        animationTimer = 0.f;
        currentFrame = (currentFrame + 1) % frameCount;
        if (sprite) sprite->setTextureRect(sf::IntRect(sf::Vector2i(currentFrame * frameWidth, 0), sf::Vector2i(frameWidth, frameHeight)));
    }
}

void Player::draw(sf::RenderWindow& window) {
    if (sprite) window.draw(*sprite);
    // Optionally draw shape for debugging:
    // window.draw(shape);
} 