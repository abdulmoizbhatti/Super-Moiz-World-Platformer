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
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(shape);
} 