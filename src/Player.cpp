#include "Player.hpp"
#include "Constants.hpp"
#include <SFML/Graphics.hpp>
#include <string>

enum class AnimationType { Idle, Run, Jump };
static AnimationType currentAnim = AnimationType::Idle;

Player::Player(float x, float y) {
    shape.setSize(sf::Vector2f(24.0f, 28.0f));
    shape.setOrigin(shape.getSize() / 2.0f);
    shape.setPosition(sf::Vector2f(GAME_WIDTH / 2, y));
    shape.setFillColor(sf::Color(255, 87, 51));
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(2.0f);
    onGround = false;
    velocity = sf::Vector2f(0, 0);
    worldX = x;
    worldY = y;
    // Load idle sprite by default
    if (!texture.loadFromFile("sprites/craftpix-net-453698-free-shinobi-sprites-pixel-art/Fighter/Idle.png")) {
        // Handle error
    }
    sprite = std::make_unique<sf::Sprite>(texture);
    sprite->setOrigin(sf::Vector2f(frameWidth / 2.0f, frameHeight));
    sprite->setPosition(sf::Vector2f(worldX, worldY + spriteYOffset));
    // Set initial texture rect
    sprite->setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(frameWidth, frameHeight)));
}

void Player::setAnimationState(bool running, bool force) {
    AnimationType newAnim = isJumping ? AnimationType::Jump : (running ? AnimationType::Run : AnimationType::Idle);
    bool animChanged = (newAnim != this->currentAnim) || force;
    if (animChanged) {
        this->currentAnim = newAnim;
        currentFrame = 0;
        animationTimer = 0.f;
        if (newAnim == AnimationType::Jump) {
            texture.loadFromFile("sprites/craftpix-net-453698-free-shinobi-sprites-pixel-art/Fighter/Jump.png");
            frameCount = jumpFrameCount;
            frameWidth = jumpFrameWidth;
        } else if (newAnim == AnimationType::Run) {
            texture.loadFromFile("sprites/craftpix-net-453698-free-shinobi-sprites-pixel-art/Fighter/Run.png");
            frameCount = runFrameCount;
            frameWidth = runFrameWidth;
        } else {
            texture.loadFromFile("sprites/craftpix-net-453698-free-shinobi-sprites-pixel-art/Fighter/Idle.png");
            frameCount = 6;
            frameWidth = 128;
        }
        if (sprite) {
            sprite->setTexture(texture);
            sprite->setOrigin(sf::Vector2f(frameWidth / 2.0f, frameHeight));
            sprite->setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(frameWidth, frameHeight)));
        }
    }
}

void Player::handleInput() {
    bool running = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        velocity.x = -PLAYER_MOVE_SPEED;
        running = true;
        if (facingRight) {
            facingRight = false;
            if (sprite) {
                sprite->setScale(sf::Vector2f(-1.f, 1.f));
                sprite->setOrigin(sf::Vector2f(frameWidth / 2.0f, frameHeight));
            }
        }
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        velocity.x = PLAYER_MOVE_SPEED;
        running = true;
        if (!facingRight) {
            facingRight = true;
            if (sprite) {
                sprite->setScale(sf::Vector2f(1.f, 1.f));
                sprite->setOrigin(sf::Vector2f(frameWidth / 2.0f, frameHeight));
            }
        }
    } else {
        velocity.x = 0;
    }
    // Jumping
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) && onGround) {
        velocity.y = PLAYER_JUMP_VELOCITY;
        onGround = false;
        isJumping = true;
        setAnimationState(running, true);
    } else if (onGround && isJumping) {
        isJumping = false;
        setAnimationState(running, true);
    } else {
        setAnimationState(running);
    }
}

void Player::update(float dt) {
    worldX += velocity.x * dt;
    velocity += GRAVITY * dt;
    worldY += velocity.y * dt;
    shape.setPosition(sf::Vector2f(worldX, worldY));
    if (sprite) sprite->setPosition(sf::Vector2f(worldX, worldY + spriteYOffset));

    // Animate
    animationTimer += dt;
    if (animationTimer >= animationSpeed) {
        animationTimer = 0.f;
        currentFrame = (currentFrame + 1) % frameCount;
        if (sprite) sprite->setTextureRect(sf::IntRect(sf::Vector2i(currentFrame * frameWidth, 0), sf::Vector2i(frameWidth, frameHeight)));
    }
}

void Player::draw(sf::RenderWindow& window) {
    if (sprite) window.draw(*sprite);
    // draw shape for debugging:
    // window.draw(shape);
} 